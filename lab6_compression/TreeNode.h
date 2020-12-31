#ifndef LAB6_COMPRESSION_TREENODE_H
#define LAB6_COMPRESSION_TREENODE_H


class TreeNode {
public:
    explicit TreeNode(unsigned char byte, size_t priority);

    void add_child(TreeNode *child);

    unsigned char get_byte() const;
    TreeNode *get_left() const;
    TreeNode *get_right() const;
    size_t get_priority() const;

    ~TreeNode();
private:
    unsigned char _byte;
    size_t _priority;
    TreeNode *_left;
    TreeNode *_right;
};


#endif //LAB6_COMPRESSION_TREENODE_H
