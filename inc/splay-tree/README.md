# splay-tree
C++ Template of a Splay Tree with an iterator and no parent pointers

My Implementation of a splay tree with no extra overhead in each node as a header only library. 

## Interface

`insert(key, value):` Inserts `key` into the tree with value `value`. No return value. 

`remove(key):` Removes `key` from the tree. If the key is not present, splays the tree such the last leaf node becomes the new root. No return

`find(key):` Returns an iterator pointing to the key-value pair. If the key is not present, then returns the `end` iterator.

`exists(key):` Checks if a key is in the tree. Performs the splay operation. 

`begin():` Returns an iterator pointing to the beginning of the tree. 

`end():` End iterator. Allows for `for (auto iter : splayTree)` use. 

`size():` Returns the tree size. 

`operator[key]:` Returns the value (*not an iterator*) associated with `key`. Undefined for keys that aren't in the tree, does not allow for insertion inside the tree

All time complexities meet the  asymtotic bounds specified in Sleator and Tarjan's 1985 paper. 

## Files: 
`splayTree.hpp:` Header file listing the implementation

`splayTree-private.hpp:` Has the entire implementation

