/*
 * project2.cc - Main program for CPS222 project 2
 *
 * Repeatedly read infix expressions from standard input (one per line)
 * and echo each and print out various information.
 *
 * Copyright (c) 2001, 2003, 2013 - Russell C. Bjork
 *
 * Modified by Adam Vigneaux and Jordan Hunt
 */

#include <iostream>
#include "expression.h"

// Write out an expression with an extra space between characters
static void writeExpression(std::string expression) {
    for (unsigned long i = 0; i < expression.length(); i ++) {
        if (i != 0) {
            std::cout << " ";
        }
        std::cout << expression[i];
    }
}

// Write out an error marker carat (^) at specified place
static void writeErrorMarker(int position) {
    for (int i = 0; i < position; i ++) {
        std::cout << "  ";
    }
    std::cout << "^";
}

int main() {
    std::string infix, postfix, prefix;
    getline(std::cin, infix);
    while (! std::cin.eof()) {
        try {
            Expression theExpression(infix);

            std::cout << "Infix:   ";
            writeExpression(theExpression.getInfix());
            std::cout << std::endl;

            postfix = theExpression.getPostfix();
            if (postfix.length() > 0) {
                std::cout << "Postfix: ";
                writeExpression(postfix);
                std::cout << std::endl;

                int value = theExpression.getValue();
                std::cout << "Value:   " << value << std::endl;

                prefix = theExpression.getPrefix();
                if (prefix.length() > 0) {
                    std::cout << "Prefix:  ";
                    writeExpression(prefix);
                    std::cout << std::endl;
                }
            }
        } catch (SyntaxError e) {
            std::cout << "         ";
            writeErrorMarker(e.getPosition());
            std::cout << e.getDescription() << std::endl;
        } catch (DivideByZeroError e) {
            std::cout << "         ";
            writeErrorMarker(e.getPosition());
            std::cout << "Division by Zero" << std::endl;
        }

        std::cout << std::endl;

        getline(std::cin, infix);
    }
}
