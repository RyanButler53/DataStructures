#include <vector>
#include <map>
#include <random>
#include <string>
#include <iostream>

// Libraries
#include "benchmark.hpp"
#include "treelib.hpp"

// Trees
#include "tree/splay-tree.hpp"
#include "tree/scapegoat.hpp"
#include "interfaces.hpp"

int main(int argc, char** argv) {

    size_t numOps = 10000;
    if (argc == 2){
        numOps = atoi(argv[1]);
    }
    std::vector<int> balanced, shuffled, duplicates;
    
    // Setup phase: 
    TreeLib::Setup s(numOps);
    std::thread balancedThread([&s, &balanced]()
                               { s.getBalanced(balanced); });
    std::thread shuffledThread([&s, &shuffled]()
                               { s.getShuffled(shuffled); });
    std::thread duplicatesThread([&s, &duplicates]()
                               { s.getDuplicates(duplicates); });

    balancedThread.join();
    shuffledThread.join();
    duplicatesThread.join();

    // Make a benchmark suite:
    BenchmarkSuite suite("Balanced Trees Suite");
    suite.setConfig(numOps, 10);

    suite.addConfiguredTest("In order insertion RB Tree", TreeLib::insert<std::map<int, int>>, std::ref(numOps));
    suite.addConfiguredTest("In order insertion Splay Tree", TreeLib::insert<SplayTree<int, int>>, std::ref(numOps));
    suite.addConfiguredTest("In order insertion Scapegoat Tree", TreeLib::insert<ScapegoatTree<int, int>>, std::ref(numOps));
    suite.run();

    suite.addConfiguredTest("Balanced insertion RB Tree", TreeLib::insertElements<std::map<int, int>>, std::ref(balanced));
    suite.addConfiguredTest("Balanced insertion Splay Tree", TreeLib::insertElements<SplayTree<int, int>>, std::ref(balanced));
    suite.addConfiguredTest("Balanced insertion Scapegoat Tree", TreeLib::insertElements<ScapegoatTree<int, int>>, std::ref(balanced));
    suite.run();

    suite.addConfiguredTest("Random insertion RB Tree", TreeLib::insertElements<std::map<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Random insertion Splay Tree", TreeLib::insertElements<SplayTree<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Random insertion Scapegoat Tree", TreeLib::insertElements<ScapegoatTree<int, int>>, std::ref(shuffled));
    suite.run();

    suite.addConfiguredTest("Clear RB Tree", TreeLib::insertAndClear<std::map<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Clear Splay Tree", TreeLib::insertAndClear<SplayTree<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Clear Scapegoat Tree", TreeLib::insertAndClear<ScapegoatTree<int, int>>, std::ref(shuffled));
    suite.run();

    suite.addConfiguredTest("Delete from RB Tree", TreeLib::insertAndDelete<std::map<int, int>>, std::ref(shuffled), std::ref(shuffled));
    suite.addConfiguredTest("Delete from Splay Tree", TreeLib::insertAndDelete<SplayTree<int, int>>, std::ref(shuffled), std::ref(shuffled));
    suite.addConfiguredTest("Delete from Scapegoat Tree", TreeLib::insertAndDelete<ScapegoatTree<int, int>>, std::ref(shuffled), std::ref(shuffled));
    suite.run();

    suite.addConfiguredTest("Find Duplicates RB Tree", TreeLib::findDuplicates<std::map<int, int>>, std::ref(shuffled), std::ref(duplicates));
    suite.addConfiguredTest("Find Duplicates Splay Tree", TreeLib::findDuplicates<SplayTree<int, int>>, std::ref(shuffled), std::ref(duplicates));
    suite.addConfiguredTest("Find Duplicates Scapegoat Tree", TreeLib::findDuplicates<ScapegoatTree<int, int>>, std::ref(shuffled), std::ref(duplicates));
    suite.run();

    suite.addConfiguredTest("RB Tree Sort", TreeLib::treeSort<std::map<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Splay Tree Sort", TreeLib::treeSort<SplayTree<int, int>>, std::ref(shuffled));
    suite.addConfiguredTest("Scapegoat Tree Sort", TreeLib::treeSort<ScapegoatTree<int, int>>, std::ref(shuffled));
    
    suite.run();

    suite.resultsToCSV("balancedTrees.csv");
}