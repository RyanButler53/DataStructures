// Defines a FAMILY of hash functions
// Hash function
#include <cstdint> 

// Some arbitrarily chosen big prime (51 bits)
constexpr uint64_t PRIME = 2734440556118881;

template <typename T>
class Hash
{
private:
    uint64_t a_, b_;

public:
    Hash():a_{1},b_{1}{}
    Hash(uint64_t a, uint64_t b):a_{a}, b_{b}{}
    ~Hash() = default;

    uint64_t operator()(T x)const {
        std::hash<T> firstHash;
        return (a_ * firstHash(x) + b_)%PRIME;
    }

};
