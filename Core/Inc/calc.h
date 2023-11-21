#ifndef __CALC_H
#define __CALC_H

// Stack structure
typedef struct {
    char dataC[20];
    float dataF[20];
    int topC, topF;
} * stackPtr, stack;

// Function to initialize the stack
stackPtr InitStack() {
    stackPtr head = (stackPtr)malloc(sizeof(stack));
    head->topC = -1;
    head->topF = -1;
    return head;
}

// Function to push a character onto the stack
void pushC(stackPtr head, char c) {
    if (head->topC >= 18) {
        printf("No space for characters in the stack.\n");
        return;
    }
    head->topC++;
    head->dataC[head->topC] = c;
}

// Function to pop a character from the stack
char popC(stackPtr head) {
    char tempchar = head->dataC[head->topC];
    head->topC--;
    return tempchar;
}

// Function to push a float onto the stack
void pushF(stackPtr head, float c) {
    if (head->topF >= 18) {
        printf("No space for floats in the stack.\n");
        return;
    }
    head->topF++;
    head->dataF[head->topF] = c;
}

// Function to pop a float from the stack
float popF(stackPtr head) {
    float tempfloat = head->dataF[head->topF];
    head->topF--;
    return tempfloat;
}

float CalcExpression(char* Expression);

#endif