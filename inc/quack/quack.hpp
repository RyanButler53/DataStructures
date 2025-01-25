
#ifndef QUACK_HPP_INCLUDED
#define QUACK_HPP_INCLUDED

#include <vector>
#include <ostream>

template <typename T>
class Quack
{
private:
    // Data
    std::vector<T> queue_;
    std::vector<T> stack_;
    size_t size_;
    
    // Functions
    void reshuffle(std::vector<T>& full, std::vector<T>& empty);

public:

    // Constructors
    Quack();
    ~Quack() = default;
    Quack(const Quack &other) = delete;
    Quack &operator=(Quack &other) = delete;

    // Public Methods
    void push(T val);
    void push_front(T val);
    void pop();
    void dequeue();
    bool empty();
    T front();
    T back();

    void printToStream(std::ostream &out) const;
};
template <typename T>
std::ostream& operator<<(std::ostream& os, const Quack<T>& q);

#include "quack-private.hpp"

#endif //QUACK_INLCUDED
