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
    EXPECT_EQ(empty.size(), 2); // Mala by sa zvacsit velkost o 1
}

TEST_F(SuffixAutomatonTest, ClearAutomaton)
{
    abc.clear();

    // Po vycisteni ma byt automat prazdny
    EXPECT_EQ(abc.size(), 1);
    EXPECT_FALSE(abc.contains("a"));
}

TEST_F(SuffixAutomatonTest, ComplexSequence)
{
    SuffixAutomaton complex;
    complex.add_sequence("pododdiel"); // Komplexnejsie slovo s opakovanymi pismenami

    EXPECT_TRUE(complex.contains("pod"));
    EXPECT_TRUE(complex.contains("odd"));
    EXPECT_TRUE(complex.contains("iel"));
    EXPECT_TRUE(complex.contains("dod"));

    EXPECT_FALSE(complex.contains("odie"));
}

TEST_F(SuffixAutomatonTest, GetState)
{
    const State &test = abc.get_state(0);

    EXPECT_EQ(test.len, 0);
    EXPECT_FALSE(test.link.has_value());

    const State &test2 = abc.get_state(1);

    EXPECT_EQ(test2.len, 1);
    EXPECT_TRUE(test2.link.has_value());
}

TEST_F(SuffixAutomatonTest, StepFunction)
{
    size_t next_state;

    // Vieme ist cez 'a'
    EXPECT_TRUE(abc.step(0, 'a', next_state));
    // Ale nie cez 'x'
    EXPECT_FALSE(abc.step(0, 'x', next_state));
}

TEST_F(SuffixAutomatonTest, NextFunction)
{
    // Pre 'abc' by mali byt v stave 0 3 prechody ('a', 'b', 'c')
    auto map = abc.next(0);

    EXPECT_EQ(map.size(), 3);
}
// sort

//============================================================================//

/*** Konec souboru white_box_tests.cpp ***/
