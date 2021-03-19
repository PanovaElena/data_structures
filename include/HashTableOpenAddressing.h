#pragma once
#include "Table.h"


// all cells of open addressing hash table contain some labels
// it is necessary to determine if a cell with default key == 0 is empty or not
// or to determine a cell is deleted or not
template <class ElemType>
struct HashTableOpenAddressingCell {
    std::pair<KeyType, ElemType> data;
    bool is_cell_empty = true;
    bool is_element_was_deleted = false;

    HashTableOpenAddressingCell() :
        is_cell_empty(true), is_element_was_deleted(false) {}

    HashTableOpenAddressingCell(const KeyType& first, const ElemType& second,
        bool flag1 = false, bool flag2 = false) :
        data(std::pair<KeyType, ElemType>(first, second)),
        is_cell_empty(flag1), is_element_was_deleted(flag2) {}
};


// class for a hash table with open addressing
template <class ElemType>
class HashTableOpenAddressing :
    public HashTable<ElemType, HashTableOpenAddressingCell<ElemType>> {

protected:

    using BaseClass = HashTable<ElemType, HashTableOpenAddressingCell<ElemType>>;

    // returns elements of probe sequence
    size_t getProbeSequenceElem(size_t hashValue, size_t i) {
        if (storage.size() == 0) throw "Storage is empty";
        return (hashValue + i * i) & (storage.size() - 1);
    }

    void repack() {
        M += size_t(1);   // double the storage size
        std::vector<HashTableOpenAddressingCell<ElemType>> tmp(getStorageSize(M));  // new storage
        std::swap(tmp, storage);

        // insertion of all elements (existing and deleted) again
        size = 0;
        for (size_t i = 0; i < tmp.size(); i++)
            if (!tmp[i].is_cell_empty || tmp[i].is_element_was_deleted) {
                insert(tmp[i].data.first, tmp[i].data.second);
                auto insertedElem = findCell(tmp[i].data.first);
                insertedElem->is_cell_empty = tmp[i].is_cell_empty;
                insertedElem->is_element_was_deleted = tmp[i].is_element_was_deleted;
            }
    }

    HashTableOpenAddressingCell<ElemType>* findCell(const KeyType& key) {
        size_t hashValue = hash(key);

        // looking for cell with key or empty cell
        // item does not exist if there is an empty cell in probe sequence
        size_t cell = 0, i = 0;
        for (; i < storage.size(); ++i) {
            cell = getProbeSequenceElem(hashValue, i);
            if (!storage[cell].is_element_was_deleted)  // skip deleted items
                if (storage[cell].is_cell_empty || storage[cell].data.first == key) // cell is empty or key has been found
                    break;
        }

        if (i == storage.size() || storage[cell].is_cell_empty
            || storage[cell].is_element_was_deleted || storage[cell].data.first != key)
            return nullptr;

        return &(storage[cell]);
    }

public:

    HashTableOpenAddressing(size_t M = START_STORAGE_SIZE_DEG_HASH_TABLE) :
        BaseClass(M) {}

    // search O(1) on the average
    std::pair<KeyType, ElemType>* find(const KeyType& key) {
        auto cell = findCell(key);
        if (!cell) return nullptr;
        return &(cell->data);
    }

    // insertion O(1) on the average
    bool insert(const KeyType& key, const ElemType& elem) {
        if (findCell(key)) return false;  // key already exists

        // if table is almost full then repack
        if (size >= size_t(MAX_FILL_FACTOR_HASH_TABLE * storage.size()))
            repack();

        size_t hashValue = hash(key);

        // looking for cell we can insert to
        size_t cell = 0, i = 0;
        for (; i < storage.size(); ++i) {
            cell = getProbeSequenceElem(hashValue, i);
            if (storage[cell].is_cell_empty)  // if cell is empty
                break;
        }

        if (i == storage.size()) {  // if table is full then repack and try again
            repack();
            return this->insert(key, elem);
        }

        // if empty cell was found
        size++;
        storage[cell] = HashTableOpenAddressingCell<ElemType>(key, elem);

        return true;
    }

    // erasing O(1) on the average
    bool erase(const KeyType& key) {
        auto cell = findCell(key);
        if (!cell) return false;  // key does not exists

        size--;
        cell->is_cell_empty = true;
        cell->is_element_was_deleted = true;
        return true;
    }

};
