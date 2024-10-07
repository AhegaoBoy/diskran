#include <iostream>
#include <cstdint>


template<class T>
class vector
{
private:
    size_t _size;
    size_t _capacity;
    T* _buffer;
public:

    vector(size_t n);
    vector();

    vector(vector const& other) noexcept;
    vector(vector&& other) noexcept;
    vector& operator=(vector const& other) noexcept;
    vector& operator=(vector&& other) noexcept;
    ~vector();

public:
    void push_back(T const& element);
    void push_back(T&& element);

    size_t size() const
    {
        return _size;
    }

    T& operator[](size_t const& i)
    {
        return _buffer[i];
    }

    T operator[](size_t const& i) const
    {
        return _buffer[i];
    }

    bool is_empty()
    {
        return _size == 0;
    };

};
template<typename T>
vector<T>::vector() : _size(0), _capacity(1)
{
    _buffer = new T[1];
}

template<typename T>
vector<T>::vector(size_t n) : _size(n), _capacity(n<<1)
{
    _buffer = new T[_capacity];
}

template<typename T>
vector<T>::vector(const vector<T> &other) noexcept: _size(other._size), _capacity(other._capacity)
{
    _buffer = new T [_capacity];

    for(size_t i = 0; i < _size; ++i)
        _buffer[i] = other._buffer[i];
}

template<typename T>
vector<T>::vector(vector<T> &&other) noexcept: _size(other._size), _capacity(other._capacity), _buffer(other._buffer)
{
    other._size = other._capacity = 0;
    other._buffer = nullptr;
}

template<typename T>
vector<T> &vector<T>::operator=(const vector<T> &other) noexcept
{
    if(this != &other)
    {
        _size = other._size;
        _capacity = other._capacity;

        delete[] _buffer;
        _buffer = new T[_capacity];

        for(size_t i = 0; i < _size; ++i)
            _buffer[i] = other._buffer[i];
    }
    return *this;
}

template<typename T>
vector<T> &vector<T>::operator=(vector<T> &&other) noexcept
{
    if(this != &other)
    {
        _size = other._size;
        _capacity = other._capacity;
        _buffer = other._buffer;

        other._buffer = nullptr;
        other._capacity = other._size = 0;
    }
    return *this;
}

template<typename T>
vector<T>::~vector()
{
    delete [] _buffer;
}

template<typename T>
void vector<T>::push_back(const T &element)
{
    if(_size == _capacity)
    {
        _capacity <<= 1;

        T* new_buffer = new T[_capacity];
        for(size_t i = 0; i < _size; ++i)
            new_buffer[i] = _buffer[i];
        delete[] _buffer;
        _buffer = new_buffer;
    }

    _buffer[_size++] = element;
}

template<typename T>
void vector<T>::push_back(T &&element)
{
    if(_size == _capacity)
    {
        _capacity <<= 1;
        T* new_buffer = new T[_capacity];
        for(size_t i = 0; i < _size; ++i)
            new_buffer[i] = _buffer[i];
        delete[] _buffer;
        _buffer = new_buffer;
    }

    _buffer[_size++] = std::move(element);
}

template<typename key, typename value>
struct pair
{
public:
    key first;
    value second;

    pair() : first(), second(){}
    pair(const key& k, const value& v) : first(k), second(v) {}
};

void count_sort(vector<pair<unsigned int, uint64_t>>& vec)
{
    if(vec.is_empty()) return;

    unsigned int _max = 0;
    for(size_t i = 0; i < vec.size(); ++i)
        _max = vec[i].first > _max ? vec[i].first : _max;

    int* tmp = new int[_max + 1]();

    for(size_t i = 0; i < vec.size(); ++i)
        ++tmp[vec[i].first];

    for(size_t i = 1; i < _max + 1; ++i)
        tmp[i] += tmp[i - 1];

    vector<pair<unsigned int, uint64_t>> result(vec.size());

    for(int i = vec.size() - 1; i >= 0; --i)
    {
        size_t pos = tmp[vec[i].first]--;
        result[pos - 1] = vec[i];
    }
    delete [] tmp;
    vec = std::move(result);
}

char* make_post_number(uint64_t number)
{
    char* result = new char[6];
    for(int i = 0; i < 6; ++i)
        result[i] = '0';

    int indx = 5;
    while(number >= 1)
    {
        result[indx--] = number % 10 + '0';
        number /= 10;
    }
    return result;
}

int main()
{
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    vector<pair<unsigned int, uint64_t>> vec;

    pair<unsigned int, uint64_t> pair;
    while(std::cin>>pair.first>>pair.second)
        vec.push_back(pair);

    count_sort(vec);
    for(size_t i = 0; i < vec.size(); ++i)
    {
        char* result = make_post_number(vec[i].first);
        printf("%s\t%llu\n", result, vec[i].second );
        delete[] result;
    }

    return 0;
}
