#pragma once
#include "Table.h"


template <class ElemType>
class OrderedTable : public TableByArray<ElemType> {

    // temporary O(n)
    // returns position to insert
    size_t binarySearch(const KeyType& key) {
        size_t leftIndex = 0, rightIndex = size;
        while (leftIndex + 1 < rightIndex) {
            size_t middleIndex = (leftIndex + rightIndex) / 2;
            if (key < storage[middleIndex].first)
                rightIndex = middleIndex;
            else leftIndex = middleIndex;
        }
        if (leftIndex < size && storage[leftIndex].first >= key) return leftIndex;
        return rightIndex;
    }

public:

    // binary search O(log(n))
    std::pair<KeyType, ElemType>* find(const KeyType& key) override {
        size_t searchRes = binarySearch(key);
        if (searchRes == size || storage[searchRes].first != key) // key does not exist
            return nullptr;
        return &(storage[searchRes]);
    }

    // insertion O(log(n)) + O(n)
    bool insert(const KeyType& key, const ElemType& elem) override {
        size_t searchRes = binarySearch(key);
        if (searchRes != size && storage[searchRes].first == key)  // key already exists
            return false;

        if (storage.size() == size) repack();
        
        for (size_t i = size; i > searchRes; i--)
            storage[i] = storage[i - 1];
        storage[searchRes] = std::make_pair(key, elem);
        size++;

        return true;
    }

    // erasing O(log(n)) + O(n)
    bool erase(const KeyType& key) override {
        size_t searchRes = binarySearch(key);
        if (searchRes == size || storage[searchRes].first != key)  // key does not exist
            return false;

        for (size_t i = searchRes + 1; i < size; i++)
            storage[i - 1] = storage[i];
        size--;

        return true;
    }

};
