// Modified from:
// https://blog.csdn.net/qq_60678811/article/details/124762176

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../Inc/calc.h"

Pair CalcOrSolve(char *str){
    if (strstr(str, "=") != NULL) {
        return SolveEquation(str);
    }
    else {
        return CalcExpression(str);
    }
}
int main() {
    char tempExpression[100];
    printf("Enter the expression: ");
    fgets(tempExpression, sizeof(tempExpression), stdin);

    printf("The result = %.2f, the errcode is %d\n", CalcExpression(tempExpression).result, CalcExpression(tempExpression).errorCode);
    return 0;
}