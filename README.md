# Cubic-octree [![CodeFactor](https://www.codefactor.io/repository/github/markusgod/cubic-octree/badge/master)](https://www.codefactor.io/repository/github/markusgod/cubic-octree/overview/master) [![Codacy Badge](https://app.codacy.com/project/badge/Grade/002dc8d796c34fc799801662452fa004)](https://www.codacy.com/manual/markusgod/cubic-octree?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=markusgod/cubic-octree&amp;utm_campaign=Badge_Grade)
Cubic octree with array-like access. Originally made to store voxel data for minecraft-like terrain.

## Example
```c++
Octree<int, 5> octree; // This will create octree of size 32x32x32 with only root node allocated
octree.set(1, 2, 3, 4) // This will allocate nodes down to depth of 5 and set node at position [1,2,3] to value 4.
int val = octree.get(1, 2, 3) // Get value at node [1,2,3]
octree.SimplifyTree() // Collapse nodes with same values.
```
