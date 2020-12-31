#include <stdexcept>
#include "TreeNode.h"

TreeNode::TreeNode(unsigned char byte, size_t priority)
    : _byte(byte), _left(nullptr), _right(nullptr), _priority(priority) {
}

TreeNode::~TreeNode() {
    delete _left;
    delete _right;
}

void TreeNode::add_child(TreeNode *child) {
    if (!_left) {
        _left = child;
    } else if (!_right) {
        _right = child;
    } else {
        throw std::logic_error("already 2 children");
    }
}

unsigned char TreeNode::get_byte() const {
    return _byte;
}

TreeNode *TreeNode::get_left() const {
    return _left;
}

TreeNode *TreeNode::get_right() const {
    return _right;
}

size_t TreeNode::get_priority() const {
    return _priority;
}
