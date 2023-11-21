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
    if (head->topC >= 18) {
        printf("No space for characters in the stack.\n");
        return;
    }
    head->topC++;
    head->dataC[head->topC] = c;
}

char popC(stackPtr head) {
    char tempchar = head->dataC[head->topC];
    head->topC--;
    return tempchar;
}

void pushF(stackPtr head, float c) {
    if (head->topF >= 18) {
        printf("No space for floats in the stack.\n");
        return;
    }
    head->topF++;
    head->dataF[head->topF] = c;
}

float popF(stackPtr head) {
    float tempfloat = head->dataF[head->topF];
    head->topF--;
    return tempfloat;
}