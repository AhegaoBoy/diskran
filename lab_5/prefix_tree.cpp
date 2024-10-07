#include "prefix_tree.h"

typename suffix_tree::node* suffix_tree::create_node(int start, int *end)
{
    return new node(start, end);
}

int suffix_tree::count_length(suffix_tree::node *current_node)
{
    if(!current_node)
        return 0;
    return *(current_node->end) - current_node->start + 1;
}

suffix_tree::suffix_tree(const std::string &text)
{
    txt = text + "$";

    root_end = new int();
    *root_end = -1;

    root = new node(-1, root_end);
    active_node = root;

    for(int pos = 0; pos < txt.size(); ++pos)
        Ukkonen_algo(pos);
}

void suffix_tree::Ukkonen_algo(int txt_pos)
{
    suffixes_remained++;
    end_leaf_index = txt_pos;
    last_added_node = nullptr;

    while(suffixes_remained)
    {
        if(!active_length)
            active_edge = txt_pos;

        if(active_node->children.find(txt[active_edge]) == active_node->children.end())
        {
            active_node->children[txt[active_edge]] = create_node(txt_pos, &end_leaf_index);

            if(last_added_node)
            {
                last_added_node->suffix_link = active_node;
                last_added_node = nullptr;
            }

        }
        else
        {
            node* jump = active_node->children[txt[txt_pos]];

            if(!jump)
                break;

            int  jumped_length = count_length(jump);

            if(active_length >= jumped_length)
            {
                active_edge += jumped_length;
                active_length -= jumped_length;
                active_node = jump;
                continue;
            }

            if(txt[jump->start + active_length] == txt[txt_pos])
            {
                active_length++;

                if(last_added_node && active_node != root)
                {
                    last_added_node->suffix_link = active_node;
                    last_added_node = nullptr;
                }
                break;
            }

            split_end = new int();

            *split_end = jump->start + active_edge - 1;

            node* split = create_node(jump->start, jump->end);
            active_node->children[txt[active_edge]] = split;

            split->children[txt[txt_pos]] = create_node(txt_pos, &end_leaf_index);
            jump->start += active_length;
            split->children[txt[jump->start]] = jump;

            if(last_added_node)
                last_added_node->suffix_link = split;

            last_added_node = split;
        }

        --suffixes_remained;

        if(active_node == root && active_length > 0)
        {
            --active_length;
            active_edge = txt_pos - suffixes_remained + 1;
        }
        else if(active_node != root)
            active_node = active_node->suffix_link;
    }
}

void suffix_tree::find_pos(suffix_tree::node *node, int depth, int &minimal_cut, int &minimal_depth)
{
    if (!node) return;

    if (node != root) {
        int start = node->start;
        int end = *(node->end);
        int length = (end - start + 1);

        // Если это лист и длина суффикса меньше текущего минимального
        if (depth + length < minimal_depth && start < txt.length() / 2) {
            minimal_depth = depth + length;
            minimal_cut = start;
        }
    }

    for (auto it = node->children.begin(); it != node->children.end(); ++it)
        find_pos(it->second, depth + count_length(it->second), minimal_cut, minimal_depth);

}

std::string suffix_tree::do_work()
{
    int min_cut_position = -1;
    int min_depth = INT_MAX;

    find_pos(root, 0, min_cut_position, min_depth);

    return txt.substr(min_cut_position, txt.size() / 2);
}