#ifndef TERO_OCTREENODE_HPP
#define TERO_OCTREENODE_HPP

#include <bitset>
#include <iostream>
#include <memory>
#include <variant>

// X right, Y up, Z forward

namespace Teroleon {
    template<typename T>
    requires std::equality_comparable<T> struct OctreeNode {
        std::variant<std::shared_ptr<OctreeNode<T>[]>, std::shared_ptr<T>> data;
        bool isLeaf;

        /// Subdivide current node and copy it content to new leaves.
        void Subdivide() {
            if (!isLeaf) return;
            isLeaf = false;
            auto savedValue = std::get<std::shared_ptr<T>>(data);
            data = std::shared_ptr<OctreeNode<T>[]>(new OctreeNode<T>[8]);
            auto subNodes = std::get<std::shared_ptr<OctreeNode<T>[]>>(data);
            for (uint8_t i = 0; i < 8; i++) {
                subNodes[i].isLeaf = true;
                subNodes[i].data = savedValue;
            }
        }

        /// Checks if all subnodes contain same value and if so, merges them into current node.
        /// \param simplifySubnodes perform recursive simplification.
        /// \return was simplification performed.
        bool Simplify(bool simplifySubNodes = false) {
            if (isLeaf) return false;
            auto subNodes = std::get<std::shared_ptr<OctreeNode<T>[]>>(data);
            if (simplifySubNodes) {
                for (uint8_t i = 0; i < 8; i++) {
                    subNodes[i].Simplify(true);
                }
            }
            for (uint8_t i = 0; i < 8; i++) {
                if (!subNodes[i].isLeaf) return false; // all subnodes must be leaves
            }
            std::shared_ptr<T> controlValue = std::get<std::shared_ptr<T>>(subNodes[0].data);
            for (uint8_t i = 0; i < 8; i++) {
                if (*std::get<std::shared_ptr<T>>(subNodes[i].data) != *controlValue) return false;
            }
            isLeaf = true;
            data = controlValue;
            return true;
        }
    };
}

#endif//TERO_OCTREENODE_HPP
