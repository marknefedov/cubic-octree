# Cubic-octree [![CodeFactor](https://www.codefactor.io/repository/github/marknefedov/cubic-octree/badge)](https://www.codefactor.io/repository/github/marknefedov/cubic-octree/badge) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/1855da7c093d47a3bb1d16e0b33a5b89)](https://app.codacy.com/gh/marknefedov/cubic-octree/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
Cubic octree with array-like access. Originally made to store voxel data for minecraft-like terrain.

## Example
```c++
Octree<int, 5> octree; // This will create octree of size 32x32x32 with only root node allocated
octree.set(1, 2, 3, 4) // This will allocate nodes down to depth of 5 and set node at position [1,2,3] to value 4.
int val = octree.get(1, 2, 3) // Get value at node [1,2,3]
octree.SimplifyTree() // Collapse nodes with same values.
```
