#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../Core/Inc/calc.h"

char* decimalToStr(long long decimal) {
    char* binary = (char*)malloc(20 * sizeof(char));
    if (decimal == 0){
        binary[0] = '0';
        binary[1] = '\0';
        return binary;
    }
    int index = 0;

    while (decimal > 0) {
        binary[index++] = (char)((decimal % 10) + '0');
        decimal /= 10;
    }

    // reverse
    for (int i = 0; i < index / 2; ++i) {
        char temp = binary[i];
        binary[i] = binary[index - i - 1];
        binary[index - i - 1] = temp;
    }

    binary[index] = '\0';
    return binary;
}

Pair CalcBinaryExpression(char* expression){
    char decimalExpression[100] = {0};
    int length = strlen(expression);
    char buffer[20];
    int decimalIndex = 0;

    for (int i = 0; i < length; i++){
        if (expression[i] == '+'|| expression[i] == '-' || expression[i] == '*') {
            decimalExpression[decimalIndex++] = expression[i];
        }
        else if (expression[i] == '0' || expression[i] == '1') {
            int j = 0;
            while (expression[i] == '0' || expression[i] == '1') {
                buffer[j++] = expression[i++];
            }
            buffer[j] = '\0';
            char *endptr;
            long int num = strtol(buffer, &endptr, 2);
            if (*endptr != '\0'){
                return (Pair){TOO_LARGE_NUMBER, 0};
            }

            char* decimalString = decimalToStr(num);
            for (int k = 0; k < strlen(decimalString); k++)
                decimalExpression[decimalIndex++] = decimalString[k];

            i--;
        }
    }
    decimalExpression[decimalIndex] = '\0';
    printf("%s\n",decimalExpression);

    return CalcExpression(decimalExpression);
}

int main(){
    char str[100];
    printf("Enter the bi-expression: ");
    fgets(str, sizeof(str), stdin);
    Pair ans = CalcBinaryExpression(str);
    printf("Ans = %d",(int)ans.result);
}