# Quack

A templated C++ implementation of the Quack (Queue + Stack) ADT using 2 std::vectors

### Interface: 

`void push(item)`: Push an item to the stack

`void push_front(item)`: Push an item to the front of the queue

`void pop()`: Pop item from top of stack

`void dequeue()`: Remove the first item from the queue

`type front()`: Returns the item at the front of the queue

`type back()`: Returns the item at the back of the queue

`bool empty()`: Checks if the quack is empty

### Files in this Repo: 

`quack.hpp`: Header file with function and class declarations

`quack-private.hpp`: Source code for Quack ADT

`Makefile`: Build the experiment

`Experiment.cpp`: Runs a small experiment to test the speed of the quack vs `std::queue`. 

Usage: `./experiment n numTrials`


### Asymtotic Runtime (n items in quack): 

`push(item)`: $\Theta(1)$ worst case

`pop()`: $\Theta(n)$ worst case, $\Theta(1)$ amortized

`dequeue()`: $\Theta(n)$ worst case, $\Theta(1)$ amortized

### Run Time Experiment:
To see how this compares to the std::queue library. I did 4 tests with n = 10000000 (30 trials):
- Experiment 1: Push n items to the quack/queue
- Experiment 2: Push n items to quack/queue. Pop all n items afterwards
- Experiment 3: Push n items to the quack/queue. Dequeue all n items. (This triggers the $\Theta(n)$ worst case for the Quack most frequently)
- Experiment 4: Push n items to the quack/queue. Alternate between Dequeuing and popping n/2 times. 

Times for my machine (seconds): 

| Experiment 1 | Avg | Stdev |
| --- | --- | --- |
| Quack |	 0.0366994|	0.0072778 |
| List |	 0.47697 |	0.0037062 |

|Experiment 2 | Avg 	| Stdev|
| --- | --- | --- |
|Quack |	 0.0526603 |	0.000914514|
|List 	| 0.924854	| 0.006616|

|Experiment 3 |Avg 	| Stdev|
| --- | --- | --- |
|Quack 	| 0.0636115	| 0.000747878|
|List |	 1.30905	|0.0269176|

|Experiment 4| Avg 	| Stdev|
| --- | --- | --- |
|Quack 	| 0.0614627	|0.000586715|
|List 	| 1.19822	| 0.0298306|
