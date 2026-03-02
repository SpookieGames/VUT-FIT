//======= Copyright (c) 2026, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     Michal Holeša <xholesm00@stud.fit.vutbr.cz>
// $Date:       $2025-02-19
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Michal Holeša
 *
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v
//       indexu

class SuffixAutomatonTest : public ::testing::Test
{
protected:
    SuffixAutomaton empty;
    SuffixAutomaton abc;

    void SetUp() override
    {
        abc = SuffixAutomaton("abc");
    }
};

TEST_F(SuffixAutomatonTest, EmptyAutomaton)
{
    EXPECT_EQ(empty.size(), 1); // Pociatocta velkost je 1

    EXPECT_TRUE(empty.contains(""));
    EXPECT_FALSE(empty.contains("a"));
}

// Overenie ci retazec obsahuje vsetky podretazce
TEST_F(SuffixAutomatonTest, Contains)
{
    EXPECT_TRUE(abc.contains("a"));
    EXPECT_TRUE(abc.contains("b"));
    EXPECT_TRUE(abc.contains("c"));
    EXPECT_TRUE(abc.contains("ab"));
    EXPECT_TRUE(abc.contains("bc"));
    EXPECT_TRUE(abc.contains("abc"));

    EXPECT_FALSE(abc.contains("cb"));
    EXPECT_FALSE(abc.contains("abcd"));
    EXPECT_FALSE(abc.contains("x"));
}

TEST_F(SuffixAutomatonTest, AddElement)
{
    empty.add_element('a');
    EXPECT_TRUE(empty.contains("a"));
    EXPECT_EQ(empty.size(), 2); // pribudol stav
}
//============================================================================//

/*** Konec souboru white_box_tests.cpp ***/
