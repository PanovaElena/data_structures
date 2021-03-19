#include "ordered_unordered_table.h"
#include "hash_table.h"

#include <string>

#include <gtest.h>


// macro to run a test for all types of search tables
// defines name "TableType" as a type of a table inside of the test body
#define TEST_FOR_ALL_TABLES(test_case, test_name)                                    \
template <template<class> class TableType> void func##test_case##test_name();        \
TEST(test_case##UnorderedTable, test_name) {                                         \
    func##test_case##test_name<UnorderedTable>();                                    \
}                                                                                    \
TEST(test_case##OrderedTable, test_name) {                                           \
    func##test_case##test_name<OrderedTable>();                                      \
}                                                                                    \
TEST(test_case##HashTableOpenAddressing, test_name) {                                \
    func##test_case##test_name<HashTableOpenAddressing>();                           \
}                                                                                    \
TEST(test_case##HashTableSeparateChaining, test_name) {                              \
    func##test_case##test_name<HashTableSeparateChaining>();                         \
}                                                                                    \
template <template<class> class TableType>                                           \
void func##test_case##test_name()



TEST_FOR_ALL_TABLES(TestCommon, dont_throw_exception_when_insert) {
    TableType<std::string> table;

    ASSERT_NO_THROW(table.insert(1, "a"));
}

TEST_FOR_ALL_TABLES(TestCommon, dont_throw_exception_when_find) {
    TableType<std::string> table;
    table.insert(1, "a");

    ASSERT_NO_THROW(table.find(1));
}

TEST_FOR_ALL_TABLES(TestCommon, can_insert_and_find_element) {
    TableType<std::string> table;
    table.insert(1, "a");

    ASSERT_NE(table.find(1), nullptr);
}

TEST_FOR_ALL_TABLES(TestCommon, find_after_insert_gives_correct_ptr) {
    TableType<std::string> table;
    table.insert(1, "a");

    table.insert(2, "b");
    std::pair<KeyType, std::string>* searchRes = table.find(2);

    ASSERT_EQ("b", searchRes->second);
}

TEST_FOR_ALL_TABLES(TestCommon, cant_find_default_key_if_table_is_empty) {
    TableType<std::string> table;

    ASSERT_EQ(table.find(0), nullptr);
}

TEST_FOR_ALL_TABLES(TestCommon, insert_by_existent_key_gives_false) {
    TableType<std::string> table;
    table.insert(1, "a");

    ASSERT_FALSE(table.insert(1, "b"));
}

TEST_FOR_ALL_TABLES(TestCommon, dont_throw_exception_when_erase_by_key) {
    TableType<std::string> table;
    table.insert(1, "a");

    ASSERT_NO_THROW(table.erase(1));
}

TEST_FOR_ALL_TABLES(TestCommon, can_erase_element) {
    TableType<std::string> table;
    table.insert(1, "a");

    table.erase(1);

    ASSERT_EQ(table.find(1), nullptr);
}

TEST_FOR_ALL_TABLES(TestCommon, erasing_nonexistend_key_gives_false) {
    TableType<std::string> table;
    table.insert(1, "a");

    ASSERT_FALSE(table.erase(2));
}

TEST_FOR_ALL_TABLES(TestCommon, can_clear_table) {
    TableType<std::string> table;
    table.insert(1, "a");
    table.insert(4, "b");
    table.insert(2, "c");
    table.insert(15, "d");

    table.clear();

    ASSERT_TRUE(table.isEmpty());
}

TEST_FOR_ALL_TABLES(TestCommon, can_find_element_complex_test) {
    TableType<std::string> table;
    table.insert(0, "a");
    table.insert(1, "b");
    table.insert(3, "d");
    table.insert(2, "c");

    ASSERT_EQ(0, table.find(0)->first);
    ASSERT_EQ(1, table.find(1)->first);
    ASSERT_EQ(2, table.find(2)->first);
    ASSERT_EQ(3, table.find(3)->first);
}

TEST_FOR_ALL_TABLES(TestCommon, can_insert_and_erase_complex_test) {
    TableType<std::string> table;
    table.insert(1, "a");
    table.insert(4, "b");
    table.insert(2, "c");
    table.insert(15, "d");
    table.insert(6, "d");
    table.insert(7, "d");

    table.erase(6);
    table.erase(7);
    table.erase(4);
    table.erase(2);
    table.erase(1);
    table.erase(15);

    ASSERT_TRUE(table.isEmpty());
}