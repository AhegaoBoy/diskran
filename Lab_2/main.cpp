#include <iostream>
#include <ostream>
#include <fstream>
#include <functional>
#include <cstring>
#include <cstdint>
#include <limits>

#define KEY_LENGTH 257

class AVL_tree {
    struct node {
        node* _left;
        node* _right;
        node* _parent;
        char* _key;
        uint64_t _value;
        int64_t height;

        node() : _key(nullptr), _value(0), _right(nullptr), _left(nullptr), _parent(nullptr), height(1) {};

        node(const char* key, uint64_t value) : _value(value)
        {
            _key = new char[strlen(key) + 1];
            strcpy(_key, key);
            _left = _right = _parent = nullptr;
            height = 1;
        }

        ~node() {
            delete[] _key;
        }
    };

public:
    AVL_tree();

    AVL_tree(AVL_tree const& other) noexcept;
    AVL_tree(AVL_tree&& other) noexcept;
    AVL_tree& operator=(AVL_tree const& other) noexcept;
    AVL_tree& operator=(AVL_tree&& other) noexcept;
    ~AVL_tree();

private:
    class dispose_class;
    class insert_class;

private:
    class balance_class{
        friend class AVL_tree::dispose_class;
        friend class AVL_tree::insert_class;
        friend class AVL_tree;

        AVL_tree* _tree;
        balance_class(AVL_tree* tree) : _tree(tree) {};
        balance_class(AVL_tree const* tree) : _tree(const_cast<AVL_tree*>(tree)) {};

        void balance(AVL_tree::node*& current_node);
        void count_height(AVL_tree::node*& current_node);
        int64_t balance_factor(AVL_tree::node const* current_node);
        void small_left_rotation(AVL_tree::node*& current_node);
        void small_right_rotation(AVL_tree::node*& current_node);
        void big_left_rotation(AVL_tree::node*& current_node);
        void big_right_rotation(AVL_tree::node*& current_node);
    };

    class dispose_class{
        friend class AVL_tree;
        enum dispose_status { OK, NoSuchWord };
        AVL_tree* _tree;

        dispose_class(AVL_tree* tree) : _tree(tree) {};
        dispose_class(AVL_tree const* tree) : _tree(const_cast<AVL_tree*>(tree)) {};

        dispose_status dispose(const char* key_to_dispose);
    };

    class insert_class{
        friend class AVL_tree;
        enum insert_status { OK, Exist };

        AVL_tree* _tree;

        insert_class(AVL_tree* tree) : _tree(tree) {};
        insert_class(AVL_tree const* tree) : _tree(const_cast<AVL_tree*>(tree)) {};

        insert_status insert(const char* key, uint64_t const& value);
        insert_status insert(const char* key, uint64_t&& value);
    };

    class obtain_class
    {
        friend class AVL_tree;
        enum obtain_status { OK, NotFound };
        AVL_tree* _tree;

        obtain_class(AVL_tree* tree) : _tree(tree) {};
        obtain_class(AVL_tree const* tree) : _tree(const_cast<AVL_tree*>(tree)) {};

        std::pair<obtain_status, uint64_t> obtain(const char* key);
    };

private:
    dispose_class* disposeClass;
    insert_class* insertClass;
    obtain_class* obtainClass;
    balance_class* balanceClass;
    node* root;

public:
    std::string insert(const char* key, uint64_t const& value);
    std::string insert(const char* key, uint64_t && value);
    std::string dispose(const char* key);
    std::pair<std::string, uint64_t> obtain(const char* key);


    std::string load_to_file(char* filepath);
    std::string load_from_file(char* filepath);

private:
    void clear_tree(node* root);
    node* copy_tree(node* root);
};

void AVL_tree::balance_class::balance(node*& current_node) {
    node* iter = current_node;
    while (iter)
    {
        count_height(iter);
        int bf = balance_factor(iter);
        if(bf == 2)
        {
            if(balance_factor(iter->_left) < 0)
                small_left_rotation(iter->_left);
            small_right_rotation(iter);
        }
        if(bf == -2)
        {
            if(balance_factor(iter->_right) > 0)
                small_right_rotation(iter->_right);
            small_left_rotation(iter);
        }
        iter = iter->_parent;
    }
}

void AVL_tree::balance_class::count_height(node*& current_node) {
    if (!current_node) {
        return;
    }
    if (!current_node->_left && !current_node->_right) {
        current_node->height = 1;
        return;
    }

    if (!current_node->_left && current_node->_right) {
        current_node->height = current_node->_right->height + 1;
        return;
    }
    if (current_node->_left && !current_node->_right) {
        current_node->height = current_node->_left->height + 1;
        return;
    }
    current_node->height = std::max(current_node->_right->height, current_node->_left->height) + 1;
}

int64_t AVL_tree::balance_class::balance_factor(const node* current_node) {
    if (!current_node || (!current_node->_left && !current_node->_right))
        return 0;

    if (!current_node->_left && current_node->_right)
        return -current_node->_right->height;

    if (current_node->_left && !current_node->_right)
        return current_node->_left->height;

    return current_node->_left->height - current_node->_right->height;
}

void AVL_tree::balance_class::small_left_rotation(node*& current_node) {
    if(!current_node || !current_node->_right) return;
    node* tmp = current_node->_right;
    tmp->_parent = current_node->_parent;
    current_node->_right = tmp->_left;
    if (tmp->_left)
        tmp->_left->_parent = current_node;
    tmp->_left = current_node;
    current_node->_parent = tmp;

    if (!tmp->_parent)
        _tree->root = tmp;
    else {
        if (tmp->_parent->_left == current_node)
            tmp->_parent->_left = tmp;
        else
            tmp->_parent->_right = tmp;
    }

    current_node = tmp;

    count_height(current_node->_left);
    count_height(current_node);
}

void AVL_tree::balance_class::small_right_rotation(node*& current_node) {
    if(!current_node || !current_node->_left) return;
    node* tmp = current_node->_left;
    tmp->_parent = current_node->_parent;
    current_node->_left = tmp->_right;
    if (tmp->_right)
        tmp->_right->_parent = current_node;
    tmp->_right = current_node;
    current_node->_parent = tmp;

    if (!tmp->_parent)
        _tree->root = tmp;
    else {
        if (tmp->_parent->_right == current_node)
            tmp->_parent->_right = tmp;
        else
            tmp->_parent->_left = tmp;
    }

    current_node = tmp;
    count_height(current_node->_right);
    count_height(current_node);
}

void AVL_tree::balance_class::big_left_rotation(node*& current_node) {
    if(!current_node) return;
    small_right_rotation(current_node->_right);
    small_left_rotation(current_node);
}

void AVL_tree::balance_class::big_right_rotation(node*& current_node) {
    if(!current_node) return;
    small_left_rotation(current_node->_left);
    small_right_rotation(current_node);
}

typename AVL_tree::insert_class::insert_status AVL_tree::insert_class::insert(const char* key, uint64_t&& value) {
    if (_tree->root == nullptr) {
        _tree->root = new node(key, value);
        return insert_status::OK;
    }

    node* iter = _tree->root;
    node* parent = iter;
    bool is_left = true;

    while (iter) {
        if (strcmp(key, iter->_key) < 0) {
            is_left = true;
            parent = iter;
            iter = iter->_left;
        } else if (strcmp(key, iter->_key) > 0) {
            is_left = false;
            parent = iter;
            iter = iter->_right;
        } else
            return insert_status::Exist;
    }
    node* new_node = new node(key, std::move(value));
    if (is_left)
        parent->_left = new_node;
    else
        parent->_right = new_node;

    new_node->_parent = parent;
    _tree->balanceClass->balance(new_node);

    return insert_status::OK;
}

typename AVL_tree::insert_class::insert_status AVL_tree::insert_class::insert(const char* key, const uint64_t& value) {
    if (_tree->root == nullptr) {
        _tree->root = new node(key, value);
        return insert_status::OK;
    }

    node* iter = _tree->root;
    node* parent = iter;
    bool is_left_subtree = true;


    while (iter) {
        if (strcmp(key, iter->_key) < 0)
        {
            parent = iter;
            iter = iter->_left;
            is_left_subtree = true;
        } else if (strcmp(key, iter->_key) > 0)
        {
            parent = iter;
            iter = iter->_right;
            is_left_subtree = false;
        } else
            return insert_status::Exist;
    }

    node* new_node = new node(key, value);

    if (is_left_subtree)
        parent->_left = new_node;
    else
        parent->_right = new_node;

    new_node->_parent = parent;
    _tree->balanceClass->balance(new_node);

    return insert_status::OK;
}

typename AVL_tree::dispose_class::dispose_status AVL_tree::dispose_class::dispose(const char* key) {
    node* iter = _tree->root;
    while (iter) {
        if (strcmp(key, iter->_key) < 0) {
            iter = iter->_left;
        } else if (strcmp(key, iter->_key) > 0) {
            iter = iter->_right;
        } else
            break;
    }
    if (iter == nullptr)
        return dispose_status::NoSuchWord;

    node* node_to_dispose = iter;
    node* parent = node_to_dispose->_parent;

    if (iter->_left && iter->_right) {
        node* target_to_swap = iter->_left;
        while (target_to_swap->_right)
            target_to_swap = target_to_swap->_right;

        std::swap(iter->_value, target_to_swap->_value);
        std::swap(iter->_key, target_to_swap->_key);
        node_to_dispose = target_to_swap;
        parent = node_to_dispose->_parent;
    }

    node* child = (node_to_dispose->_left != nullptr) ? node_to_dispose->_left : node_to_dispose->_right;
    if (child)
        child->_parent = node_to_dispose->_parent;

    if (node_to_dispose->_parent == nullptr)
        _tree->root = child;
    else {
        if (node_to_dispose == node_to_dispose->_parent->_left)
            node_to_dispose->_parent->_left = child;
        else
            node_to_dispose->_parent->_right = child;
    }

    delete node_to_dispose;
    _tree->balanceClass->balance(parent == nullptr ? _tree->root : parent);
    return dispose_status::OK;
}

std::pair<typename AVL_tree::obtain_class::obtain_status, uint64_t> AVL_tree::obtain_class::obtain(const char* key) {
    node* iter = _tree->root;
    while (iter) {
        if (strcmp(key, iter->_key) < 0)
            iter = iter->_left;
        else if (strcmp(key, iter->_key) > 0)
            iter = iter->_right;
        else {
            return std::make_pair(obtain_status::OK, iter->_value);
        }
    }

    return std::make_pair(obtain_status::NotFound, static_cast<uint64_t>(-1));
}

std::string AVL_tree::insert(const char* key, uint64_t&& value) {
    insert_class::insert_status status_code = insertClass->insert(key, std::move(value));
    return status_code == insert_class::insert_status::OK ? "OK\n" : "Exist\n";
}

std::string AVL_tree::insert(const char* key, const uint64_t& value) {
    insert_class::insert_status status_code = insertClass->insert(key, value);
    return status_code == insert_class::insert_status::OK ? "OK\n" : "Exist\n";
}

std::string AVL_tree::dispose(const char* key) {
    dispose_class::dispose_status status_code = disposeClass->dispose(key);
    return status_code == dispose_class::dispose_status::OK ? "OK\n" : "NoSuchWord\n";
}

std::pair<std::string, uint64_t> AVL_tree::obtain(const char* key){
    std::pair<obtain_class::obtain_status, uint64_t> status_code = obtainClass->obtain(key);
    return status_code.first == obtain_class::obtain_status::OK ?
           std::make_pair("OK", status_code.second) :
           std::make_pair("NoSuchWord", status_code.second);
}

std::string AVL_tree::load_to_file(char* filepath) {
    std::ofstream outfile(filepath);
    if (!outfile.is_open()) {
        return "Failed to open file";
    }

    std::function<void(const node*)> write_node = [&](const node* n) {
        if (n) {
            outfile << n->_key << " " << n->_value << "\n";
            if (!outfile.good()) {
                outfile.close();
                return;
            }
            write_node(n->_left);
            write_node(n->_right);
        }
    };

    write_node(root);
    outfile.close();
    return "OK\n";
}

std::string AVL_tree::load_from_file(char* filepath) {
    std::ifstream infile(filepath);
    if (!infile.is_open()) {
        return "Failed to open file";
    }

    clear_tree(root);
    root = nullptr;

    std::string key;
    uint64_t value;
    while (infile >> key >> value) {
        insert(key.c_str(), value);
        if (!infile.good()) {
            infile.close();
            return "File read error";
        }
    }

    infile.close();
    return "OK\n";
}

void AVL_tree::clear_tree(node* root) {
    if (root) {
        clear_tree(root->_left);
        clear_tree(root->_right);
        delete root;
        root = nullptr;
    }
}

typename AVL_tree::node* AVL_tree::copy_tree(node* root) {
    if (!root) return nullptr;
    node* new_root = new node(root->_key, root->_value);
    new_root->_left = copy_tree(root->_left);
    if (new_root->_left) new_root->_left->_parent = new_root;
    new_root->_right = copy_tree(root->_right);
    if (new_root->_right) new_root->_right->_parent = new_root;
    return new_root;
}

AVL_tree::AVL_tree()
        : obtainClass(new obtain_class(this)),
          disposeClass(new dispose_class(this)),
          insertClass(new insert_class(this)),
          balanceClass(new balance_class(this)),
          root(nullptr) {}

AVL_tree::AVL_tree(AVL_tree&& other) noexcept
        : insertClass(other.insertClass),
          disposeClass(other.disposeClass),
          obtainClass(other.obtainClass),
          balanceClass(other.balanceClass),
          root(other.root) {
    other.obtainClass = nullptr;
    other.insertClass = nullptr;
    other.disposeClass = nullptr;
    other.balanceClass = nullptr;
    other.root = nullptr;
}

AVL_tree::AVL_tree(const AVL_tree& other) noexcept
        : obtainClass(new obtain_class(&other)),
          disposeClass(new dispose_class(&other)),
          insertClass(new insert_class(&other)),
          balanceClass(new balance_class(&other)),
          root(copy_tree(other.root)) {}

AVL_tree& AVL_tree::operator=(AVL_tree&& other) noexcept
{
    if (this != &other) {
        clear_tree(root);
        delete obtainClass;
        delete insertClass;
        delete disposeClass;
        delete balanceClass;

        obtainClass = other.obtainClass;
        insertClass = other.insertClass;
        disposeClass = other.disposeClass;
        balanceClass = other.balanceClass;
        root = other.root;

        other.obtainClass = nullptr;
        other.insertClass = nullptr;
        other.disposeClass = nullptr;
        other.balanceClass = nullptr;
        other.root = nullptr;
    }
    return *this;
}

AVL_tree& AVL_tree::operator=(const AVL_tree& other) noexcept
{
    if (this != &other) {
        clear_tree(root);
        delete obtainClass;
        delete insertClass;
        delete disposeClass;
        delete balanceClass;

        obtainClass = new obtain_class(&other);
        disposeClass = new dispose_class(&other);
        insertClass = new insert_class(&other);
        balanceClass = new balance_class(&other);
        root = copy_tree(other.root);
    }
    return *this;
}

AVL_tree::~AVL_tree() {
    clear_tree(root);
    delete obtainClass;
    delete insertClass;
    delete disposeClass;
    delete balanceClass;
}

void to_lower(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        str[i] = tolower(str[i]);
    }
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    AVL_tree avl;
    char buffer[KEY_LENGTH] = "";

    while (std::cin >> buffer) {
        switch (buffer[0]) {
            case '+': {
                char key[KEY_LENGTH] = "";
                uint64_t value = 0;
                if (!(std::cin >> key >> value)) {
                    std::cerr << "Invalid input\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                to_lower(key);
                std::cout << avl.insert(key, value);
                break;
            }
            case '-': {
                if (!(std::cin >> buffer)) {
                    std::cerr << "Invalid input\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                to_lower(buffer);
                std::cout << avl.dispose(buffer);
                break;
            }
            case '!': {
                char path[50];
                if (!(std::cin >> buffer >> path)) {
                    std::cerr << "Invalid input\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                if (!strcmp(buffer, "Save"))
                    std::cout << avl.load_to_file(path);
                else
                    std::cout << avl.load_from_file(path);
                break;
            }
            default: {
                to_lower(buffer);
                std::pair<std::string, uint64_t> res = avl.obtain(buffer);
                if (res.first == "NoSuchWord")
                    std::cout << "NoSuchWord\n";
                else
                    std::cout << res.first << ": " << res.second << "\n";
                break;
            }
        }
    }
    return 0;
}