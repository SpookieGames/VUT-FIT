//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test
{
protected:
    BinaryTree tree;
};

TEST_F(EmptyTree, InsertNode)
{
    std::pair<bool, Node_t *> result = tree.InsertNode(10);
    EXPECT_TRUE(result.first);
    ASSERT_NE(result.second, nullptr);
    EXPECT_EQ(result.second->key, 10);
}

TEST_F(EmptyTree, DeleteNode)
{
    bool result = tree.DeleteNode(0);
    EXPECT_FALSE(result);
}

TEST_F(EmptyTree, FindNode)
{
    Node_t *node = tree.FindNode(10);
    EXPECT_EQ(node, nullptr);
}

/*** Konec souboru black_box_tests.cpp ***/
