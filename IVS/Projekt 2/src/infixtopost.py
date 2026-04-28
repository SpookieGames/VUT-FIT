#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""! 
@file infixtopost.py
@brief Conversion of infix expression to a postfix expression
@date 27.3.2026
@author Ha Pham 
"""

from mathlib import *

##
#@brief Function to evaluate operator precedence
#@param operator Operator to be assigned a precendence (value of importance)
#@return Integerer representing  precedence (0-4)
#@exception ValueError If operator is unknown
#
def eval_precedence(operator):
    if operator == "(": 
        return 0
    elif operator in "+-":
        return 1
    elif operator in "*/%":
        return 2
    elif operator == "^":
        return 3
    elif operator in ["ln", "sqt", "fac"]:
        return 4
    else:
        raise ValueError(f"Unknown operator: {operator}")

##
#@brief Function to check if a token is a number = operand
#@return True if operand is a number
#@param operand Token to check
#@return True if operand is a number False otherwise
#
def is_operand(operand):
    try: 
        value = float(operand)
    except ValueError:
        return False
    else:
        return True 

##
#@brief Checks if a token is a valid operator
#@param operator Token to check
#@return True if operator is +,-,*,/,%,^,sqt,fac,ln False otherwise
#
def is_operator(operator):
    if operator in "+-*/^%":
        return True
    elif operator == "sqt" or operator == "fac" or operator == "ln":
        return True
    else:
        return False

##
#@brief Converts infix expression into postfix expression
#@param parse_string Infix expression to be transformed to postfix expression
#@return Expression in postfix form
#@exception Exception If mismatched parentheses or invalid token found
#
def InfixToPostFix(parse_string):
    
    stack_of_operators = []
    output_postfix=""
    tokens = parse_string.split()
    
    #ITERATE EXPRESSION
    for i in tokens:
        
        #IF SYMBOL IS OPERAND - add into output_postfix
        if is_operand(i):
            output_postfix+=i + " "
        
        #IF SYMBOL LEFT PARENTHESIS - append to stack_of_operators
        elif i =="(":
            stack_of_operators.append("(")

        #IF SYMBOL RIGHT PARENTHESIS - keep popping from stack_of_operators and adding to output_postfix until "(" is found and popped, if not found then raise Exception 
        elif i == ")":
            while len(stack_of_operators) > 0 and stack_of_operators[-1] != "(":
                output_postfix+=stack_of_operators[-1] + " "
                stack_of_operators.pop()
            if len(stack_of_operators) == 0:
                raise Exception("No left parenthesis found")           
            
            stack_of_operators.pop()
        
        #IF SYMBOL OPERATOR
        elif is_operator(i):
            
            operator_precedence=eval_precedence(i)

            if len(stack_of_operators) == 0:
                stack_of_operators.append(i)
            
            elif len(stack_of_operators) != 0:
                last_operator_precedence=eval_precedence(stack_of_operators[-1])
                
                if operator_precedence > last_operator_precedence:
                    stack_of_operators.append(i)

                elif operator_precedence <= last_operator_precedence:
                    while len(stack_of_operators) > 0 and (operator_precedence < last_operator_precedence or (operator_precedence == last_operator_precedence and i != "^")):
                        output_postfix+=stack_of_operators[-1] + " "
                        stack_of_operators.pop()
                        if len(stack_of_operators) > 0:
                            last_operator_precedence=eval_precedence(stack_of_operators[-1])
                    stack_of_operators.append(i)
        else:
            raise Exception(f"Invalid token: {i}")
    
    while len(stack_of_operators) > 0:
        popped_operator = stack_of_operators.pop()
        if popped_operator == "(":
            raise Exception("Mismatched parenthesis")
        output_postfix+= popped_operator + " "
    
    return output_postfix

##
#@brief Evaluates a postfix expression
#@param postfix_string String already in postfix form
#@exception TypeError If postfix_string is not a string
#@exception ValueError If not enough operands for operator
#@exception IndexError If stack is empty at end
#@return Result is a float of evaluation
#
def eval_postfix(postfix_string):
    if not isinstance(postfix_string, str):
        raise TypeError
    
    tokens = postfix_string.split()
    operands_stack = []
    

    for token in tokens:

        if is_operand(token):
            operands_stack.append(token)
        
        elif is_operator(token):
            
            match token:
                case "+":
                    if len(operands_stack) < 2:
                        raise ValueError("Not enough operands for +")
                    operand1 = float(operands_stack.pop())
                    operand2 = float(operands_stack.pop())
                    result = add(operand1,operand2)
                    operands_stack.append(result)
                case "-":
                    if len(operands_stack) < 2:
                        raise ValueError("Not enough operands for -")
                    operand1 = float(operands_stack.pop())
                    operand2 = float(operands_stack.pop())
                    result = sub(operand2,operand1)
                    operands_stack.append(result)
                case "*":
                    if len(operands_stack) < 2:
                        raise ValueError("Not enough operands for *")
                    operand1 = float(operands_stack.pop())
                    operand2 = float(operands_stack.pop())
                    result = mul(operand1,operand2)
                    operands_stack.append(result)
                case "/":
                    if len(operands_stack) < 2:
                        raise ValueError("Not enough operands for /")
                    operand1 = float(operands_stack.pop())
                    operand2 = float(operands_stack.pop())
                    result = div(operand2,operand1)
                    operands_stack.append(result)
                case "%":
                    if len(operands_stack) < 2:
                        raise ValueError("Not enough operands for %")
                    operand1 = float(operands_stack.pop())
                    operand2 = float(operands_stack.pop())
                    result = mod(operand2,operand1)
                    operands_stack.append(result)    
                case "^":
                    if len(operands_stack) < 2:
                        raise ValueError("Not enough operands for ^")
                    operand1 = float(operands_stack.pop())
                    operand2 = float(operands_stack.pop())
                    result = pow(operand2,operand1) 
                    operands_stack.append(result)
                case "fac":
                    if len(operands_stack) < 1:
                        raise ValueError("Not enough operands for fac")                    
                    operand_float = float(operands_stack.pop())                    
                    if not operand_float.is_integer():
                        raise TypeError("Factorial is only defined for integers")                        
                    operand1 = int(operand_float)
                    result = fac(operand1)
                    operands_stack.append(result)
                case "ln":
                    if len(operands_stack) < 1:
                        raise ValueError("Not enough operands for ln")
                    operand1 = float(operands_stack.pop())
                    result = ln(operand1)
                    operands_stack.append(result)
                case "sqt":
                    if len(operands_stack) < 2:
                        raise ValueError("Not enough operands for sqt")
                    operand1 = float(operands_stack.pop())
                    operand2 = float(operands_stack.pop())
                    result = sqt(operand1,operand2) 
                    operands_stack.append(result)
    
    final_result = float(operands_stack.pop())
    return final_result
