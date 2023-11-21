#pragma once

#define isNumber(c) ((c) >= '0' && (c) <= '9')

// Error types
#define NO_ERROR 0
#define DIVISION_BY_ZERO 1
#define INVALID_EQUATION 2
#define INVALID_EXPRESSION 3

// Stack structure
#define stackDepth 80
typedef struct {
    char dataC[stackDepth];
    float dataF[stackDepth];
    int topC, topF;
} * stackPtr, stack;

// Pair structure
typedef struct {
    int errorCode;
    float result;
} Pair;

typedef struct {
    int rootNum;
    float root1;
    float root2;
} Root;

typedef struct {
    int errorCode;
    Root root;
} Pair2;

typedef struct {
    int inputType;
    Pair p1;
    Pair2 p2;
} calcPair;

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
 
// Function to calculate an expression
Pair CalcExpression(char* Expression);

// Function to solve an equation
Pair2 SolveEquation(char* str);

calcPair CalcOrSolve(char *str);