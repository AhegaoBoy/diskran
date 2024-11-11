#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <utility>
#include <map>


class suffix_tree {
    struct node{
        friend class suffix_tree;
        int start;
        int *end;
        std::map<char, node*> children;
        node* suffix_link;
        node(int start, int *end) : start(start), end(end), suffix_link(nullptr) {}

        ~node() 
        {
            for (auto &it: children) 
                delete (it.second);
            
        }
    };

    std::string txt = "";

    node* root;
    node* last_added_node = nullptr;
    node* active_node = nullptr;

    int active_edge = -1;
    int active_length = 0;
    int suffixes_remained = 0;

    int global_end = -1;


public:
    explicit suffix_tree(std::string text);
    std::string do_work();
    ~suffix_tree() = default;

private:
    void Ukkonen_algo(const int &txt_pos);
    node* create_node(int start, int* end);
    int count_length(node* current_node);
};

typename suffix_tree::node *suffix_tree::create_node(int start, int *end)
{
    return new node(start, end);
}

int suffix_tree::count_length(suffix_tree::node *current_node)
{
    return *(current_node->end) - current_node->start + 1;
}

suffix_tree::suffix_tree(std::string text) : txt(std::move(text + text + "$"))
{
    root = create_node(-1, new int(-1));
    active_node = root;


    for (int i = 0; i < txt.size(); ++i)
        Ukkonen_algo(i);
}

void suffix_tree::Ukkonen_algo(const int &txt_pos)
{
    last_added_node = nullptr;
    suffixes_remained++;
    global_end++;
    while (suffixes_remained)
    {
        
        if (active_length == 0) 
            active_edge = txt_pos;
        

        if (!active_node->children[txt[active_edge]]) 
        {
            active_node->children[txt[active_edge]] = create_node(txt_pos, &global_end);
            
            if (last_added_node != nullptr) 
            {
                last_added_node->suffix_link = active_node;
                last_added_node = nullptr;
            }
        } 
        else 
        {
            node* jump = active_node->children[txt[active_edge]];
            int jumped_length = count_length(jump);

            if (active_length >= jumped_length)
            {
                active_edge += jumped_length;
                active_length -= jumped_length;
                active_node = jump;
                continue;
            }

            if (txt[jump->start + active_length] == txt[txt_pos])
            {

                if (last_added_node != nullptr) 
                {
                    last_added_node->suffix_link = active_node;
                    last_added_node = nullptr;
                }
                ++active_length;
                break;
            }

            node* to_add = create_node(jump->start, new int(jump->start + active_length - 1));
            active_node->children[txt[active_edge]] = to_add;
            jump->start += active_length;

            std::pair<char, node*> first = std::make_pair(txt[txt_pos],create_node(txt_pos, &global_end));

            std::pair<char, node*> second = std::make_pair(txt[jump->start], jump);
            to_add->children.insert(first);
            to_add->children.insert(second);
            
            if (last_added_node != nullptr) 
                last_added_node->suffix_link = to_add;
            
            last_added_node = to_add;
        }

        --suffixes_remained;

        if (active_node == root && active_length > 0) 
        {
            ++active_edge;
            --active_length;
        } 
        
        else if (active_node != root) 
            active_node = active_node->suffix_link;
        
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
    std::cin >> text;
    suffix_tree tree(text);
    std::cout << tree.do_work() << "\n";

    return 0;
}