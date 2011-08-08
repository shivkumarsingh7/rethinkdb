#ifndef __BTREE_NODE_FUNCTIONS_HPP__
#define __BTREE_NODE_FUNCTIONS_HPP__

// We have to have this file because putting these definitions in node.hpp would require mutually recursive header files.

#include "btree/node.hpp"
#include "btree/leaf_node.hpp"

namespace node {

template <class V>
bool is_underfull(value_sizer_t<V> *sizer, const node_t *node) {
    if (node->magic == sizer->btree_leaf_magic()) {
        return leaf::is_underfull(sizer, reinterpret_cast<const loof_t *>(node));
    } else {
        rassert(is_internal(node));
        return internal_node::is_underfull(sizer->block_size(), reinterpret_cast<const internal_node_t *>(node));
    }
}

template <class V>
bool is_mergable(value_sizer_t<V> *sizer, const node_t *node, const node_t *sibling, const internal_node_t *parent) {
    if (sizer->btree_leaf_magic() == node->magic) {
        return loof::is_mergable(sizer, node, sibling);
    } else {
        rassert(is_internal(node));
        return internal_node::is_mergable(block_size, reinterpret_cast<const internal_node_t *>(node), reinterpret_cast<const internal_node_t *>(sibling), parent);
    }
}

template <class Value>
void split(value_sizer_t<Value> *sizer, buf_t *node_buf, node_t *rnode, btree_key_t *median) {
    if (is_leaf(reinterpret_cast<const node_t *>(node_buf->get_data_read()))) {
        leaf::split(sizer, node_buf, reinterpret_cast<leaf_node_t *>(rnode), median);
    } else {
        internal_node::split(sizer->block_size(), node_buf, reinterpret_cast<internal_node_t *>(rnode), median);
    }
}

template <class Value>
void merge(value_sizer_t<Value> *sizer, const node_t *node, buf_t *rnode_buf, btree_key_t *key_to_remove, const internal_node_t *parent) {
    if (is_leaf(node)) {
        leaf::merge(sizer, reinterpret_cast<const leaf_node_t *>(node), rnode_buf, key_to_remove);
    } else {
        internal_node::merge(sizer->block_size(), reinterpret_cast<const internal_node_t *>(node), rnode_buf, key_to_remove, parent);
    }
}

template <class Value>
bool level(value_sizer_t<Value> *sizer, buf_t *node_buf, buf_t *rnode_buf, btree_key_t *key_to_replace, btree_key_t *replacement_key, const internal_node_t *parent) {
    if (is_leaf(reinterpret_cast<const node_t *>(node_buf->get_data_read()))) {
        return leaf::level(sizer, node_buf, rnode_buf, key_to_replace, replacement_key);
    } else {
        return internal_node::level(sizer->block_size(), node_buf, rnode_buf, key_to_replace, replacement_key, parent);
    }
}

template <class Value>
void validate(UNUSED value_sizer_t<Value> *sizer, UNUSED const node_t *node) {
#ifndef NDEBUG
    if (node->magic == sizer->btree_leaf_magic()) {
        leaf::validate(sizer, reinterpret_cast<const leaf_node_t *>(node));
    } else if (node->magic == internal_node_t::expected_magic) {
        internal_node::validate(sizer->block_size(), reinterpret_cast<const internal_node_t *>(node));
    } else {
        unreachable("Invalid leaf node type.");
    }
#endif
}



}  // namespace node


#endif  // __BTREE_NODE_FUNCTIONS_HPP__
