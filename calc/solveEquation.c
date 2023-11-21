#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <math.h>
#include "../../Inc/calc.h"

#define LeftOfEqual (i - 1 < posEqual ? 1 : -1)

// Function to solve linear equations
Pair2 solveLinearEquation(float b, float c) {
    if (b == 0) {
        return (Pair2){DIVISION_BY_ZERO, (Root){0, 0, 0}};
    }
    return (Pair2){NO_ERROR, (Root){1, -c / b, 0}};
}

// Function to solve quadratic equations
Pair2 solveQuadraticEquation(float a, float b, float c) {
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return (Pair2){NO_ERROR, (Root){0, 0, 0}}; // No real roots
    } else {
        float root1 = (-b + sqrt(discriminant)) / (2 * a);
        float root2 = (-b - sqrt(discriminant)) / (2 * a);
        return (Pair2){NO_ERROR, (abs(root1 - root2) < 1e-5) ? (Root){1, root1, root2} : (Root){2, root1, root2}};
    }
}

// Function to parse coefficients from an equation
void parseEquation(char *equation, float *a, float *b, float *c, int *errorcode) {
    int i = 0;
    int posEqual = strchr(equation, '=') - equation;
    
    // Initialize coefficients
    *a = 0;
    *b = 0;
    *c = 0;

    char unknown = '0';

    for (int j = 0; j < strlen(equation); j++){
        if (equation[j] >= 'a' && equation[j] <= 'z'){
            if (unknown == '0') unknown = equation[j];
            else if (unknown != equation[j]){
                errorcode = INVALID_EQUATION;
                return;
            }
        }
    }

    float coefficient = 1.0;
    int sign = 1;
    while (i < strlen(equation)) {
        if (equation[i] == 10) break;
        // printf("i = %d, equation[i] = %c\n",i,equation[i]);
        // Parse terms

        // Parse coefficient
        if (equation[i] == '-') {
            sign = -1;
            i++;
        } else if (equation[i] == '+') {
            i++;
        }

        // Check if there is a coefficient
        else if (isNumber(equation[i])) {
            int j = i;
            while (isNumber(equation[j]) || equation[j] == '.') {
                j++;
            }
            coefficient = atof(strndup(equation + i, j - i)) * sign;
            sign = 1;
            // printf("coefficient = %f\n",coefficient);
            i = j;
            if (equation[i] != unknown){
                *c += LeftOfEqual * coefficient;
                // printf("%d * %f\n",LeftOfEqual,coefficient);
                // printf("update c to %f\n",*c);
                coefficient = 1;
            }
        }

        // Check if there is a variable (x)
        else if (equation[i] == unknown) {
            // Move to the next character after 'x'
            i++;
            // Check if there is an exponent (^)
            if (equation[i] == '^') {
                if (equation[++i] != '2'){
                    errorcode = INVALID_EQUATION;
                    return;
                }
                i++;
                *a += LeftOfEqual * coefficient;
                // printf("%d * %f\n",(i-1<posEqual),coefficient);
                // printf("update a to %f\n",*a);
                coefficient = 1;
            } else {
                // If no exponent, treat it as x^1
                *b += LeftOfEqual * coefficient;
                // printf("%d * %f\n",LeftOfEqual,coefficient);
                // printf("update b to %f\n",*b);
                coefficient = 1;
            }
        }

        // Check if there is an equal sign
        else if (equation[i] == '=') {
            i++;
        }
    }
}

// Updated function to solve equations
Pair2 SolveEquation(char *str) {
    float a, b, c;
    int errorcode = 0;
    parseEquation(str, &a, &b, &c, &errorcode);

    if (a != 0) {
        // It's a quadratic equation
        return solveQuadraticEquation(a, b, c);
    } else if (b != 0) {
        // It's a linear equation
        return solveLinearEquation(b, c);
    } else {
        return (Pair2){INVALID_EQUATION, (Root){0, 0, 0}};
    }
}

// int main(){
//     char tempExpression[100];
//     printf("Enter the equaltion: ");
//     fgets(tempExpression, sizeof(tempExpression), stdin);
//     Pair2 ans = SolveEquation(tempExpression);
//     printf("The result = %.2f and %.2f(num = %d), the errcode is %d\n", 
//         ans.root.root1, ans.root.root2, ans.root.rootNum, ans.errorCode);
// }