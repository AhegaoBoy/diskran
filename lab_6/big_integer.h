#ifndef LAB_6_BIG_INTEGER_H
#define LAB_6_BIG_INTEGER_H

#include <corecrt.h>
#include <string>
#include <vector>
#include <iostream>

class big_integer
{

private:

    void clear();

    void copy_from(
            big_integer const &other);

    void initialize_from(
            int const *digits,
            size_t digits_count);

    void initialize_from(
            std::vector<int> const &digits,
            size_t digits_count);

    void initialize_from(
            std::vector<unsigned int> const &digits,
            size_t digits_count);

    void initialize_from(
            std::string const &value,
            size_t base);

private:

    static void print_byte(
            std::ostream &stream,
            unsigned char byte_value);

    static void dump_int_value(
            std::ostream &stream,
            int value);

private:

    int _oldest_digit;
    unsigned int *_other_digits;




private:

    big_integer &change_sign();

    std::vector<unsigned int> convert_to_base(std::string const & biiiiiiiiiiig_number, size_t base);

public:

    inline int get_digits_count() const noexcept;

    inline int sign() const noexcept;

    inline bool is_equal_to_zero() const noexcept;

    inline unsigned int get_digit(
            int position) const noexcept;

public:

    big_integer(
            std::string const &digits);

    big_integer(
            int const *digits,
            size_t digits_count);

    big_integer(
            std::vector<int> const &digits);

    big_integer(
            std::string const &value,
            size_t base);

public:

    big_integer(const big_integer &other);

    big_integer &operator=(
            big_integer const &other);

    ~big_integer();

public:

    void remove_leading_zeros();


public:

    static big_integer euclidean_algorithm(
            big_integer first_number,
            big_integer second_number);

    big_integer &operator+=(
            big_integer const &other);

    big_integer operator+(
            big_integer const &other) const;

    big_integer &operator-=(
            big_integer const &other);

    big_integer operator-(
            big_integer const &other) const;

    big_integer operator-() const;

    big_integer &operator*=(
            big_integer const &other);

    big_integer operator*(
            big_integer const &other) const;

    big_integer &operator/=(
            big_integer const &other);

    big_integer operator/(
            big_integer const &other) const;

    big_integer &operator%=(
            big_integer const &other);

    big_integer operator%(
            big_integer const &other) const;

    bool operator==(
            big_integer const &other) const;

    bool operator!=(
            big_integer const &other) const;

    bool operator<(
            big_integer const &other) const;

    bool operator<=(
            big_integer const &other) const;

    bool operator>(
            big_integer const &other) const;

    bool operator>=(
            big_integer const &other) const;

    big_integer operator~() const;

public:

    big_integer &operator&=(
            big_integer const &other);

    big_integer operator&(
            big_integer const &other) const;

    big_integer &operator|=(
            big_integer const &other);

    big_integer operator|(
            big_integer const &other) const;

    big_integer &operator^=(
            big_integer const &other);

    big_integer operator^(
            big_integer const &other) const;

    big_integer &operator<<=(
            size_t shift_value);

    big_integer operator<<(
            size_t shift_value) const;

    big_integer &operator>>=(
            size_t shift_value);

    big_integer operator>>(
            size_t shift_value) const;

public:

    friend std::ostream &operator<<(
            std::ostream &stream,
            big_integer const &value);

    friend std::istream &operator>>(
            std::istream &stream,
            big_integer &value);

};

#endif //LAB_6_BIG_INTEGER_H
