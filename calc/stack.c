#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../Inc/calc.h"

stackPtr InitStack() {
    stackPtr head = (stackPtr)malloc(sizeof(stack));
    head->topC = -1;
    head->topF = -1;
    return head;
}

void pushC(stackPtr head, char c) {
    if (head->topC >= stackDepth - 1) {
        printf("No space for characters in the stack.\n");
        return;
    }
    head->dataC[++head->topC] = c;
}

char popC(stackPtr head) {
    return head->dataC[head->topC--];
}

void pushF(stackPtr head, float c) {
    if (head->topF >= stackDepth - 1) {
        printf("No space for floats in the stack.\n");
        return;
    }
    head->dataF[++head->topF] = c;
}

float popF(stackPtr head) {
    return head->dataF[head->topF--];
}