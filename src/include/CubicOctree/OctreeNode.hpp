#ifndef TERO_OCTREENODE_HPP
#define TERO_OCTREENODE_HPP

#include <bitset>
#include <iostream>
#include <memory>
#include <variant>

// X right, Y up, Z forward

namespace Teroleon {

    template<typename T>
    using sharedPtrTypeSelector = typename std::conditional<sizeof(T) <= sizeof(std::shared_ptr<T>), T, std::shared_ptr<T>>::type;

    template<typename T>
    class swStorage{
        sharedPtrTypeSelector<T> data;
        
    public:
        bool operator==(const swStorage &rhs) const {
            if constexpr (std::is_pointer_v<sharedPtrTypeSelector<T>>)
                return *data == *rhs.data;
            else
                return data == rhs.data;
        }
        bool operator!=(const swStorage &rhs) const {
            return !(rhs == *this);
        }

        operator T() const {
            if constexpr (std::is_pointer_v<sharedPtrTypeSelector<T>>)
                return *data;
            else
                return data;
        }

        swStorage(){
            if constexpr (std::is_pointer_v<sharedPtrTypeSelector<T>>)
                data = std::make_shared<T>(0);
        }

        explicit swStorage(const T& newData){
            if constexpr (std::is_pointer_v<sharedPtrTypeSelector<T>>)
                data = std::make_shared<T>(newData);
            else
                data = newData;
        }
    };

    template<typename T> requires std::equality_comparable<T>
    struct OctreeNode {
        std::variant<std::shared_ptr<OctreeNode<T>[]>, swStorage<T>> data;
        bool isLeaf;
        OctreeNode() : isLeaf(true){};
        OctreeNode(const T& newData, bool leaf):data(swStorage<T>(newData)), isLeaf(leaf) {
        }

        /// Subdivide current node and copy it content to new leaves.
        void Subdivide() noexcept {
            if (!isLeaf) return;
            isLeaf = false;
            auto savedValue = std::get<swStorage<T>>(data);
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
        bool Simplify(bool simplifySubNodes = false) noexcept {
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
            swStorage<T> controlValue = std::get<swStorage<T>>(subNodes[0].data);
            for (uint8_t i = 0; i < 8; i++) {
                if (std::get<swStorage<T>>(subNodes[i].data) != controlValue) return false;
            }
            isLeaf = true;
            data = controlValue;
            return true;
        }
    };
}

#endif//TERO_OCTREENODE_HPP
