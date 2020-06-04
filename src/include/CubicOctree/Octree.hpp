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

        [[nodiscard]] std::bitset<3> NodePositionSingleLevel(const size_t x, const size_t y, const size_t z, const size_t levelSize) const noexcept
        {
            const uint8_t halfSize = levelSize/2;
            std::bitset<3> code;
            if (x >= halfSize) code.set(0, true);
            if (y >= halfSize) code.set(1, true);
            if (z >= halfSize) code.set(2, true);
            return code;
        }

        /// Updates x,y,z to be in range of next subnode.
        void InnerLocation(uint16_t currentDepth, const std::bitset<3> &locationalCode, size_t &x, size_t &y, size_t &z) const noexcept {
            size_t offset = size /std::pow(2, currentDepth)/2;
            if (locationalCode.test(0)) x -= offset;
            if (locationalCode.test(1)) y -= offset;
            if (locationalCode.test(2)) z -= offset;
        }

    public:
        Octree() : root({T(), true}){}

        void Set(size_t x, size_t y, size_t z, T value) {
            if (x > size || y > size || z > size) throw std::out_of_range("Out of bounds");
            uint16_t currentDepth = 0;
            OctreeNode<T>* currentNode = &root;
            while (currentDepth < Depth)
            {
                std::bitset<3> locationalCode = NodePositionSingleLevel(x, y, z, size/std::pow(2, currentDepth));
                if(currentNode->isLeaf) currentNode->Subdivide();
                currentNode = &std::get<std::shared_ptr<OctreeNode<T>[]>>(currentNode->data)[locationalCode.to_ulong()];
                InnerLocation(currentDepth, locationalCode, x, y, z);
                currentDepth++;
            }
            currentNode->data = swStorage<T>(value);
        }

        T Get(size_t x, size_t y, size_t z) const {
            if (x > size || y > size || z > size) throw std::out_of_range("Out of bounds");
            uint16_t currentDepth = 0;
            const OctreeNode<T>* currentNode = &root;
            while (!currentNode->isLeaf)
            {
                std::bitset<3> locationalCode = NodePositionSingleLevel(x, y, z, size/std::pow(2, currentDepth));
                currentNode = &std::get<std::shared_ptr<OctreeNode<T>[]>>(currentNode->data)[locationalCode.to_ulong()];
                InnerLocation(currentDepth, locationalCode, x, y, z);
                currentDepth++;
            }
            return std::get<swStorage<T>>(currentNode->data);
        }

        /// Collapse nodes that can be represented as a single branch.
        void SimplifyTree()
        {
            root.Simplify(true);
        }
    };
}

#endif//TERO_OCTREE_HPP