#pragma once
#include "Table.h"


template <class ElemType>
class UnorderedTable : public TableByArray<ElemType> {

    // linear search O(n)
    // returns position to insert
    size_t linearSearch(const KeyType& key) {
        size_t index = 0;
        for (; index < size; index++)
            if (storage[index].first == key) break;
        return index;
    }

public:

    // linear search O(n)
    std::pair<KeyType, ElemType>* find(const KeyType& key) override {
        size_t searchRes = linearSearch(key);
        if (searchRes == size)
            return nullptr;
        return &(storage[searchRes]);
    }

    // insertion O(n) + O(1)
    bool insert(const KeyType& key, const ElemType& elem) override {
        size_t searchRes = linearSearch(key);
        if (searchRes != size && storage[searchRes].first == key)  // key already exists
            return false;

        if (storage.size() == size) repack();
        
        storage[size] = std::make_pair(key, elem);
        size++;

        return true;
    }

    // erasing O(n) + O(1)
    bool erase(const KeyType& key) override {
        size_t searchRes = linearSearch(key);
        if (searchRes == size || storage[searchRes].first != key)  // key does not exist
            return false;

        size--;
        std::swap(storage[searchRes], storage[size]);

        return true;
    }

};

