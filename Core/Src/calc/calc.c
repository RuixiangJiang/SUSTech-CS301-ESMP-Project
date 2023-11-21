// Modified from:
// https://blog.csdn.net/qq_60678811/article/details/124762176

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../Inc/calc.h"

// Function to check if a character is a number
int isNumber(char c) {
    return (c >= '0' && c <= '9');
}

// Function to perform calculations based on operators
void cal_loc(stackPtr head) {
    if (head->dataC[head->topC] == '(') return;
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
}

// Function to calculate the result of an expression
float calculate(stackPtr head, char *str) {
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
                sum = pow(sum, power);
            }
            i = j - 1;
            pushF(head, sum);
            if ((head->dataC[head->topC] == '*' || head->dataC[head->topC] == '/') && str[i + 1] != '^') cal_loc(head);
            if (head->dataC[head->topC] == '^') cal_loc(head);
        }
        else {
            switch (temp) {
                case '(':
                    pushC(head, temp);
                    break;
                case ')':
                    while (head->dataC[head->topC] != '(') cal_loc(head);
                    popC(head);
                    if (head->dataC[head->topC] == '^') cal_loc(head);
                    if (head->dataC[head->topC] == '*' && str[i + 1] != '^') cal_loc(head);
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
            }
        }
    }
    return popF(head);
}

// Function to test the calculator
float CalcExpression(char* Expression) {
    stackPtr head = InitStack(head);
    pushC(head, '(');
    return calculate(head, Expression);
}

int main() {
    char tempExpression[100];
    printf("Enter the expression: ");
    fgets(tempExpression, sizeof(tempExpression), stdin);

    printf("The result = %.2f\n", CalcExpression(tempExpression));
    return 0;
}