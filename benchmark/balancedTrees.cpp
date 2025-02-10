#include <vector>
#include "splay-tree/splay-tree.hpp"
#include <map>
#include <chrono>
#include <random>
#include <string>
#include <iostream>
#include <concepts>
#include "benchmark.hpp"


// template <typename Container>
// concept Tree = requires(Container &container, const typename Container::value_type &value) {
    
// };


template <typename Container>
class Benchmark_Tree : Benchmark {
  private:

      std::vector<int> randomActions_;

  public:
    Benchmark_Tree(std::string name, std::vector<int>& actions) : 
        Benchmark(actions.size(), name) {}


};


int main(int argc, char** argv) {

    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }

    // std::vector<int> actions = Benchmark::getRandomActions(numOps, 4);
    std::map<int, int> map;
    map[3] = 10;
    map[4] = 10;

    for (auto [k, v] : map)
    {
        std::cout << k << " " << v << std::endl;
    }

    SplayTree<int, int> st;

    for (int i = 0; i < 10; ++i){
        st.insert({i, i});
    }
    st.insert({0, -15});

    std::cout << st << std::endl;
    typename SplayTree<int, int>::const_iterator it = st.find(5);
    std::cout << it->second << std::endl;
    ++it;
    std::cout << it->second << std::endl;
}