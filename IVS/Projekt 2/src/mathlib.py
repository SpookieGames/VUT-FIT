#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""!
@file mathlib.py
@brief Math library for IVS calculator 2026, implemenation of calculator functions
@date 12.3.2026
@author: Ha Pham, Kristian Duzek 
"""

import math

##
#@brief Adds two numbers
#@return Sum of two numbers provided
#@param a addend
#@param b addend
#
def add(a,b):
    return a+b

##
#@brief Subtracts two numbers
#@return Subtraction of two numbers provided
#@param a minuend
#@param b subtrahend
#
def sub(a,b):
    return a-b

##
#@brief Multiplies two numbers
#@return Multiplication of two numbers provided
#@param a multiplicant
#@param b multiplier
#
def mul(a,b):
    return a*b

##
#@brief Divides two numbers
#@return Division of two numbers provided
#@param a numerator
#@param b denominator
#@exception ZeroDivisionError in case b == 0
#
def div(a,b):
    if b == 0:
        raise ZeroDivisionError
    else:
        return a/b

##
#@brief Calculates the modulo of two numbers
#@return Remainder of the division of a by b
#@param a dividend
#@param b divisor
#@exception ZeroDivisionError in case b == 0
#
def mod(a,b):
    if b == 0:
        raise ZeroDivisionError
    else:
        return a % b

##
#@brief Calculates power
#@return Power of the base to the exponent
#@param a base
#@param b exponent
#
def pow(a,b):
    if b == 2:
        return a*a
    return math.pow(a,b)

##
# @brief      Calculates the root of a number
# @param      a the number
# @param      b the degree of the root
# @exception  ValueError if b == 0
# @exception  ValueError if a < 0 and b is even
# @return     b-th root of a
#
def sqt(a,b):
    if b == 0:
        raise ValueError("Root degree cannot be zero")
    if a < 0 and b % 2 == 0:
        raise ValueError("Even root of negative number is not real")
    
    if b == 2:
        return math.sqrt(a)
    if a < 0 and b % 2 != 0:
        return -((-a) ** (1/b))

    return a ** (1 / b)

##
# @brief      Calculates factorial of a non-negative integer
# @param      n the input number (non-negative integer)
# @exception  ValueError if n is negative
# @exception  TypeError if n is not an integer
# @return     Factorial of n
#
def fac(n):
    if not isinstance(n, int):
        raise TypeError("n must be an integer")
    if n < 0:
        raise ValueError("Factorial is not defined for negative numbers")
    
    return math.factorial(n)

##
# @brief      Calculates natural log 
# @param      a the input number (non-negative integer including zero)
# @exception  ValueError if a is negative or zero
# @return     natural log of a
#
def ln(a):
    if a <= 0:
        raise (ValueError)("a cannot be less than 0 OR == 0")
    return math.log(a)