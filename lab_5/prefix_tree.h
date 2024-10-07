#ifndef LAB_5_PREFIX_TREE_H
#define LAB_5_PREFIX_TREE_H

#include <map>
#include <string>
#include <iostream>
class suffix_tree
{
    class node
    {
        friend class suffix_tree;
        std::map<char, node*> children;
        int start;
        int* end;

        node* suffix_link;

        node(int start, int* end) : start(start), end(end), suffix_link(nullptr){};
    };

public:
    std::string txt;

    node* root;
    node* last_added_node = nullptr;
    node* active_node = nullptr;

    int active_edge = -1;
    int active_length = 0;

    int suffixes_remained = 0;
    int end_leaf_index = -1;

    int* root_end = nullptr;
    int* split_end = nullptr;

public:
    suffix_tree(std::string const & text);

    std::string do_work();

private:

    void Ukkonen_algo(int txt_pos);

    node* create_node(int start, int* end);

    int count_length(node* current_node);

    void  find_pos(node* node, int depth, int& minimal_cut, int& minimal_depth);


};
#endif
