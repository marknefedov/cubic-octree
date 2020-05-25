#ifndef TERO_OCTREE_HPP
#define TERO_OCTREE_HPP

#include "OctreeNode.hpp"
#include <cmath>

namespace Teroleon{
    /// Cubic octree allocates noodes on demand.
    /// \tparam T Stored type
    /// \tparam Depth Number of splits, index limits is 2^depth.
    template<typename T, uint16_t Depth>
    class Octree{
        OctreeNode<T> root;
        const size_t size = std::pow(2, Depth);

        std::bitset<3> NodePositionSingleLevel(const size_t x, const size_t y, const size_t z, const size_t levelSize)
        {
            const uint8_t halfSize = levelSize/2;
            std::bitset<3> code;
            if (x >= halfSize) code.set(0, true);
            if (y >= halfSize) code.set(1, true);
            if (z >= halfSize) code.set(2, true);
            return code;
        }

    public:
        Octree() : root({std::make_shared<T>(T()), true}){}

        void Set(size_t x, size_t y, size_t z, T value) {
            if (x > size || y > size || z > size) throw std::out_of_range("Out of bounds");
            uint16_t currentDepth = 0;
            OctreeNode<T>* currentNode = &root;
            while (currentDepth < Depth)
            {
                std::bitset<3> locationalCode = NodePositionSingleLevel(x, y, z, size/std::pow(2, currentDepth));
                size_t index = locationalCode.to_ulong();
                if(currentNode->isLeaf) currentNode->Subdivide();
                currentNode = &std::get<std::shared_ptr<OctreeNode<T>[]>>(currentNode->data)[index];
                size_t offset = size/std::pow(2, currentDepth)/2;
                if (locationalCode.test(0)) x -= offset;
                if (locationalCode.test(1)) y -= offset;
                if (locationalCode.test(2)) z -= offset;
                currentDepth++;
            }
            currentNode->data = std::make_shared<T>(value);
        }

        T Get(size_t x, size_t y, size_t z){
            if (x > size || y > size || z > size) throw std::out_of_range("Out of bounds");
            uint16_t currentDepth = 0;
            OctreeNode<T>* currentNode = &root;
            while (!currentNode->isLeaf)
            {
                std::bitset<3> locationalCode = NodePositionSingleLevel(x, y, z, size/std::pow(2, currentDepth));
                size_t index = locationalCode.to_ulong();
                currentNode = &std::get<std::shared_ptr<OctreeNode<T>[]>>(currentNode->data)[index];
                size_t offset = size/std::pow(2, currentDepth)/2;
                if (locationalCode.test(0)) x -= offset;
                if (locationalCode.test(1)) y -= offset;
                if (locationalCode.test(2)) z -= offset;
                currentDepth++;
            }
            return *std::get<std::shared_ptr<T>>(currentNode->data);
        }

        /// Collapses nodes that can be represented as single branch.
        void SimplifyTree()
        {
            root.Simplify(true);
        }
    };
}

#endif//TERO_OCTREE_HPP