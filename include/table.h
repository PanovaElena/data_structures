#pragma once
#include <vector>


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