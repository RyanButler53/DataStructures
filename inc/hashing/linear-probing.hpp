#ifndef LINEAR_PROBING_HPP_DEFINED
#define LINEAR_PROBING_HPP_DEFINED

#include "hash.hpp"
#include <vector>
#include <cstring>
#include <ranges>

template <typename T>
class LinearProbing {

    std::vector<char> available_;
    std::vector<T> items_;
    Hash<T> hash_;
    size_t size_ = 0;
    size_t numBuckets_ = 1;
    double maxLoad_;

    void rehash(size_t newNumBuckets);

    public:

    using key_type = T;

    LinearProbing();
    LinearProbing(double maxLoad);
    ~LinearProbing() = default;

    void empty() const {return size_ == 0;} ;
    void size() const  {return size_;};
    void clear();
    void insert(T key);
    bool contains(T key ) const ;
    void remove(T key);
};

#include "linear-probing-private.hpp"

#endif // LINEAR_PROBING_HPP_DEFINED
