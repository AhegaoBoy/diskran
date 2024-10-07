#include "AVL_tree.h"

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::balance(AVL_tree<tkey, tvalue>::node*& current_node)
{
    node* iter = current_node;
    while(iter->_parent)
    {
        int64_t cur_node_bf = balance_factor(current_node);
        if(std::abs(cur_node_bf) <= 1)
            iter = iter->_parent;

        else
        {
            if(cur_node_bf == 2)
            {
                int64_t left_subtree_bf = balance_factor(current_node->_left);

                if(left_subtree_bf > 0)
                {
                    small_left_rotation(current_node);
                }
                else
                {
                    big_right_rotation(current_node);
                }
            }
            else
            {
                int64_t right_subtree_bf = balance_factor(current_node->_right);
                if(right_subtree_bf > 0)
                {
                    small_right_rotation(current_node);
                }
                else
                {
                    big_right_rotation(current_node);
                }
            }
        }

    }
}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::count_height(AVL_tree<tkey, tvalue>::node*&  current_node)
{
    if(!current_node)
    {
        return;
    }
    if(!current_node->_left && !current_node->_right)
    {
        current_node->height = 1;
        return;
    }

    if(!current_node->_left && current_node->_right)
    {
        current_node->height = current_node->_right->height + 1;
        return ;
    }
    if(current_node->_left && !current_node->_right)
    {
        current_node->height = current_node->_right->height + 1;
        return ;
    }
    current_node->height = std::max(current_node->_right->height, current_node->_left->height) + 1;
}

template<typename tkey, typename tvalue>
int64_t AVL_tree<tkey, tvalue>::balance_class::balance_factor(const AVL_tree<tkey, tvalue>::node * current_node)
{
    if(!current_node || !current_node->_left && !current_node->_right)
        return 0;
    if(!current_node->_left && current_node->_right)
        return -current_node->_right->height;

    if(current_node->_left && !current_node->_right)
        return current_node->_left->height;

    return current_node->_left->height - current_node->_right->height;
}

template<typename tkey,typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::small_left_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{
    node* tmp = current_node->_left;

    tmp->_parent = current_node->_parent;
    tmp->_right = current_node;
    current_node->_parent = tmp;

    node* parent_node = tmp->_parent;
    if(parent_node->_left == current_node)
        parent_node->_left = tmp;
    else
        parent_node->_right = tmp;

    count_height(current_node);
    count_height(tmp);

}


template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::small_right_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{
    node* tmp = current_node->_right;

    tmp->_parent = current_node->_parent;
    tmp->_left = current_node;
    current_node->_parent = tmp;

    node* parent_node = tmp->_parent;
    if(parent_node->_left == current_node)
        parent_node->_left = tmp;
    else
        parent_node->_right = tmp;
    count_height(current_node);
    count_height(tmp);
}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::big_left_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{
    node* child = current_node->right_subtree;
    node* grand_child = child->left_subtree;

    node* grand_child_data = grand_child->right_subtree;

    current_node->right_subtree = grand_child;
    grand_child->right_subtree = child;

    child->left_subtree = grand_child_data;

    count_height(current_node);
    count_height(grand_child_data);

    small_left_rotation(current_node);
}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::big_right_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{
    node* child = current_node->left_subtree;
    node* grand_child = child->right_subtree;

    node* grand_child_data = grand_child->left_subtree;

    current_node->left_subtree = grand_child;
    grand_child->left_subtree = child;

    child->right_subtree = grand_child_data;

    count_height(current_node);
    count_height(grand_child_data);

    small_right_rotation(current_node);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::insert_class::insert_status AVL_tree<tkey, tvalue>::insert_class::insert(tkey key, tvalue &&value)
{

    node* iter = _tree->root;
    std::pair<typename AVL_tree<tkey, tvalue>::obtain_class::obtain_status, typename AVL_tree<tkey, tvalue>::node*&> result;
    while(iter)
    {
        if(key < iter->_key)
            iter = iter->_left;
        else if(key > iter->_key)
            iter = iter->_right;
        else
            return std::make_pair(AVL_tree<tkey, tvalue>::obtain_class::obtain_status::OK, iter);
    }

    return std::make_pair(AVL_tree<tkey, tvalue>::obtain_class::obtain_status::NotFound, iter);
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::insert_class::insert_status AVL_tree<tkey, tvalue>::insert_class::insert(tkey key, const tvalue &value)
{
    node* iter = _tree->root;
    node* parent = iter;

    AVL_tree<tkey, tvalue>::insert_class::insert_status result;
    while(iter)
    {
        if(key < iter->_key)
        {
            parent = iter;
            iter = iter->_left;
        }
        else if(key > iter->_key)
        {
            parent = iter;
            iter = iter->_right;
        }
        else
            return AVL_tree<tkey, tvalue>::insert_class::insert_status::Exist;
    }

    bool is_left_subtree = parent->_left == iter;
    node* new_node = new node(key, value);

    is_left_subtree ? parent->_left : parent->_right = new_node;
    new_node->_parent = parent;
    balance_class::balance(new_node);

}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::dispose_class::dispose_status AVL_tree<tkey, tvalue>::dispose_class::dispose(tkey key)
{

    node* iter = _tree->root;
//    std::pair<typename AVL_tree<tkey, tvalue>::obtain_class::obtain_status, typename AVL_tree<tkey, tvalue>::node*&> result;
    while(iter)
    {
        if(key < iter->_key)
            iter = iter->_left;
        else if(key > iter->_key)
            iter = iter->_right;
        else
            break;
    }

    if(!iter)
        return std::make_pair(AVL_tree<tkey, tvalue>::obtain_class::obtain_status::NotFound, iter);

    if(iter->_left)
    {

        node* find_the_least = iter->_left;


        while(find_the_least->_right)
            find_the_least = find_the_least->_right;

    }

    node* parent = iter->_parent;
    parent->_right = iter->_right;
    delete iter;
    balance_class::balance(iter);

}

template<typename tkey, typename tvalue>
std::pair<typename AVL_tree<tkey, tvalue>::obtain_class::obtain_status,
typename AVL_tree<tkey, tvalue>::node*&> AVL_tree<tkey, tvalue>::obtain_class::obtain(tkey key)
{
    node* iter = _tree->root;
    std::pair<typename AVL_tree<tkey, tvalue>::obtain_class::obtain_status, typename AVL_tree<tkey, tvalue>::node*&> result;
    while(iter)
    {
        if(key < iter->_key)
            iter = iter->_left;
        else if(key > iter->_key)
            iter = iter->_right;
        else
            return std::make_pair(AVL_tree<tkey, tvalue>::obtain_class::obtain_status::OK, iter);
    }

    return std::make_pair(AVL_tree<tkey, tvalue>::obtain_class::obtain_status::NotFound, iter);
}




template<typename tkey, typename tvalue>
std::string AVL_tree<tkey, tvalue>::insert(tkey key, tvalue &&value)
{
    typename AVL_tree<tkey, tvalue>::insert_class::insert_status status_code = insertClass->insert(key, std::move(value));
    return status_code == AVL_tree<tkey, tvalue>::insert_class::insert_status::OK ? "OK" : "Exist";
}



template<typename tkey, typename tvalue>
std::string AVL_tree<tkey, tvalue>::insert(tkey key, const tvalue &value)
{
    typename AVL_tree<tkey, tvalue>::insert_class::insert_status status_code = insertClass->insert(key, value);
    return status_code == AVL_tree<tkey, tvalue>::insert_class::insert_status::OK ? "OK" : "Exist";
}

template<typename tkey, typename tvalue>
std::string AVL_tree<tkey, tvalue>::dispose(tkey key)
{
    typename AVL_tree<tkey, tvalue>::dispose_class::dispose_status status_code = disposeClass->dispose(key);
    return status_code == AVL_tree<tkey, tvalue>::insert_class::insert_status::OK ? "OK" : "NOtInTree";
}



template<typename tkey, typename tvalue>
std::string AVL_tree<tkey, tvalue>::obtain(tkey key)
{
    typename AVL_tree<tkey, tvalue>::obtain_class::obtain_status status_code = obtainClass->obtain(key);
    return status_code == AVL_tree<tkey, tvalue>::insert_class::insert_status::OK ? "OK" : "NotFound";
}



template<typename tkey, typename tvalue>
std::string AVL_tree<tkey, tvalue>::load_from_file(const std::string &filepath)
{

}

template<typename tkey, typename tvalue>
std::string AVL_tree<tkey, tvalue>::load_to_file(const std::string &filepath)
{

}



template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::clear_tree(AVL_tree<tkey, tvalue>::node *root)
{
    if(!root)
        return;

    clear_tree(root->_left);
    clear_tree(root->_right);
    delete root;

    root = nullptr;
}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::node* AVL_tree<tkey, tvalue>::copy_tree(AVL_tree<tkey, tvalue>::node *root)
{
    if(!root)
        return nullptr;

    node* copied_subtree = new node();
    copied_subtree->_value = root->_value;
    copied_subtree->_key = root->key;

    copied_subtree->_left = copy_tree(root->_left);
    copied_subtree->_right = copy_tree(root->_right);

    return copied_subtree;
}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree() : obtainClass(new obtain_class(this)),
                                        disposeClass(new dispose_class(this)),
                                        insertClass(new insert_class(this)),
                                        root(nullptr)
{

}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(AVL_tree<tkey, tvalue> &&other) : insertClass(other.insertClass),
                                                                    disposeClass(other.disposeClass),
                                                                    obtainClass(other.obtainClass),
                                                                    root(other.root)
{
    other.obtainClass = nullptr;
    other.insertClass = nullptr;
    other.disposeClass = nullptr;
    other.root = nullptr;
}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(const AVL_tree<tkey, tvalue> &other)
{

    delete insertClass;
    delete obtainClass;
    delete disposeClass;
    clear_tree();

    insertClass = new insert_class(&other);
    obtainClass = new obtain_class(&other);
    disposeClass = new dispose_class(&other);
    root = copy_tree(other.root);

}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(AVL_tree<tkey, tvalue> &&other)
{
    if(this != &other)
    {
        insertClass = other.insertClass;
        disposeClass = other.disposeClass;
        obtainClass = other.obtainClass;
        root = other.root;

        other.insertClass = nullptr;
        other.disposeClass = nullptr;
        other.obtainClass = nullptr;
        other.root = nullptr;
    }
    return *this;
}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>& AVL_tree<tkey, tvalue>::operator=(const AVL_tree<tkey, tvalue> &other)
{
    if(this != &other)
    {
        delete insertClass;
        delete obtainClass;
        delete disposeClass;
        clear_tree();

        insertClass = new insert_class(&other);
        obtainClass = new obtain_class(&other);
        disposeClass = new dispose_class(&other);
        root = copy_tree(other.root);
    }
    return *this;
}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::~AVL_tree()
{
    clear_tree(root);
    delete obtainClass;
    delete insertClass;
    delete disposeClass;
}