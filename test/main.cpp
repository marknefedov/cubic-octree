#include <CubicOctree/Octree.hpp>
#include <map>

using namespace Teroleon;

#ifdef FUZZER

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 3 || size > 5) return 0;
    if (data[0] < 0 || data[1] < 0 || data[2] < 0) return 0;
    int controlVal = data[3];
    int testval;
    Octree<int, 5> testOctree;
    try {
        testOctree.Set(data[0], data[1], data[2], data[3]);
        testOctree.FullSimplify();
        testval = testOctree.Get(data[0], data[1], data[2]);
    }
    catch (const std::out_of_range& e) {
        return 0;
    }
    return controlVal - testval; // Should always be zero.
}

#else

int main() {
    std::map<std::tuple<int, int, int>, int> testmap;
    Octree<int, 5> testOctree;
    for (int repeates = 0; repeates < 1000; ++repeates) {
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                for (int k = 0; k < 32; ++k) {
                    testOctree.Set(i, j, k, i+j+k);
                    testmap[{i,j,k}] = i+j+k;
                }
            }
        }
        testOctree.SimplifyTree();
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                for (int k = 0; k < 32; ++k) {
                    if (testOctree.Get(i, j, k) != testmap[{i,j,k}]) throw std::runtime_error("mismatch");
                }
            }
        }
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                for (int k = 0; k < 32; ++k) {
                    testOctree.Set(i, j, k, i + j);
                }
            }
        }
        testOctree.SimplifyTree();
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                for (int k = 0; k < 32; ++k) {
                    testOctree.Set(i, j, k, i);
                }
            }
        }
        testOctree.SimplifyTree();
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 32; ++j) {
                for (int k = 0; k < 32; ++k) {
                    testOctree.Set(i, j, k, 0);
                }
            }
        }
        testOctree.SimplifyTree();
    }
    return 0;
}

#endif