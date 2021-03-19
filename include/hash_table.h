#pragma once
#include "table.h"
#include "list.h"
#include <functional>
#include <random>


const size_t START_STORAGE_SIZE_DEG_HASH_TABLE = 4;  // start storage size = 2^4 = 16
const double MAX_FILL_FACTOR = 0.7;

// base class for hash tables
// defines hash function
template <class ElemType, class CellType>
class HashTable : public TableByArray<ElemType, CellType> {

protected:

    size_t M = START_STORAGE_SIZE_DEG_HASH_TABLE;  // storage size is 2^M

    size_t getStorageSize(size_t M) {  // returns 2^M
        return size_t(1) << M;
    }

    // length of mashine word (32)
    const size_t W = sizeof(uint32_t) * 8;

    // "a" is a random parameter of the universal hash function
    size_t a;

    void setHashParameter() {
        std::random_device rd;
        std::default_random_engine randGen(rd());
        std::uniform_int_distribution<uint32_t> dist;
        a = (size_t)dist(randGen);
    }

    // universal hash function that can be computed fast
    size_t hash(KeyType key) {
        return (size_t)((uint32_t)(a * (uint64_t)key) >> (W - M));
    }

public:

    HashTable(size_t M = START_STORAGE_SIZE_DEG_HASH_TABLE) :
        M(M), TableByArray<ElemType, CellType>(getStorageSize(M)) {
        setHashParameter();
    }

    void clear() override {
        TableByArray<ElemType, CellType>::clear();
        M = START_STORAGE_SIZE_DEG_HASH_TABLE;
        storage.resize(getStorageSize(M));
    }

};


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
        if (size >= size_t(MAX_FILL_FACTOR * storage.size()))
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
        if (size >= size_t(MAX_FILL_FACTOR * storage.size()))
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
