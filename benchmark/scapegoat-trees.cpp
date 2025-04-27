// Investigation: What is the best alpha value for a Scapegoat Tree? 

// Run the exact same tests, but on varying scapegoat trees

#include <vector>
#include <thread>
#include <sstream>

#include "treelib.hpp"
#include "benchmark.hpp"

// Scapegoat Trees
#include "tree/scapegoat.hpp"

void insert(size_t numOps, double alpha){
    ScapegoatTree<int, int> tree(alpha);
    for (int i = 0; i < numOps; ++i)
    {
        tree.insert({i, i});
    }
}

void insertElements(std::vector<int>& elements, double alpha){
    ScapegoatTree<int, int> tree(alpha);
    for (int x : elements){
        tree.insert({x, x});
    }
}

void insertAndClear(std::vector<int>& elements, double alpha){
    ScapegoatTree<int, int> tree(alpha);
    TreeLib::insertItems(tree, elements);
    tree.clear();
}

void insertAndDelete(std::vector<int>& toInsert,std::vector<int>& toDelete, double alpha){
    ScapegoatTree<int, int> tree(alpha);
    TreeLib::insertItems(tree, toInsert);
    for (int x : toDelete){
        tree.erase(x);
    }
}

void findDuplicates(std::vector<int>& toInsert, std::vector<int> toFind, double alpha){
    ScapegoatTree<int, int> tree(alpha);
    TreeLib::insertItems(tree, toInsert);
    for (int x : toFind){
        tree.find(x);
    }
}

std::vector<int> treeSort(std::vector<int>& elements, double alpha){
    ScapegoatTree<int, int> tree(alpha);
    for (int x : elements){
        tree.insert({x, x});
    }
    std::vector<int> sorted;
    sorted.reserve(elements.size());
    for (int x : elements){
        sorted.push_back(x);
    }
    return sorted;
}

int main(int argc, char** argv){
    constexpr std::array<double, 6> ALPHAS{0.51, 0.6, 2.0/3, 0.75, 5.0/6, 0.9};

    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }
    std::vector<int> shuffled, duplicates;
    
    // Setup phase: 
    TreeLib::Setup s(numOps);

    std::thread shuffledThread([&s, &shuffled]()
                               { s.getShuffled(shuffled); });
    std::thread duplicatesThread([&s, &duplicates]()
                               { s.getDuplicates(duplicates); });

    shuffledThread.join();
    duplicatesThread.join();

    BenchmarkSuite suite("Scapegoat Trees");
    suite.setConfig(numOps, 10);

    // Must do all trials in order, cannot do all tests at once in case the CPU gets slowed down

    for (const double& a : ALPHAS){
        std::stringstream ss;
        ss << "In order insertion a = " << a;
        suite.addConfiguredTest(ss.str(), insert, std::ref(numOps), std::ref(a));
    }
    suite.run();

    for (double a : ALPHAS){
        std::stringstream ss;
        ss << "In Random Insertion a = " << a;
        suite.addConfiguredTest(ss.str(), insertElements, std::ref(shuffled), std::ref(a));
    }
    suite.run();

    for (double a : ALPHAS){
        std::stringstream ss;
        ss << "Deletion a = " << a;
        suite.addConfiguredTest(ss.str(), insertAndDelete, std::ref(shuffled), std::ref(shuffled), std::ref(a));
    }
    suite.run();

    for (double a : ALPHAS){
        std::stringstream ss;
        ss << "Finding Duplicates a = " << a;
        suite.addConfiguredTest(ss.str(), findDuplicates, std::ref(shuffled), std::ref(duplicates), std::ref(a));
    }
    suite.run();

    for (double a : ALPHAS){
        std::stringstream ss;
        ss << "Sorting a = " << a;
        suite.addConfiguredTest(ss.str(), treeSort, std::ref(shuffled), std::ref(a));
    }
    suite.run();

    suite.resultsToCSV("scapegoat.csv");

    return 0;
}