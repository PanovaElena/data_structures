#pragma once
#include "Table.h"
#include "List.h"


// class for a hash table with separate chaining (cell is a list)
template <class ElemType>
class HashTableSeparateChaining : public HashTable<ElemType,
    List<std::pair<KeyType, ElemType>>> {

protected:

    // repack if table is almost filled
    void repack() {
        M += size_t(1);   // double the storage size
        std::vector<List<std::pair<KeyType, ElemType>>> tmp(getStorageSize(M));  // new storage
        std::swap(tmp, storage);

        // insertion of all elements again
        size = 0;
        for (size_t i = 0; i < tmp.size(); i++)
            for (auto ptr = storage[i].getFirst(); ptr; ptr = ptr->next)
                insert(ptr->data.first, ptr->data.second);
    }

    using BaseClass = HashTable<ElemType, List<std::pair<KeyType, ElemType>>>;

public:

    HashTableSeparateChaining(size_t M = START_STORAGE_SIZE_DEG_HASH_TABLE) :
        BaseClass(M) {}

    // search O(1) on the average
    std::pair<KeyType, ElemType>* find(const KeyType& key) override {
        size_t hashValue = hash(key);
        List<std::pair<KeyType, ElemType>>& cellList = storage[hashValue];

        auto ptr = cellList.getFirst();
        for (; ptr; ptr = ptr->next)
            if (ptr->data.first == key) break;

        if (!ptr) return nullptr;
        return &(ptr->data);
    }

    // insertion O(1) on the average
    bool insert(const KeyType& key, const ElemType& elem) override {
        if (find(key)) return false;  // key already exists

        // if table is almost full then repack
        if (size >= size_t(MAX_FILL_FACTOR_HASH_TABLE * storage.size()))
            repack();

        size_t hashValue = hash(key);
        storage[hashValue].pushFront(std::make_pair(key, elem));
        size++;

        return true;
    }

    // erasing O(1) on the average
    bool erase(const KeyType& key) override {
        if (!find(key)) return false;  // key does not exists

        size_t hashValue = hash(key);
        List<std::pair<KeyType, ElemType>>& cellList = storage[hashValue];

        auto ptr = cellList.getFirst();
        Node<std::pair<KeyType, ElemType>>* prevPtr = nullptr;
        for (; ptr; ptr = ptr->next)
            if (ptr->data.first == key) break;
            else prevPtr = ptr;

        storage[hashValue].eraseAfter(prevPtr);
        size--;
    }

};
