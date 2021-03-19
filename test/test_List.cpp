#include "List.h"

#include <gtest.h>

class TestList : public testing::Test {
public:

    List<int> list;
    List<int> emptyList;
    
    TestList() {
        list.pushFront(3);
        list.pushFront(2);
        list.pushFront(1);
    }
};


TEST_F(TestList, can_copy_list_correctly_1) {
    List<int> list2(list);
    EXPECT_EQ(list, list2);
}

TEST_F(TestList, can_copy_list_correctly_2) {
    List<int> list2(list);
    list.getFirst()->data = 10;
    EXPECT_NE(list, list2);
}

TEST_F(TestList, can_assign_list) {
    List<int> list2;
    list2 = list;
    list.getFirst()->data = 10;
    EXPECT_NE(list, list2);
}

TEST_F(TestList, can_clear_list) {
    list.clear();
    EXPECT_EQ(emptyList, list);
}

TEST_F(TestList, empty_method_is_correct) {
    list.clear();
    EXPECT_TRUE(list.empty());
}

TEST_F(TestList, can_push_front_to_empty_list) {
    emptyList.pushFront(1);
    EXPECT_EQ(emptyList.getFirst()->data, 1);
    EXPECT_EQ(emptyList.getFirst()->next, nullptr);
}

TEST_F(TestList, can_push_front_to_not_empty_list) {
    list.pushFront(10);
    EXPECT_EQ(list.getFirst()->data, 10);
    EXPECT_NE(list.getFirst()->next, nullptr);
}

TEST_F(TestList, can_pop_front) {
    list.popFront();
    EXPECT_EQ(list.getFirst()->data, 2);
}

TEST_F(TestList, pop_front_from_empty_list_does_nothing) {
    emptyList.popFront();
    EXPECT_EQ(emptyList, emptyList);
}

TEST_F(TestList, can_push_back) {
    emptyList.pushFront(1);
    emptyList.pushBack(10);
    EXPECT_EQ(emptyList.getFirst()->next->data, 10);
}

TEST_F(TestList, can_pop_back) {
    list.popBack();
    EXPECT_EQ(list.getFirst()->next->next, nullptr);
}

TEST_F(TestList, can_insert_after) {
    list.insertAfter(10, list.getFirst());
    EXPECT_EQ(list.getFirst()->next->data, 10);
}

TEST_F(TestList, can_erase_after) {
    list.eraseAfter(list.getFirst());
    EXPECT_EQ(list.getFirst()->next->data, 3);
}


