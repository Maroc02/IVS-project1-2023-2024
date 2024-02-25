//======= Copyright (c) 2024, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     Marek Čupr <xcuprm01@stud.fit.vutbr.cz>
// $Date:       $2024-02-19
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Marek Čupr
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>
#include "gtest/gtest.h"
#include "white_box_code.h"

using namespace testing;

// Create an empty hashtable
class EmptyHash : public Test
{
protected:
    hash_map_t *empty_hash;

    // Allocate the memory
    void SetUp() override {
        empty_hash = hash_map_ctor();
    }

    // Free the memory
    void TearDown() override {
        hash_map_dtor(empty_hash);
    }
};

// Create a non-empty hashtable
class NonEmptyHash : public Test
{
protected:
    hash_map_t *non_empty_hash;
    std::vector<const char*> keys = {"dobry", "den", "jak", "se", "dnes", "mate", "pane", "opravujici", "klidecek?"};

    // Allocate the memory
    void SetUp() override {
        non_empty_hash = hash_map_ctor();
        for (int i = 0; i < keys.size(); i++)
            hash_map_put(non_empty_hash, keys[i], i);
    }

    // Free the memory
    void TearDown() override {
        hash_map_dtor(non_empty_hash);
    }
};

// Create hashtable with collisions
class CollisionHash : public Test
{
protected:
    hash_map_t *collision_hash;
    std::vector<const char*> keys = {"klidecek", "kecedilk", "ilcedkek", "cikleedk"};

    // Allocate the memory
    void SetUp() override {
        collision_hash = hash_map_ctor();
        for (int i = 0; i < keys.size(); i++)
            hash_map_put(collision_hash, keys[i], 69);
    }

    // Free the memory
    void TearDown() override {
        hash_map_dtor(collision_hash);
    }
};

/* ************************** */
/* ****  EMPTY HASHTABLE **** */
/* ************************** */
TEST_F(EmptyHash, hash_map_clear){
    // Clear the hashtable
    hash_map_clear(empty_hash);
    EXPECT_EQ(empty_hash->used, 0);
    EXPECT_EQ(empty_hash->allocated, 8);
    ASSERT_EQ(empty_hash->first, nullptr);
    ASSERT_EQ(empty_hash->last, nullptr);
}

TEST_F(EmptyHash, hash_map_reserve){
    // Check default capacity
    ASSERT_EQ(empty_hash->allocated, 8);

    // Reserve valid size
    auto ret_code = hash_map_reserve(empty_hash, 100);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(empty_hash->allocated, 100);

    // Reserve valid size
    ret_code = hash_map_reserve(empty_hash, 200);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(empty_hash->allocated, 200);

    // Reserve invalid size
    ret_code = hash_map_reserve(empty_hash, -7);
    EXPECT_EQ(ret_code, MEMORY_ERROR);
    EXPECT_EQ(empty_hash->allocated, 200);

    // Reserve valid size
    ret_code = hash_map_reserve(empty_hash, 5);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(empty_hash->allocated, 5);
}

TEST_F(EmptyHash, hash_map_size){
    // Check hashtable size
    EXPECT_EQ(hash_map_size(empty_hash), 0);

    // Add item and check hashtable size
    hash_map_put(empty_hash, "random", 10);
    EXPECT_EQ(hash_map_size(empty_hash), 1);

    // Add item and check hashtable size
    hash_map_put(empty_hash, "random2", 10);
    EXPECT_EQ(hash_map_size(empty_hash), 2);

    // Add item and check hashtable size
    hash_map_put(empty_hash, "random3", 10);
    EXPECT_EQ(hash_map_size(empty_hash), 3);
}

TEST_F(EmptyHash, hash_map_capacity){
    // Check hashtable capacity
    EXPECT_EQ(hash_map_capacity(empty_hash), 8);

    // Add items and check hashtable capacity
    hash_map_put(empty_hash, "random1", 10);
    hash_map_put(empty_hash, "random2", 10);
    hash_map_put(empty_hash, "random3", 10);
    hash_map_put(empty_hash, "random4", 10);
    hash_map_put(empty_hash, "random5", 10);
    hash_map_put(empty_hash, "random6", 10);
    hash_map_put(empty_hash, "random7", 10);
    hash_map_put(empty_hash, "random8", 10);
    hash_map_put(empty_hash, "random9", 10);

    ASSERT_GT(hash_map_capacity(empty_hash), 8);
}

TEST_F(EmptyHash, hash_map_contains){
    // Find non-existing item
    EXPECT_FALSE(hash_map_contains(empty_hash, "random"));

    // Find non-existing item
    EXPECT_FALSE(hash_map_contains(empty_hash, "random2"));
}

TEST_F(EmptyHash, hash_map_put){
    // Add random item
    hash_map_state_code_t hash_code = hash_map_put(empty_hash, "random", 69);
    EXPECT_EQ(hash_code, OK);

    // Add existing item
    hash_code = hash_map_put(empty_hash, "random", 10);
    EXPECT_EQ(hash_code, KEY_ALREADY_EXISTS);
}

TEST_F(EmptyHash, hash_map_get){
    int value;

    // Find non-existing item
    hash_map_state_code_t hash_code = hash_map_get(empty_hash, "random",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);

    // Find non-existing item
    hash_code = hash_map_get(empty_hash, "random2",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);
}

TEST_F(EmptyHash, hash_map_pop){
    int value;

    // Delete non-existing key and save its' value
    hash_map_state_code_t hash_code = hash_map_pop(empty_hash, "random",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);

    // Delete non-existing key and save its' value
    hash_code = hash_map_pop(empty_hash, "random2",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);
}

TEST_F(EmptyHash, hash_map_remove){
    // Delete non-existing key
    hash_map_state_code_t hash_code = hash_map_remove(empty_hash, "random");
    EXPECT_EQ(hash_code, KEY_ERROR);

    // Delete non-existing key
    hash_code = hash_map_remove(empty_hash, "random2");
    EXPECT_EQ(hash_code, KEY_ERROR);
}

/* ***************************** */
/* **** NON-EMPTY HASHTABLE **** */
/* ***************************** */
TEST_F(NonEmptyHash, hash_map_clear){
    // Check hashtable capacity
    size_t hash_capacity = hash_map_capacity(non_empty_hash);
    ASSERT_GT(hash_capacity, 0);

    // Clear the hashtable
    hash_map_clear(non_empty_hash);
    EXPECT_EQ(non_empty_hash->used, 0);
    EXPECT_EQ(non_empty_hash->allocated, hash_capacity);
    ASSERT_EQ(non_empty_hash->first, nullptr);
    ASSERT_EQ(non_empty_hash->last, nullptr);
}

TEST_F(NonEmptyHash, hash_map_reserve){
    // Check first item
    ASSERT_NE(non_empty_hash->first, nullptr);

    // Check hashtable capacity
    size_t hash_capacity = hash_map_capacity(non_empty_hash);
    ASSERT_GT(hash_capacity, 0);
    EXPECT_EQ(non_empty_hash->allocated, hash_capacity);

    // Reserve valid size
    auto ret_code = hash_map_reserve(non_empty_hash, 100);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(non_empty_hash->allocated, 100);

    // Reserve valid size
    ret_code = hash_map_reserve(non_empty_hash, 200);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(non_empty_hash->allocated, 200);

    // Reserve invalid size
    ret_code = hash_map_reserve(non_empty_hash, -7);
    EXPECT_EQ(ret_code, MEMORY_ERROR);
    EXPECT_EQ(non_empty_hash->allocated, 200);

    // Reserve invalid size
    ret_code = hash_map_reserve(non_empty_hash, SIZE_MAX);
    EXPECT_EQ(ret_code, MEMORY_ERROR);
}

TEST_F(NonEmptyHash, hash_map_size){
    // Check hashtable size
    size_t hash_size = hash_map_size(non_empty_hash); 
    ASSERT_GT(hash_size, 0);

    // Add item and check hashtable size
    hash_map_put(non_empty_hash, "random", 10);
    EXPECT_EQ(hash_map_size(non_empty_hash), hash_size + 1);

    // Add item and check hashtable size
    hash_map_put(non_empty_hash, "random2", 10);
    EXPECT_EQ(hash_map_size(non_empty_hash), hash_size + 2);

    // Add item and check hashtable size
    hash_map_put(non_empty_hash, "random3", 10);
    EXPECT_EQ(hash_map_size(non_empty_hash), hash_size + 3);
}

TEST_F(NonEmptyHash, hash_map_capacity){
    // Check hashtable capacity
    size_t hash_capacity = hash_map_capacity(non_empty_hash); 
    ASSERT_GT(hash_capacity, 0);
}

TEST_F(NonEmptyHash, hash_map_contains){
    // Check first item
    ASSERT_NE(non_empty_hash->first, nullptr);

    // Find existing item
    EXPECT_TRUE(hash_map_contains(non_empty_hash, "dobry"));
    // Find non-existing item
    EXPECT_FALSE(hash_map_contains(non_empty_hash, "random2"));
}

TEST_F(NonEmptyHash, hash_map_put){
    // Check first item
    ASSERT_NE(non_empty_hash->first, nullptr);

    // Add existing key
    hash_map_state_code_t hash_code = hash_map_put(non_empty_hash, "dobry", 2);
    EXPECT_EQ(hash_code, KEY_ALREADY_EXISTS);

    // Add random key
    hash_code = hash_map_put(non_empty_hash, "ahoj", 69);
    EXPECT_EQ(hash_code, OK);
}

TEST_F(NonEmptyHash, hash_map_get){
    // Check first item
    ASSERT_NE(non_empty_hash->first, nullptr);


    int value;
    // Find non-existing item
    hash_map_state_code_t hash_code = hash_map_get(non_empty_hash, "random",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);

    // Find existing key
    hash_code = hash_map_get(non_empty_hash, "dobry",  &value);
    EXPECT_EQ(hash_code, OK);
    EXPECT_EQ(value, 0);
}

TEST_F(NonEmptyHash, hash_map_pop){
    // Check first item
    ASSERT_NE(non_empty_hash->first, nullptr);

    int value;
    // Delete existing item and save its' value
    hash_map_state_code_t hash_code = hash_map_pop(non_empty_hash, "dobry",  &value);
    EXPECT_EQ(hash_code, OK);
    EXPECT_EQ(value, 0);

    // Delete non-existing item and save its' value
    hash_code = hash_map_pop(non_empty_hash, "random",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);
}

TEST_F(NonEmptyHash, hash_map_remove){
    // Check first item
    ASSERT_NE(non_empty_hash->first, nullptr);

    // Delete existing item
    hash_map_state_code_t hash_code = hash_map_remove(non_empty_hash, "dobry");
    EXPECT_EQ(hash_code, OK);

    // Delete non-existing item
    hash_code = hash_map_remove(non_empty_hash, "random");
    EXPECT_EQ(hash_code, KEY_ERROR);
}

/* ****************************** */
/* **** HASHTABLE COLLISIONS **** */
/* ****************************** */
TEST_F(CollisionHash, hash_map_clear){
    // Check first item
    ASSERT_NE(collision_hash->first, nullptr);

    // Check hashtable capacity
    size_t hash_capacity = hash_map_capacity(collision_hash);
    ASSERT_GT(hash_capacity, 0);

    // Clear the hashtable
    hash_map_clear(collision_hash);
    EXPECT_EQ(collision_hash->used, 0);
    EXPECT_EQ(collision_hash->allocated, hash_capacity);
    ASSERT_EQ(collision_hash->first, nullptr);
    ASSERT_EQ(collision_hash->last, nullptr);
}

TEST_F(CollisionHash, hash_map_reserve){
    // Check first item
    ASSERT_NE(collision_hash->first, nullptr);

    // Check hashtable capacity
    size_t hash_capacity = hash_map_capacity(collision_hash);
    ASSERT_GT(hash_capacity, 0);
    EXPECT_EQ(collision_hash->allocated, hash_capacity);

    // Reserve valid size
    auto ret_code = hash_map_reserve(collision_hash, 100);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(collision_hash->allocated, 100);

    // Reserve valid size
    ret_code = hash_map_reserve(collision_hash, 200);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(collision_hash->allocated, 200);

    // Reserve invalid size
    ret_code = hash_map_reserve(collision_hash, -7);
    EXPECT_EQ(ret_code, MEMORY_ERROR);
    EXPECT_EQ(collision_hash->allocated, 200);

    // Reserve invalid valid size
    ret_code = hash_map_reserve(collision_hash, 2);
    EXPECT_EQ(ret_code, VALUE_ERROR);
    EXPECT_EQ(collision_hash->allocated, 200);

    // Reserve valid size
    ret_code = hash_map_reserve(collision_hash, 200);
    EXPECT_EQ(ret_code, OK);
    EXPECT_EQ(collision_hash->allocated, 200);
}

TEST_F(CollisionHash, hash_map_size){
    // Check hashtable size
    size_t hash_size = hash_map_size(collision_hash); 
    ASSERT_GT(hash_size, 0);

    // Add item and check hashtable size
    hash_map_put(collision_hash, "random", 10);
    EXPECT_EQ(hash_map_size(collision_hash), hash_size + 1);

    // Add item and check hashtable size
    hash_map_put(collision_hash, "random2", 10);
    EXPECT_EQ(hash_map_size(collision_hash), hash_size + 2);

    // Add item and check hashtable size
    hash_map_put(collision_hash, "random3", 10);
    EXPECT_EQ(hash_map_size(collision_hash), hash_size + 3);
}

TEST_F(CollisionHash, hash_map_capacity){
    // Check hashtable capacity
    size_t hash_capacity = hash_map_capacity(collision_hash); 
    ASSERT_GT(hash_capacity, 0);
}

TEST_F(CollisionHash, hash_map_contains){
    // Check first item
    ASSERT_NE(collision_hash->first, nullptr);

    // Find existing key
    EXPECT_TRUE(hash_map_contains(collision_hash, "klidecek"));

    // Find existing key
    ASSERT_TRUE(hash_map_contains(collision_hash, "kecedilk"));

    // Find non-existing key
    EXPECT_FALSE(hash_map_contains(collision_hash, "random"));

    // Find non-existing key
    ASSERT_FALSE(hash_map_contains(collision_hash, "random2"));
}

TEST_F(CollisionHash, hash_map_put){
    // Check first item
    ASSERT_NE(collision_hash->first, nullptr);

    // Add existing item
    hash_map_state_code_t hash_code = hash_map_put(collision_hash, "kecedilk", 2);
    EXPECT_EQ(hash_code, KEY_ALREADY_EXISTS);

    // Add colliding item
    hash_code = hash_map_put(collision_hash, "kieeldck", 111);
    EXPECT_EQ(hash_code, OK);

    // Add non-existing item
    hash_code = hash_map_put(collision_hash, "random", 1122);
    EXPECT_EQ(hash_code, OK);

    // Add non-existing item
    hash_code = hash_map_put(collision_hash, "random2", 123);
    EXPECT_EQ(hash_code, OK);

    // Add non-existing item
    hash_code = hash_map_put(collision_hash, "random2", 123);
    EXPECT_EQ(hash_code, KEY_ALREADY_EXISTS);
}

TEST_F(CollisionHash, hash_map_get){
    // Check first item
    ASSERT_NE(collision_hash->first, nullptr);

    int value, value_colliding;
    // Find non-existing item
    hash_map_state_code_t hash_code = hash_map_get(collision_hash, "random",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);

    // Find non-existing item
    hash_code = hash_map_get(collision_hash, "random2",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);

    // Find existing item
    hash_code = hash_map_get(collision_hash, "klidecek",  &value);
    EXPECT_EQ(hash_code, OK);

    // Find colliding item
    hash_code = hash_map_get(collision_hash, "kecedilk",  &value_colliding);
    EXPECT_EQ(hash_code, OK);

    // Check values of colliding items
    EXPECT_EQ(value, value_colliding);
}

TEST_F(CollisionHash, hash_map_pop){
    // Check first item
    ASSERT_NE(collision_hash->first, nullptr);

    int value, value_colliding;
    // Delete existing item and save its' value
    hash_map_state_code_t hash_code = hash_map_pop(collision_hash, "klidecek",  &value);
    EXPECT_EQ(hash_code, OK);

    // Delete colliding item and save its' value
    hash_code = hash_map_pop(collision_hash, "cikleedk",  &value_colliding);
    EXPECT_EQ(hash_code, OK);
  
    // Check values of colliding items
    EXPECT_EQ(value, value_colliding);

    // Delete non-existing key and save its' value
    hash_code = hash_map_pop(collision_hash, "random",  &value);
    EXPECT_EQ(hash_code, KEY_ERROR);
}

TEST_F(CollisionHash, hash_map_remove){
    // Check first item
    ASSERT_NE(collision_hash->first, nullptr);

    // Delete existing key
    hash_map_state_code_t hash_code = hash_map_remove(collision_hash, "klidecek");
    EXPECT_EQ(hash_code, OK);

    // Delete colliding key
    hash_code = hash_map_remove(collision_hash, "kecedilk");
    EXPECT_EQ(hash_code, OK);

    // Delete non-existing key
    hash_code = hash_map_remove(collision_hash, "random");
    EXPECT_EQ(hash_code, KEY_ERROR);
}

/*** Konec souboru white_box_tests.cpp ***/
