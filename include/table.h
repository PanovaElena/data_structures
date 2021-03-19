#pragma once
#include <vector>
#include <random>


typedef uint32_t KeyType;


template <class ElemType>
class TableInterface {
public:

    // returns true if elem was inserted
    virtual bool insert(const KeyType& key, const ElemType& elem) = 0;

    // returns true if elem was deleted
    virtual bool erase(const KeyType& key) = 0;

    // returns nullptr if elem was not found
    virtual std::pair<KeyType, ElemType>* find(const KeyType& key) = 0;

    virtual void clear() = 0;
    virtual bool isEmpty() const = 0;
    virtual size_t getSize() const = 0;

};


const double REPACK_COEFF = 1.3;
const size_t START_STORAGE_SIZE = 10;

template <class ElemType, class CellType = std::pair<KeyType, ElemType>>
class TableByArray : public TableInterface<ElemType> {
protected:

    std::vector<CellType> storage;
    size_t size = 0;

    void repack() {  // re-allocates memory
        storage.resize(size_t(storage.size()*REPACK_COEFF));
    }

public:

    TableByArray(size_t storageSize = START_STORAGE_SIZE) : storage(storageSize) {}

    void clear() override {
        std::vector<CellType> tmp(START_STORAGE_SIZE);
        std::swap(tmp, storage);
        size = 0;
    }

    size_t getSize() const override {
        return size;
    }

    bool isEmpty() const override {
        return size == 0;
    }

};


const size_t START_STORAGE_SIZE_DEG_HASH_TABLE = 4;  // start storage size = 2^4 = 16
const double MAX_FILL_FACTOR_HASH_TABLE = 0.7;

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

