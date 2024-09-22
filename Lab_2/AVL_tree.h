#ifndef LAB_2_AVL_TREE_H
#define LAB_2_AVL_TREE_H

#include<string>
template<typename tkey, typename tvalue>
class AVL_tree
{
    struct node
    {
        node* _left;
        node* _right;
        node* _parent;
        tkey _key;
        tvalue _value;

        size_t height;
        node(tkey key, tvalue value) : _key(key), _value(value), _right(nullptr), _left(nullptr), height(1){};

    };

public:
    AVL_tree();

    AVL_tree(AVL_tree const& other);
    AVL_tree(AVL_tree&& other);
    AVL_tree& operator=(AVL_tree<tkey, tvalue> const& other);
    AVL_tree& operator=(AVL_tree<tkey, tvalue>&& other);
    ~AVL_tree();

private:
    class balance_class
    {
        void balance(AVL_tree::node current_node);

        size_t count_height();

        void small_left_rotation(AVL_tree::node*& current_node);
        void small_right_rotation(AVL_tree::node*& current_node);
        void big_left_rotation(AVL_tree::node*& current_node);
        void big_right_rotation(AVL_tree::node*& current_node);


    };

    class dispose_class
    {
        enum dispose_status
        {
            OK,
            NoSuchWord
        };
        AVL_tree<tkey, tvalue>* _tree;

        dispose_class(AVL_tree<tkey, tvalue>* tree) : _tree(tree){};

        dispose_status dispose(tkey key_to_dispose);
    };

    class insert_class
    {
        enum insert_status
        {
            OK,
            Exist
        };

        AVL_tree<tkey, tvalue>* _tree;

        insert_class(AVL_tree<tkey, tvalue>* tree) : _tree(tree){};
        insert_status insert(tkey key, tvalue const& value);
        insert_status insert(tkey key, tvalue&& value);
    };

    class obtain_class
    {
        enum obtain_status
        {
            OK,
            NotFound
        };
        AVL_tree<tkey, tvalue>* _tree;

        obtain_class(AVL_tree<tkey, tvalue>* tree) : _tree(tree){};

        std::pair<obtain_status, tvalue> obtain(tkey key);
    };

private:
    friend class dispose_class;
    friend class obtain_class;
    friend class insert_class;


    dispose_class* disposeClass;
    insert_class* insertClass;
    obtain_class* obtainClass;
    node* root;
public:
    std::string insert(tkey key, tvalue const& value);
    std::string insert(tkey key, tvalue && value);
    std::string dispose(tkey key);
    std::string obtain(tkey key);

    std::string load_to_file(std::string const& filepath);
    std::string load_from_file(std::string const& filepath);

private:
    void clear_tree(AVL_tree::node* root);
    node* copy_tree(AVL_tree::node* root);


};
#endif
