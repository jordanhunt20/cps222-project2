/*
 * expression.cc - Implementation of a class representing simple arithmetic
 *                 expressions, as declared in expression.h.  Part of CPS222
 *                 Project 2.
 *
 * Skeleton copyright (c) 2001, 2013 - Russell C. Bjork
 *
 */

#include "expression.h"
#include <stack>
#include <iostream>
#include <string>

/**
 * Get precedence of an operator
 * Inspired by https://gist.github.com/mycodeschool/7867739
 * @param c Operator (+, -, *, /, (, ))
 */
int getPrecedence(char c) {
    if ( c == '-' || c == '+' ) {
        return 1;
	} else if ( c == '*' || c == '/' ){
        return 2;
    } else if ( c == '(') {
        return 0;
    } else if (c == '#') {
        return 3;
    } else {
        return -1;
    }
}

/**
 * Check if a char is an operator
 * @param c Any character
 */
bool isOperator(char c) {
    std::string operators = "+-*/";
    return operators.find(c) != std::string::npos;
}

/**
 * Check if a char is an integer
 * @param c Any character
 */
bool isInt( char c ) {
    std::string operators = "1234567890";
    return operators.find(c) != std::string::npos;
}

/**
 * Convert an infix expression to postfix notation
 * @param infix Expression in infix notation, ex: 2 + 3 + 4 = 2 3 + 4 +
 */
std::string Expression::convertToPostfix(std::string infix) const throw (SyntaxError) {
    enum Character { OPERAND, OPERATOR };
    Character charExpected = OPERAND;
    std::string postfix = "";
    std::stack<char> operators;
    int index = 0;
    for (char& c : infix) {

        // char c is an operator
        if (isOperator (c)) {
            bool push = true;

            if (charExpected == OPERATOR) {
                charExpected = OPERAND;
            } else if (c == '-') {
                c = '#'; // Represent negation with pound symbol
            } else {
                throw SyntaxError(index, "Operand expected");
            }

            // Add any operators of higher precedence to the postfix output
            while (!operators.empty() &&
                    getPrecedence(operators.top()) >= getPrecedence(c) && c != '#') {
                postfix += operators.top();
                operators.pop();
            }

            // Add current operator to the stack
            if (push) {
                operators.push(c);
            }

        // char c is (
        } else if (c == '(') {

            if (charExpected == OPERATOR) {
                throw SyntaxError(index, "Operator expected");
            }

            operators.push(c);

        // char c is )
        } else if (c == ')') {

            if (charExpected == OPERAND) {
                throw SyntaxError(index, "Operand expected");
            } else if (operators.empty()) {
                throw SyntaxError(index, "( expected");
            }

            // Add operators to postfix output until opening parenthesis is found
            while (operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
                if (operators.empty()) {
                    throw SyntaxError(index, "( expected");
                }
            }

            operators.pop();

        // char c is an integer
        } else if (isInt (c)) {
            if (charExpected == OPERAND) {
                charExpected = OPERATOR;
            } else {
                throw SyntaxError(index, "Operator expected");
            }
            postfix += c;

        // char c is an invalid character
        } else {
            throw SyntaxError(index, "Invalid character");
        }

        index++;
    }

    // Add remaining operators to the postfix output
    while (!operators.empty()) {

        if (charExpected == OPERAND) {
            throw SyntaxError(index, "Operand expected");
        } else if (operators.top() == '(') {
            throw SyntaxError(index, ") expected");
        }

        postfix += operators.top();
        operators.pop();
    }

    return postfix;
}

/**
 * Do math
 * @param op Operation (+, -, *, /)
 * @param a  First number
 * @param b  Second number
 */
int doMath(char op, int a, int b) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
    return 0;
}

/**
 * Evaluate a postfix expression
 * @param postfix Expression in postfix notation, ex: 2 3 + 4 + = 2 + 3 + 4
 */
int Expression::evaluate(std::string postfix) const throw (DivideByZeroError) {
    std::stack<int> evaluation;
    int index = 0;
    for (char& c : postfix) {

        if (c == '#') {
            evaluation.top() *= -1;
        } else if (!isOperator(c)) {
            evaluation.push(c - '0'); // Convert char to int
        } else {
            int temp = evaluation.top();
            evaluation.pop();
            if (c == '/' && temp == 0) {
                throw DivideByZeroError(index);
            }
            int result = doMath(c, evaluation.top(), temp);
            evaluation.pop();
            evaluation.push(result);
        }
        index++;
    }

    return evaluation.top();
}

/**
 * Convert a postfix expression to prefix notation
 * @param postfix Expression in postfix notation, ex: 2 3 + 4 + = + + 2 3 4
 */
std::string Expression::convertToPrefix(std::string postfix) const {

    std::string prefix = "";
    char lastChar = postfix[postfix.length() - 1];

    if (isInt(lastChar)) {
        return postfix;
    } else if (lastChar == '#') {
        return lastChar + convertToPrefix(postfix.substr(0, postfix.length()-1));
    } else {
        int operandsNeeded = 1;
        int index = postfix.length() - 1;

        while (index >= 0 && operandsNeeded > 0) {
            index--;
            if (isInt(postfix[index])) {
                operandsNeeded--;
            } else if (isOperator(postfix[index])) {
                operandsNeeded++;
            }
        }

        prefix += lastChar;
        prefix += convertToPrefix(postfix.substr(0, index));
        prefix += convertToPrefix(postfix.substr(index, (postfix.length()-1)-(index)));
    }

    return prefix;
}
