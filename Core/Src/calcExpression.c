#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calc.h"

#define RetError {\
        int returnType = cal_loc(head);\
        if (returnType){\
            free(head);\
            return (Pair){returnType, 0.0};\
        }\
    }

// Function to perform calculations based on operators
int cal_loc(stackPtr head) {
    if (head->topF < 1 || head->topC < 0) return INVALID_EXPRESSION;
    if (head->dataC[head->topC] == '(') return NO_ERROR;
    float a = popF(head);
    float b = popF(head);
    char c = popC(head);
    float res = 0;
    switch (c) {
        case '^':
            res = pow(b, a);
            break;
        case '*':
            res = a * b;
            break;
        case '/':
            if (a == 0){
                printf("Error: Division by zero.\n");
                return DIVISION_BY_ZERO;
            }
            res = b / a;
            break;
        case '+':
            res = a + b;
            break;
        case '-':
            res = b - a;
            break;
    }
    pushF(head, res);
    return NO_ERROR;
}

// Function to calculate the result of an expression
float calculate(stackPtr head, char *str) {
}

Pair CalcExpression(char* str) {
    stackPtr head = InitStack(head);
    pushC(head, '(');
    for (int i = 0; i <= strlen(str); i++) {
        char temp;
        if (i < strlen(str)) {
            temp = str[i];
        }
        else {
            temp = ')';
        }
        if (isNumber(temp)) {
            int j = i;
            float sum = 0, power = 0;
            while ((isNumber(str[j]) || str[j] == '.') && j < strlen(str)) {
                if (str[j] == '.') {
                    float fraction = 0.1;
                    j++;
                    while (isNumber(str[j]) && j < strlen(str)) {
                        sum = sum + (str[j] - '0') * fraction;
                        fraction /= 10.0;
                        j++;
                    }
                } else {
                    sum = sum * 10 + str[j] - '0';
                    j++;
                }
            }
            if (str[j] == '^' && j < strlen(str) && head->dataC[head->topC] != '^' && str[j + 1] != '(' && str[j + 1] != ')') {
                j++;
                while (isNumber(str[j]) && j < strlen(str)) {
                    power = power * 10 + str[j] - '0';
                    j++;
                }
                if (power * log(sum) > 32 * log(2)) return (Pair){TOO_LARGE_NUMBER, 0};
                sum = pow(sum, power);
            }
            i = j - 1;
            pushF(head, sum);
            if ((head->dataC[head->topC] == '*' || head->dataC[head->topC] == '/') && str[i + 1] != '^') RetError
            if (head->dataC[head->topC] == '^') RetError
        }
        else {
            switch (temp) {
                case '(':
                    pushC(head, temp);
                    break;
                case ')':
                    while (head->dataC[head->topC] != '(') RetError
                    popC(head);
                    if (head->dataC[head->topC] == '^') RetError
                    if (head->dataC[head->topC] == '*' && str[i + 1] != '^') RetError
                    break;
                case '^':
                    pushC(head, temp);
                    break;
                case '*':
                    pushC(head, temp);
                    break;
                case '/':
                    pushC(head, temp);
                    break;
                case '+':
                    pushC(head, temp);
                    break;
                case '-':
                    pushC(head, temp);
                    break;
                default:
                    break;
            }
        }
    }
    return (Pair){NO_ERROR, popF(head)};
}