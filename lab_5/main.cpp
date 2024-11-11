#include <map>
#include <string>
#include <iostream>

class suffix_tree{
    class node {
        friend class suffix_tree;
        std::map<char, node*> children;
        int start;
        int* end;
        node* suffix_link;
        node(int start, int* end) : start(start), end(end), suffix_link(nullptr) {};
    };

private:
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
    explicit suffix_tree(std::string& text);
    ~suffix_tree();
    std::string do_work();

private:
    void Ukkonen_algo(int txt_pos);
    node* create_node(int start, int* end);
    int count_length(node* current_node);
    void delete_subtree(node* root);
};

typename suffix_tree::node* suffix_tree::create_node(int start, int* end)
{
    return new node(start, end);
}

int suffix_tree::count_length(suffix_tree::node* current_node) {
    if (!current_node)
        return 0;
    return *(current_node->end) - current_node->start + 1;
}

suffix_tree::suffix_tree(std::string& text) : txt(std::move(text + text + "$"))
{
    root_end = new int();
    *root_end = -1;
    root = new node(-1, root_end);
    active_node = root;
    for (int pos = 0; pos < txt.size(); ++pos)
        Ukkonen_algo(pos);
}

suffix_tree::~suffix_tree()
{
    delete root_end;

    delete split_end;

    delete_subtree(root);

}
void suffix_tree::delete_subtree(suffix_tree::node *subtree_root)
{
    if(!subtree_root)
        return;

    for(auto& child : subtree_root->children)
        delete_subtree(child.second);

    delete subtree_root;
}
void suffix_tree::Ukkonen_algo(int txt_pos)
{
    if(txt_pos == txt.size() - 1)
    {
        root->children['$'] = nullptr;
        (*root_end)++;
        return;
    }

    suffixes_remained++;
    end_leaf_index = txt_pos;
    last_added_node = nullptr;

    while (suffixes_remained) {
        if (active_length == 0)
            active_edge = txt_pos;

        if (!active_node->children[txt[active_edge]]) {
            active_node->children[txt[active_edge]] = create_node(txt_pos, &end_leaf_index);

            if (last_added_node) {
                last_added_node->suffix_link = active_node;
                last_added_node = nullptr;
            }

        } else {
            node* jump = active_node->children[txt[active_edge]];
            int jumped_length = count_length(jump);

            if (active_length >= jumped_length) {
                active_edge += jumped_length;
                active_length -= jumped_length;
                active_node = jump;
                continue;
            }

            if (txt[jump->start + active_length] == txt[txt_pos]) {
                active_length++;
                if (last_added_node && active_node != root) {
                    last_added_node->suffix_link = active_node;
                    last_added_node = nullptr;
                }
                break;
            }

            split_end = new int(jump->start + active_length - 1);
            node* split = create_node(jump->start, split_end);
            active_node->children[txt[active_edge]] = split;

            split->children[txt[txt_pos]] = create_node(txt_pos, &end_leaf_index);
            jump->start += active_length;
            split->children[txt[jump->start]] = jump;

            if (last_added_node)
                last_added_node->suffix_link = split;

            last_added_node = split;
        }

        suffixes_remained--;

        if (active_node == root && active_length > 0) {
            active_length--;
            active_edge = txt_pos - suffixes_remained;
        } else if (active_node != root) {
            active_node = active_node->suffix_link;
        }
    }
}

std::string suffix_tree::do_work()
{
    std::string result_string;
    size_t length = (txt.size() - 1) / 2;
    node *next = root;

    while (result_string.size() < length)
    {
        auto it = next->children.begin();

        if (it->first == '$')
            ++it;

        next = it->second;

        for (int i = next->start; i <= *(next->end); ++i)
        {
            result_string += txt[i];

            if (result_string.size() == length)
                break;
        }
    }
    return result_string;
}

int main()
{
    std::string text;
    std::cin>>text;
    suffix_tree tree(text);
    std::cout<<tree.do_work();

    return 0;
}