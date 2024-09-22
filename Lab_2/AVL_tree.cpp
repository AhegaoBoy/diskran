#include "AVL_tree.h"

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::balance(AVL_tree<tkey, tvalue>::node current_node)
{

}

template<typename tkey,typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::small_left_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{

}

template<typename tkey, typename tvalue>
size_t AVL_tree<tkey, tvalue>::balance_class::count_height()
{

}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::small_right_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{

}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::big_left_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{

}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::balance_class::big_right_rotation(AVL_tree<tkey, tvalue>::node *&current_node)
{

}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::insert_class::insert_status AVL_tree<tkey, tvalue>::insert_class::insert(tkey key, tvalue &&value)
{

}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::insert_class::insert_status AVL_tree<tkey, tvalue>::insert_class::insert(tkey key, const tvalue &value)
{

}

template<typename tkey, typename tvalue>
typename AVL_tree<tkey, tvalue>::dispose_class::dispose_status AVL_tree<tkey, tvalue>::dispose_class::dispose(tkey key_to_dispose)
{

}

template<typename tkey, typename tvalue>
std::pair<typename AVL_tree<tkey, tvalue>::obtain_class::obtain_status, tvalue> AVL_tree<tkey, tvalue>::obtain_class::obtain(tkey key)
{

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