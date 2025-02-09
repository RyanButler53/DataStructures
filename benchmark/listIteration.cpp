#include <list>
#include <sstream>
#include <iostream>
#include <future>

#include "benchmark.hpp"
#include "unrolled-linked-list/unrolled-linked-list.hpp"


// Test random insertion. Iterate over a linked list and a unrolled one and see what happens.

// Should be able to have any integral type instead of just an int
template <typename IntContainer>
concept LinkedList = requires(IntContainer &container, 
                            int value, 
                            typename IntContainer::iterator it) {
    container.push_back(value);
    container.push_front(value);
    container.pop_front();
    container.pop_back();
    container.size();
    container.clear();
    it = container.begin();
    it = container.end();
    
};

template <LinkedList List>
class Benchmark_Iteration: public Benchmark{
    std::vector<int> randomActions_;

public:
    Benchmark_Iteration(std::string name, std::vector<int> &actions) : Benchmark(actions.size(), name), randomActions_{actions} {}

    void SetUp(List& l) const {
        for (size_t i = 0; i < numOps_ / 2; ++i){
            l.push_back(i);
        }
    }

    // full iteration
    void iterate(List& l) const{
        for (auto x : l){}
    }

    void randomInsertion(List& l) const {
        typename List::iterator it = l.begin();
        for (int action : randomActions_)
        {
            switch (action) {
            // Iterate forwards
            case 0:
                if (it != l.end()){
                    ++it;
                } else {
                    it = l.begin();
                }
                break;
            // iterate backwards
            case 1:
                if (it != l.begin()){
                    --it;
                } else {
                    it = l.end();
                }
                break;

            // Insert at iterator position
            case 2:
                l.insert(it, action);
                break;

            // Erase at iterator position
            case 3:
                if (it != l.end()){
                    it = l.erase(it);
                }
                break;
            default : break;
            }
        }
    }


    std::string operator()() const {

        // Set up 2 lists
        List l;
        SetUp(l);
        double setup = measure([*this, &l]()
                               { SetUp(l); });
        double iteration = measure([*this, &l]
                                   { iterate(l); });
        double insertion = measure([*this, &l]
                                   { randomInsertion(l); });

        std::stringstream ss;
        ss << "Results for " << name_ << ": " << setup << " " << iteration << " " << insertion;
        std::cout << ss.str() << std::endl;
        return ss.str();
    };
};

int main(int argc, char** argv) {
    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }
    std::vector<int> actions = Benchmark::getRandomActions(numOps, 4);
    Benchmark_Iteration<std::list<int>>("std::list", actions)();
    Benchmark_Iteration<UnrolledLinkedList<int, 8>>("Unrolled Linked List with K = 8", actions)();
    Benchmark_Iteration<UnrolledLinkedList<int, 64>>("Unrolled Linked List with K = 64", actions)();
    Benchmark_Iteration<UnrolledLinkedList<int, 256>>("Unrolled Linked List with K = 256", actions)();
}
