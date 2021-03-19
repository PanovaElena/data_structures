#include "hash_table.h"

#include <string>
#include <vector>

#include <gtest.h>


template <class HashTableTestType>
class TestHashTable : public HashTableTestType, public testing::Test {

public:

    HashTableTestType* table = this;
    std::vector<KeyType> collisionKeys, notCollisionKeys;
    std::vector<std::string> values;

    TestHashTable() : HashTableTestType(3) {  // capacity = 2^3
        this->a = 1;  // it is very bad value, there will be a lot of collisions
        collisionKeys = { 0, 1, 2, 3, 4, 5 };  // give collisions
        notCollisionKeys = {
            size_t(1) << (W - M),
            size_t(2) << (W - M),
            size_t(3) << (W - M),
            size_t(4) << (W - M),
            size_t(5) << (W - M),
            size_t(6) << (W - M)
        };  // do not give collisions
        values = { "a", "b", "c", "d", "e", "f" };
    }

};

typedef TestHashTable<HashTableOpenAddressing<std::string>> TestHashTableOpenAddressing;
typedef TestHashTable<HashTableSeparateChaining<std::string>> TestHashTableSeparateChaining;


TEST_F(TestHashTableSeparateChaining, can_insert_and_find_first_element_if_collision) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    ASSERT_EQ("a", table->find(collisionKeys[0])->second);
}

TEST_F(TestHashTableSeparateChaining, can_insert_and_find_second_element_if_collision) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    ASSERT_EQ("b", table->find(collisionKeys[1])->second);
}

TEST_F(TestHashTableSeparateChaining, can_insert_and_find_third_element_if_collision) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    ASSERT_EQ("c", table->find(collisionKeys[2])->second);
}

TEST_F(TestHashTableSeparateChaining, can_find_third_element_if_collision_and_first_one_is_erased) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);
    table->erase(collisionKeys[0]);

    ASSERT_EQ("c", table->find(collisionKeys[2])->second);
}

TEST_F(TestHashTableSeparateChaining, can_find_third_element_if_collision_and_second_one_is_erased) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);
    table->erase(collisionKeys[1]);

    ASSERT_EQ("c", table->find(collisionKeys[2])->second);
}

TEST_F(TestHashTableSeparateChaining, cannot_find_element_if_it_is_erased) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);
    table->erase(collisionKeys[1]);

    ASSERT_EQ(nullptr, table->find(collisionKeys[1]));
}

TEST_F(TestHashTableSeparateChaining, can_repack_table_if_it_is_almost_filled) {
    // after 6 insertions repack should be called
    size_t size = storage.size();
    for (int i = 0; i < 5; i++)
        table->insert(notCollisionKeys[i], values[i]);

    table->insert(notCollisionKeys[5], values[5]);

    ASSERT_GT(storage.size(), size);
}

TEST_F(TestHashTableSeparateChaining, repack_dont_break_table) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    auto findRes = table->find(collisionKeys[4]);  // repack is called

    for (int i = 0; i < 3; i++)
        ASSERT_EQ(values[i], table->find(collisionKeys[i])->second);
    ASSERT_EQ(nullptr, table->find(collisionKeys[4]));
    ASSERT_EQ(3, table->getSize());
    ASSERT_FALSE(table->isEmpty());
}


TEST_F(TestHashTableOpenAddressing, can_insert_and_find_first_element_if_collision) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    ASSERT_EQ("a", table->find(collisionKeys[0])->second);
}

TEST_F(TestHashTableOpenAddressing, can_insert_and_find_second_element_if_collision) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    ASSERT_EQ("b", table->find(collisionKeys[1])->second);
}

TEST_F(TestHashTableOpenAddressing, can_insert_and_find_third_element_if_collision) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    ASSERT_EQ("c", table->find(collisionKeys[2])->second);
}

TEST_F(TestHashTableOpenAddressing, can_find_third_element_if_collision_and_first_one_is_erased) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);
    table->erase(collisionKeys[0]);

    ASSERT_EQ("c", table->find(collisionKeys[2])->second);
}

TEST_F(TestHashTableOpenAddressing, can_find_third_element_if_collision_and_second_one_is_erased) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);
    table->erase(collisionKeys[1]);

    ASSERT_EQ("c", table->find(collisionKeys[2])->second);
}

TEST_F(TestHashTableOpenAddressing, cannot_find_element_if_it_is_erased) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);
    table->erase(collisionKeys[1]);

    ASSERT_EQ(nullptr, table->find(collisionKeys[1]));
}

TEST_F(TestHashTableOpenAddressing, can_repack_table_if_it_is_almost_filled) {
    // after 6 insertions repack should be called
    size_t size = storage.size();
    for (int i = 0; i < 5; i++)
        table->insert(notCollisionKeys[i], values[i]);

    table->insert(notCollisionKeys[5], values[5]);

    ASSERT_GT(storage.size(), size);
}

TEST_F(TestHashTableOpenAddressing, repack_dont_break_table) {
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    auto findRes = table->find(collisionKeys[4]);  // repack is called

    for (int i = 0; i < 3; i++)
        ASSERT_EQ(values[i], table->find(collisionKeys[i])->second);
    ASSERT_EQ(nullptr, table->find(collisionKeys[4]));
    ASSERT_EQ(3, table->getSize());
    ASSERT_FALSE(table->isEmpty());
}

TEST_F(TestHashTableOpenAddressing, can_repack_table_if_insert_is_called_and_empty_cell_was_not_found) {
    // after 4 insertions with collisions repack should be called
    size_t size = storage.size();
    for (int i = 0; i < 3; i++)
        table->insert(collisionKeys[i], values[i]);

    table->insert(collisionKeys[3], values[3]);

    ASSERT_GT(storage.size(), size);
}
