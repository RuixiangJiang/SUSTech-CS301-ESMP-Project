#pragma once

#define isNumber(c) ((c) >= '0' && (c) <= '9')

// Stack structure
typedef struct {
    char dataC[20];
    float dataF[20];
    int topC, topF;
} * stackPtr, stack;

// Function to initialize the stack
stackPtr InitStack();

// Function to push a character onto the stack
void pushC(stackPtr head, char c);

// Function to pop a character from the stack
char popC(stackPtr head);

// Function to push a float onto the stack
void pushF(stackPtr head, float c);

// Function to pop a float from the stack
float popF(stackPtr head);

// Function to test the calculator
float CalcExpression(char* Expression);