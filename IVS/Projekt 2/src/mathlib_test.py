#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""!
@file mathlib_test.py
@brief Math library Tests for IVS calculator 2026, implementation of tests for calculator library
@date 12.3.2026
@author Ha Pham, Kristian Duzek 
"""

from mathlib import *
import pytest 

##
#@brief Tests addition function
#
def test_add():
    assert add(0,0) == 0
    assert add(0,5) == 5
    assert add(-1,-1) == -2
    assert add(-4, 2) == -2
    assert add(0.1,0.2) == pytest.approx(0.3)
    assert add(-0.1, 0.2) == pytest.approx(0.1)
    assert add(532169, 2350789) == 2882958

##
#@brief Tests division function
#
def test_div():
    with pytest.raises(ZeroDivisionError):
        div(1,0)
    assert div(0,1) == 0
    assert div(1, 0.2) == pytest.approx(5)
    assert div(5,-1) == -5
    assert div(-5,1) == -5
    assert div(-30, -5) == 6
    assert div(50,50) == 1
    assert div(50,-50) == -1

##
#@brief Tests subtraction function
#
def test_sub():
    assert sub(0,0) == 0
    assert sub(-5,0) == -5
    assert sub(-10,-5) == -5
    assert sub(-10, 5) == -15
    assert sub(4,-2) == 6
    assert sub(8,4) == 4
    assert sub(0,3) == -3
    assert sub(-1000000000000000000000000,1) == -1000000000000000000000001
    assert sub(-1000000000000000000000000,-1000000000000000000000000) == 0

##
#@brief Tests multiplication function
#
def test_mul():
    assert mul(2,2) == 4
    assert mul(0,0) == 0
    assert mul(0,1) == 0
    assert mul(1,0) == 0
    assert mul(10000000000000000000,0) == 0
    assert mul(10000000000000000000,1) == 10000000000000000000
    assert mul(-1, -10) == 10
    assert mul(20, -10) == -200
    assert mul(0.1, 0.1) == pytest.approx(0.01)
    assert mul(-0.2, 0.4) == pytest.approx(-0.08)

##
#@brief Tests power function
#
def test_pow():
    assert pow(2,4) == 16
    assert pow(0,0) == 1
    assert pow(4,0) == 1
    assert pow(0, 24) == 0
    assert pow(1, 325) == 1
    assert pow(-3, 2) == 9
    assert pow(0.5, 2) == pytest.approx(0.25)
    assert pow(9, 0.5) == pytest.approx(3)
    assert pow(-2, 3) == -8
    assert pow(2, -1) == 0.5

##
#@brief Tests root function 
#
def test_sqt():
    assert sqt(9,2) == 3
    with pytest.raises (ValueError):
        sqt(-9,2)
    with pytest.raises (ValueError):
        sqt(9,0)
    assert sqt (8,3) == 2
    assert sqt (0,3) == 0
    with pytest.raises (ValueError):
        sqt(64,0) 
    assert sqt (-8,3) == -2
    assert sqt(16,0.5) == pytest.approx(256)
    assert sqt(5,1) == 5
    assert sqt(4, -2) == pytest.approx(0.5)

##
#@brief Tests factorial function 
#
def test_fac():
    assert fac(1) == 1
    assert fac(2) == 2
    assert fac(5) == 120
    with pytest.raises (ValueError):
        fac(-1)
    with pytest.raises (TypeError):
        fac("abc")

##
#@brief Tests natural log function 
#
def test_ln():
    assert ln(math.e) == pytest.approx(1)
    with pytest.raises(ValueError):
        ln(-1)
    with pytest.raises(ValueError):
        ln(0)
    assert ln(0.11111111) == -2.1972245873362195
    assert ln(2) == 0.6931471805599453
    assert ln (3.5) == 1.252762968495368

##
#@brief Tests modulo function
#
def test_mod():
    assert mod(5, 2) == 1
    assert mod(10, 5) == 0
    assert mod(14, 3) == 2
    
    assert mod(0, 5) == 0
    
    with pytest.raises(ZeroDivisionError):
        mod(5, 0)
    with pytest.raises(ZeroDivisionError):
        mod(0, 0)
        
    assert mod(-5, 2) == 1
    assert mod(5, -2) == -1
    assert mod(-5, -2) == -1
    
    assert mod(5.5, 2) == 1.5
    assert mod(5, 1.5) == 0.5
