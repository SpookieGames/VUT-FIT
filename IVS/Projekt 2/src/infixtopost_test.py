#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""! 
@file infixtopost_test.py
@brief Tests for infix to postfix conversion for IVS calculator 2026
@date 29.3.2026
@author Kristian Duzek 
"""

from infixtopost import *
import pytest


##
#@brief Tests operator precedence evaluation
#
def test_eval_precedence ():
    assert eval_precedence("(") == 0
    assert eval_precedence("+") == 1
    assert eval_precedence("-") == 1
    assert eval_precedence("*") == 2
    assert eval_precedence("/") == 2
    assert eval_precedence("%") == 2
    assert eval_precedence("^") == 3
    
    with pytest.raises(ValueError):
        eval_precedence("£")
    with pytest.raises(ValueError):
        eval_precedence("#")
    with pytest.raises(ValueError):
        eval_precedence("$")


##
#@brief Tests operand detection (numbers vs non-numbers)
#
def test_is_operand ():
    assert is_operand("1") == True
    assert is_operand("0") == True
    assert is_operand("-1")== True
    assert is_operand("-2") == True
    assert is_operand("1000000000") == True
    assert is_operand("-1000000000") == True
    assert is_operand("-1.1") == True
    assert is_operand("2.57893") == True
    assert is_operand("0.999999") == True
    assert is_operand("-0.99999") == True
    assert is_operand("123124.213124") == True

    assert is_operand("%") == False
    assert is_operand("+") == False
    assert is_operand("-") == False
    assert is_operand("*") == False
    assert is_operand("/") == False
    assert is_operand("^") == False


##
#@brief Tests operator detection
#
def test_is_operator ():
    assert is_operator("%") == True
    assert is_operator("+") == True
    assert is_operator("-") == True
    assert is_operator("*") == True
    assert is_operator("/") == True
    assert is_operator("^") == True

    assert is_operator("1") == False
    assert is_operator("1000000") == False
    assert is_operator("-2.9248") == False
    assert is_operator("$") == False
    assert is_operator("£") == False
    assert is_operator("&") == False


##
# @brief Tests infix to postfix conversion including precedence and parentheses
#
def test_infixtopostfix():
    assert InfixToPostFix("5") == "5 "
    assert InfixToPostFix("3 + 4") == "3 4 + "
    assert InfixToPostFix("3 * 4") == "3 4 * "
    assert InfixToPostFix("3 + 4 * 2") == "3 4 2 * + "
    assert InfixToPostFix("8 / 4 * 2") == "8 4 / 2 * "
    assert InfixToPostFix("8 / 4 / 2") == "8 4 / 2 / "
    assert InfixToPostFix("( 3 + 4 ) * 2") == "3 4 + 2 * "
    assert InfixToPostFix("( ( 1 + 2 ) * 3 )") == "1 2 + 3 * "
    assert InfixToPostFix("3 + ( 4 * 2 )") == "3 4 2 * + "
    assert InfixToPostFix("3 + 4 * 2 / ( 1 - 5 )") == "3 4 2 * 1 5 - / + "
    assert InfixToPostFix("1 + 2 * 3 - 4") == "1 2 3 * + 4 - "
    assert InfixToPostFix("2 ^ 3") == "2 3 ^ "
    assert InfixToPostFix("2 + 3 ^ 2") == "2 3 2 ^ + "
    assert InfixToPostFix("") == ""
    assert InfixToPostFix("( 1 )") == "1 "

    with pytest.raises(Exception):
        InfixToPostFix("3 + £")
    with pytest.raises(Exception):
        InfixToPostFix("( 3 + 4")
    with pytest.raises(Exception):
        InfixToPostFix("3 + 4 )")
    with pytest.raises(Exception):
        InfixToPostFix("( ( 3 + 4 )")
    with pytest.raises(Exception):
        InfixToPostFix("( 3 + ( 4 * 2 )")
    with pytest.raises(Exception):
        InfixToPostFix("3 + ( 4 * 2 ))")


##
# @brief Tests postfix expression evaluation including arithmetic operations and errors
#
def test_eval_postfix():
    assert eval_postfix("5") == 5.0
    assert eval_postfix("3 4 +") == 7.0
    assert eval_postfix("3 4 *") == 12.0
    assert eval_postfix("3 4 2 * +") == 11.0
    assert eval_postfix("8 4 / 2 *") == 4.0
    assert eval_postfix("8 4 / 2 /") == 1.0
    assert eval_postfix("3 4 + 2 *") == 14.0
    assert eval_postfix("1 2 + 3 *") == 9.0
    assert eval_postfix("3 4 2 * 1 5 - / +") == 1.0
    assert eval_postfix("1 2 3 * + 4 -") == 3.0
    assert eval_postfix("-10.2 2 /") == -5.1


    with pytest.raises(Exception):
        eval_postfix("3 +")
    with pytest.raises(Exception):
        eval_postfix("3 4 + +")
    with pytest.raises(Exception):
        eval_postfix("+")
    with pytest.raises(Exception):
        eval_postfix("3 a +")