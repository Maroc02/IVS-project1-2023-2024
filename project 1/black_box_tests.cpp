//======= Copyright (c) 2024, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Marek Čupr <xcuprm01@stud.fit.vutbr.cz>
// $Date:       $2024-02-17
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Marek Čupr
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>
#include "gtest/gtest.h"
#include "red_black_tree.h"

using namespace testing;

// Create an empty tree
class EmptyTree : public Test
{
protected:
    BinaryTree *empty_tree;

    // Allocate the memory
    void SetUp() override {
        empty_tree = new BinaryTree();
    }

    // Free the memory
    void TearDown() override {
        delete empty_tree;
     }
};

// Create a non-empty tree
class NonEmptyTree : public Test
{
protected:
    BinaryTree *non_empty_tree;

    // Allocate the memory
    void SetUp() override {
        non_empty_tree = new BinaryTree();

        // Insert nodes to the tree
        for (int i = 11; i < 1100; i += 11)
            non_empty_tree->InsertNode(i);
    }

    // Free the memory
    void TearDown() override {
        delete non_empty_tree;
    }
};

// Create a non-empty tree
class TreeAxioms : public Test
{
protected:
    BinaryTree *tree_axioms;

    // Allocate the memory
    void SetUp() override {
        tree_axioms = new BinaryTree();

        // Generate random key values
        for (int i = 11; i < 1100; i += 11)
            tree_axioms->InsertNode(i);
    }

    // Free the memory
    void TearDown() override {
        delete tree_axioms;
    }
};

/* ********************* */
/* ****  EMPTY TREE **** */
/* ********************* */
TEST_F(EmptyTree, InsertNode){
    // Insert random node with high key value
    ASSERT_NO_THROW(empty_tree->InsertNode(999999999));

    // Insert random node
    auto insert_node = empty_tree->InsertNode(69);
    EXPECT_TRUE(insert_node.first);
    EXPECT_EQ(insert_node.second->key, 69);

    // Insert already existing node
    insert_node = empty_tree->InsertNode(69);
    EXPECT_FALSE(insert_node.first);
    EXPECT_EQ(insert_node.second->key, 69);

    // Insert many random nodes
    const std::vector<int> keys = {1,11,111,1111,11111};
    std::vector<std::pair<bool, Node_t*>> insert_nodes;

    empty_tree->InsertNodes(keys, insert_nodes);
    for (int i = 0; i < insert_nodes.size(); i++){
        EXPECT_TRUE(insert_nodes[i].first);
        ASSERT_EQ(keys[i], insert_nodes[i].second->key);
    }
}

TEST_F(EmptyTree, DeleteNode){
    // Delete non-existent node
    auto deleted_node = empty_tree->DeleteNode(69);
    EXPECT_FALSE(deleted_node);

    // Delete non-existent node
    deleted_node = empty_tree->DeleteNode(420);
    EXPECT_FALSE(deleted_node);
}


TEST_F(EmptyTree, FindNode){
    // Find non-existent node
    auto searched_node = empty_tree->FindNode(69);
    EXPECT_EQ(searched_node, nullptr);

    // Find non-existent node
    searched_node = empty_tree->FindNode(420);
    EXPECT_EQ(searched_node, nullptr);
}

/* ************************* */
/* ****  NON-EMPTY TREE **** */
/* ************************* */
TEST_F(NonEmptyTree, InsertNode){
    // Insert random node with high key value
    ASSERT_NO_THROW(non_empty_tree->InsertNode(999999999));

    // Insert random node
    auto inserted_node = non_empty_tree->InsertNode(10);
    EXPECT_EQ(inserted_node.second->key, 10);
    EXPECT_TRUE(inserted_node.first);

    // Insert random node
    inserted_node = non_empty_tree->InsertNode(102);
    ASSERT_EQ(inserted_node.second->key, 102);
    ASSERT_TRUE(inserted_node.first);

    // Insert existing node
    inserted_node = non_empty_tree->InsertNode(11);
    EXPECT_EQ(inserted_node.second->key, 11);
    EXPECT_FALSE(inserted_node.first);

    // Insert existing node
    inserted_node = non_empty_tree->InsertNode(22);
    EXPECT_EQ(inserted_node.second->key, 22);
    ASSERT_FALSE(inserted_node.first);
}

TEST_F(NonEmptyTree, DeleteNode){
    // Delete existing node
    auto deleted_node = non_empty_tree->DeleteNode(11);
    EXPECT_TRUE(deleted_node);

    // Delete existing node
    deleted_node = non_empty_tree->DeleteNode(44);
    ASSERT_TRUE(deleted_node);

    // Delete non-existing node
    deleted_node = non_empty_tree->DeleteNode(69);
    EXPECT_FALSE(deleted_node);

    // Delete non-existing node
    deleted_node = non_empty_tree->DeleteNode(99999);
    ASSERT_FALSE(deleted_node);
}

TEST_F(NonEmptyTree, FindNode){
    // Find existing node
    auto searched_node = non_empty_tree->FindNode(11);
    EXPECT_EQ(searched_node->key, 11);

    // Find existing node
    searched_node = non_empty_tree->FindNode(33);
    ASSERT_EQ(searched_node->key, 33);

    // Find non-existing node
    searched_node = non_empty_tree->FindNode(69);
    EXPECT_EQ(searched_node, nullptr);

    // Find non-existing node
    searched_node = non_empty_tree->FindNode(69);
    EXPECT_EQ(searched_node, nullptr);
}

/* ********************** */
/* ****  TREE AXIOMS **** */
/* ********************** */
TEST_F(TreeAxioms, Axiom1){
    std::vector<Node_t *> outLeafNodes;
    tree_axioms->GetLeafNodes(outLeafNodes);

    // Test if all leafs are black and have no child nodes
    for (Node_t* leafNode : outLeafNodes){
        ASSERT_TRUE(leafNode->pLeft == NULL);
        ASSERT_TRUE(leafNode->pRight == NULL);
        EXPECT_EQ(leafNode->color, BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2){
    std::vector<Node_t *> allNodes;
    tree_axioms->GetAllNodes(allNodes);

    // Test if all child nodes exist and are black
    for (Node_t* node : allNodes){
        if (node->color == RED){
            ASSERT_NE(node->pLeft, nullptr);
            EXPECT_EQ(node->pLeft->color, BLACK);
            ASSERT_NE(node->pRight, nullptr);
            EXPECT_EQ(node->pRight->color, BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3){
    std::vector<Node_t *> outLeafNodes;
    tree_axioms->GetLeafNodes(outLeafNodes);

    // Track count of black nodes from leaf nodes to the root 
    std::vector<int> distance;
    int black_nodes;

    // Loop through all leaf nodes
    for (Node_t* node : outLeafNodes){
        black_nodes = 0;
        // Go from leaf nodes to the root
        while (node != tree_axioms->GetRoot()){
            if (node->color == BLACK)
                black_nodes++;

            ASSERT_NE(node->pParent, nullptr);
            node = node->pParent;
        }
        // Add black nodes count to vector
        distance.push_back(black_nodes);
    }

    // Check if black nodes count is the same
    for (int i = 0; i < distance.size(); i++){
        for (int j = i; j < distance.size(); j++){
            EXPECT_EQ(distance[i], distance[j]);
        }
    }
}

/*** Konec souboru black_box_tests.cpp ***/
