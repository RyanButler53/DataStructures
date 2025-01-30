#include <list>
#include <forward_list>
#include <vector>
#include "unrolled-linked-list/unrolled-linked-list.hpp"
#include "quack/quack.hpp"
#include <chrono>
#include <random>
#include <string>
#include <iostream>
#include <concepts>
#include <chrono>

template <typename T>
class QuackAdapter {
    Quack<T> q_;

public:
    using value_type = T;
    QuackAdapter() = default;
    ~QuackAdapter() = default;
    void push_back(const T &value) { q_.push(value); }
    void push_front(const T &value) { q_.push_front(value); }
    void pop_front() { q_.dequeue(); }
    void pop_back() { q_.pop(); }
    size_t size() { q_.size(); }
};

template <typename Container>
concept Queue = requires(Container &container, const typename Container::value_type &value) {
    container.push_back(value);
    container.push_front(value);
    container.pop_front();
    container.pop_back();
    container.size();
};

template<Queue Container>
void testPushing(size_t numOps){
    Container c;
    for (size_t i = 0; i < numOps; ++i) {
        c.push_back(i);
    }
    for (size_t i = 0; i < numOps; ++i) {
        c.push_front(i);
    }
    return;
}

template <Queue Container>
void testAlternatingPush(size_t numOps){
    Container c;
    for (size_t i = 0; i < numOps; ++i) {
        c.push_front(i);
        c.push_back(i);
    }
}

template <Queue Container>
void testAlternatingPushPop(size_t numOps){
    Container c;
    for (size_t i = 0; i < numOps; ++i) {
        c.push_front(i);
        c.push_back(i);
    }
    for (size_t i = 0; i < numOps; i++)
    {
        c.pop_back();
        c.pop_front();
    }
}

template <Queue Container>
void testRandomArguments(std::vector<int>& actions){
    Container c;
    for (int action : actions) {
    switch (action)
        {
        case 0:
            c.push_front(action);
            break;
        case 1:
            c.push_back(action);
            break;
        case 2:
            if (c.size() != 0){
                c.pop_front();
            }
            break;
        case 3:
            if (c.size() != 0){
                c.pop_back();
            }
            break;
        }
    }
    return;
}

template <typename F, typename... Args>
void measure(std::string data_structure, F f, Args&&... args){
    auto start = std::chrono::high_resolution_clock::now();
    f(std::forward<Args...>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << data_structure << ": " << (end - start).count() << std::endl;
}

int main() {

    size_t numActions = 10000000;

    measure("Linked List", testPushing<std::list<int>>, numActions);
    measure("Unrolled Linked List K = 8 ", testPushing<UnrolledLinkedList<int, 8>>, numActions);
    measure("Unrolled Linked List K = 64", testPushing<UnrolledLinkedList<int, 64>>, numActions);
    measure("Unrolled Linked List K = 256", testPushing<UnrolledLinkedList<int, 256>>, numActions);
    measure("Quack", testPushing<std::list<int>>, numActions);

    measure("Linked List", testAlternatingPush<std::list<int>>, numActions);
    measure("Unrolled Linked List K = 8 ", testAlternatingPush<UnrolledLinkedList<int, 8>>, numActions);
    measure("Unrolled Linked List K = 64", testAlternatingPush<UnrolledLinkedList<int, 64>>, numActions);
    measure("Unrolled Linked List K = 256", testAlternatingPush<UnrolledLinkedList<int, 256>>, numActions);
    measure("Quack", testAlternatingPush<std::list<int>>, numActions);

    measure("Linked List", testAlternatingPushPop<std::list<int>>, numActions);
    measure("Unrolled Linked List K = 8 ", testAlternatingPushPop<UnrolledLinkedList<int, 8>>, numActions);
    measure("Unrolled Linked List K = 64", testAlternatingPushPop<UnrolledLinkedList<int, 64>>, numActions);
    measure("Unrolled Linked List K = 256", testAlternatingPushPop<UnrolledLinkedList<int, 256>>, numActions);
    measure("Quack", testAlternatingPushPop<std::list<int>>, numActions);

    long seed = time(0);
    std::mt19937 rng(seed);
    std::discrete_distribution<int> dist({1, 1, 1, 1});
    std::vector<int> actions;
    for (size_t a = 0; a < numActions; ++a) {   
        actions.push_back(dist(rng));
    }

    measure("Linked List", testRandomArguments<std::list<int>>, actions);
    measure("Unrolled Linked List K = 8 ", testRandomArguments<UnrolledLinkedList<int, 8>>, actions);
    measure("Unrolled Linked List K = 64", testRandomArguments<UnrolledLinkedList<int, 64>>, actions);
    measure("Unrolled Linked List K = 256", testRandomArguments<UnrolledLinkedList<int, 256>>, actions);
    measure("Quack", testRandomArguments<std::list<int>>, actions);
}
