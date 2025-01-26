#include "quack/quack.hpp"
#include <queue>
#include <list>
#include <iostream>
#include <numeric>
#include <chrono>

using namespace std;


// Results: Time for all 4 experiments

template <typename T>
double average(const std::vector<T> &a)
{
    double sum = std::accumulate(begin(a), end(a), 0.0);
    return sum / a.size();
}

template <typename T>
double stdDev(const std::vector<T> &a, double avg)
{
    double sumSquares = 0;

    for (T m : a) {
        double d = m - avg;
        sumSquares += d * d;
    }
    double s = sqrt(sumSquares / (a.size() - 1));
    return s;
}

void printExperiment(size_t expNumber,vector<double>& qvec, vector<double>& lvec){
    double qavg = average(qvec);
    double lavg = average(lvec);
    cout << "Experiment " << expNumber << ": \t Avg \t Stdev" << endl;
    cout << "Quack \t " << qavg << "\t" << stdDev(qvec, qavg) << endl;
    cout << "List \t " << lavg << "\t" << stdDev(lvec, lavg) << "\n " << endl;
    qvec.clear();
    lvec.clear();
}

void exp1(vector<double>& quackVec, vector<double>& listVec, size_t n){
    Quack<int> q;
    list<int> l;
    auto quackStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        q.push(i);
    }
    auto quackEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> quackTime = quackEnd - quackStart;
    quackVec.push_back(quackTime.count());

    auto listStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        l.push_back(i);
    }
    auto listEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> listTime = listEnd - listStart;
    listVec.push_back(listTime.count());
}

void exp2(vector<double>& quackVec, vector<double>& listVec, size_t n){
    Quack<int> q;
    list<int> l;
    auto quackStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        q.push(i);
    }
    for (size_t i = 0; i < n; ++i){
        q.pop();
    }
    auto quackEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> quackTime = quackEnd - quackStart;
    quackVec.push_back(quackTime.count());

    auto listStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        l.push_back(i);
    }
    for (size_t i = 0; i < n; ++i){
        l.pop_back();
    }
    auto listEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> listTime = listEnd - listStart;
    listVec.push_back(listTime.count());
}

void exp3(vector<double>& quackVec, vector<double>& listVec, size_t n){
    Quack<int> q;
    list<int> l;
    auto quackStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        q.push(i);
    }
    for (size_t i = 0; i < n; ++i){
        q.dequeue();
    }
    auto quackEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> quackTime = quackEnd - quackStart;
    quackVec.push_back(quackTime.count());

    auto listStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        l.push_back(i);
    }
    for (size_t i = 0; i < n; ++i){
        l.pop_front();
    }
    auto listEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> listTime = listEnd - listStart;
    listVec.push_back(listTime.count());
}

void exp4(vector<double>& quackVec, vector<double>& listVec, size_t n){
    Quack<int> q;
    list<int> l;
    auto quackStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        q.push(i);
    }
    for (size_t i = 0; i < n / 2; ++i)
    {
        q.pop();
        q.dequeue();
    }
    auto quackEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> quackTime = quackEnd - quackStart;
    quackVec.push_back(quackTime.count());

    auto listStart = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < n; ++i){
        l.push_back(i);
    }
    for (size_t i = 0; i < n / 2; ++i)
    {
        l.pop_back();
        l.pop_front();
    }
    auto listEnd = chrono::high_resolution_clock::now();
    chrono::duration<double> listTime = listEnd - listStart;
    listVec.push_back(listTime.count());
}

int main(int argc, char **argv) {

    // Process Args
    size_t n = (argc > 1) ? strtoul(argv[1], nullptr, 10) : 100000UL;
    size_t numExperiments = (argc > 2) ? strtoul(argv[2], nullptr, 10) : 10UL;

    // Set up time tracking vectors
    vector<double> listVec;
    vector<double> quackVec;

    // Experiment 1: n pushes
    for (size_t exp = 0; exp < numExperiments; ++exp)
    {
        exp1(quackVec, listVec, n);
    }
    printExperiment(1, quackVec, listVec);

    // Experiment 2: n pushes, n pops
    for (size_t exp = 0; exp < numExperiments; ++exp){
        exp2(quackVec, listVec, n);
    }
    printExperiment(2, quackVec, listVec);



    // Experiment 3: n pushes n dequeues
    for (size_t exp = 0; exp < numExperiments; ++exp){
        exp3(quackVec, listVec, n);
    }
    printExperiment(3, quackVec, listVec);


    // Experiment 4: n pushes, n/2 alternating pops and dequeues.
    for (size_t exp = 0; exp < numExperiments; ++exp){
        exp4(quackVec, listVec, n);
    }
    printExperiment(4, quackVec, listVec);
    return 0;
}

