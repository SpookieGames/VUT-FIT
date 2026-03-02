//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Michal Holeša <xholesm00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Michal Holeša
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
class EmptyTree : public ::testing::Test
{
protected:
    // Vytvorenie prazdneho stromu
    BinaryTree tree;
};

// Vlozenie uzla a overenie ze sa naozaj vytvoril
// Ocakavame TRUE a ukazatel na novy uzol
TEST_F(EmptyTree, InsertNode)
{
    std::pair<bool, Node_t *> result = tree.InsertNode(10);
    EXPECT_TRUE(result.first);
    ASSERT_NE(result.second, nullptr);
    EXPECT_EQ(result.second->key, 10);
}

// Vymazanie neexistujuceho uzla
TEST_F(EmptyTree, DeleteNode)
{
    bool result = tree.DeleteNode(0);
    EXPECT_FALSE(result);
}

// Hladanie uzla ktory neexsituje
TEST_F(EmptyTree, FindNode)
{
    Node_t *node = tree.FindNode(1);
    EXPECT_EQ(node, nullptr);
}

class NonEmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
    // Vytvorenie maleho stromu
    void SetUp() override
    {
        tree.InsertNode(10);
        tree.InsertNode(5);
        tree.InsertNode(15);
    }
};

TEST_F(NonEmptyTree, InsertNode)
{
    // Vlozenie noveho uzla
    // Ocakavame TRUE a ukazatel na novy uzol
    std::pair<bool, Node_t *> result1 = tree.InsertNode(20);
    ASSERT_TRUE(result1.first);
    ASSERT_NE(result1.second, nullptr);
    EXPECT_EQ(result1.second->key, 20);

    // Vlozenie uzla ktory uz existuje
    // Ocakavana navratova hodnota je FALSE a ukazatel na existujuci uzol
    std::pair<bool, Node_t *> result2 = tree.InsertNode(10);
    EXPECT_FALSE(result2.first);
    ASSERT_NE(result2.second, nullptr);
    EXPECT_EQ(result2.second->key, 10);
}

TEST_F(NonEmptyTree, DeleteNode)
{
    EXPECT_TRUE(tree.DeleteNode(5));  // Existujuci uzol
    EXPECT_FALSE(tree.DeleteNode(1)); // Neexistujuci uzol
}

// Hladanie uzla
TEST_F(NonEmptyTree, FindNode)
{
    Node_t *node = tree.FindNode(5); // Navratova hodnota je ukazatel na najdeny uzol
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->key, 5);

    // Hladanie uzla ktory neexsituje
    EXPECT_EQ(tree.FindNode(1), nullptr);
}

class TreeAxioms : public ::testing::Test
{
protected:
    BinaryTree tree;

    void SetUp() override
    {
        tree.InsertNode(2);
        tree.InsertNode(1);
        tree.InsertNode(5);
    }
};

// Vsetky listove uzly su cierne
TEST_F(TreeAxioms, Axiom1)
{
    std::vector<Node_t *> outLeafNodes;
    tree.GetLeafNodes(outLeafNodes);

    for (Node_t *leaf : outLeafNodes)
    {
        EXPECT_EQ(leaf->color, BinaryTree::BLACK); // Porovnanie farby
    }
}

// Ak je uzol cerveny oba jeho potomkovia su cierny
TEST_F(TreeAxioms, Axiom2)
{
    std::vector<Node_t *> outAllNodes;
    tree.GetAllNodes(outAllNodes);

    for (Node_t *leaf : outAllNodes)
    {
        ASSERT_NE(leaf, nullptr);

        if (leaf->color == BinaryTree::RED) // Ak je uzol cerveny tak pokracujeme
        {
            ASSERT_NE(leaf->pLeft, nullptr);
            ASSERT_EQ(leaf->pLeft->color, BinaryTree::BLACK); // Porovnanie farby
            ASSERT_NE(leaf->pRight, nullptr);
            ASSERT_EQ(leaf->pRight->color, BinaryTree::BLACK); // Porovnanie farby
        }
    }
}

// Overenie poctu ciest ku korenu
TEST_F(TreeAxioms, Axiom3)
{
    std::vector<Node_t *> outLeafNodes;
    tree.GetLeafNodes(outLeafNodes);
    int global_count = -1;

    for (Node_t *leaf : outLeafNodes)
    {
        Node_t *current = leaf; // Pomocny ukazatel aby sa neprepisal leaf
        int count = 0;

        while (current != nullptr)
        {
            if (current->color == BinaryTree::BLACK) // Ak je list cierny, tak zvysime count
            {
                count++;
            }

            current = current->pParent; // Posunieme ukazatel dalej
        }

        if (global_count == -1)
        {
            global_count = count;
        }

        else
        {
            EXPECT_EQ(count, global_count);
        }
    }
}
//============================================================================//

/*** Konec souboru black_box_tests.cpp ***/
