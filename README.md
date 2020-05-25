# Cubbic-octree
I made this for my voxel game to save some space.

###Example
```c++
Octree<int, 5> octree; // This will create octree of size 32x32x32 with only root node allocated
octree.set(1, 2, 3, 4) // This will allocate nodes down to depth of 5 and set node at position [1,2,3] to value 4.
int val = octree.get(1, 2, 3) // Get value at node [1,2,3]
octree.SimplifyTree() // Collapse nodes with same values.
```