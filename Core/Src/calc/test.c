#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 函数声明
char* binaryExpressionToDecimal(char* expression);

// 辅助函数声明
long long binaryToDecimal(char* binary);
char* decimalToBinary(long long decimal);

int ma() {
    char expression[100];
    char* decimalExpression;

    // 输入二进制表达式
    printf("输入二进制表达式（包含加减乘运算符）： ");
    scanf("%s", expression);

    // 转换为十进制表达式
    decimalExpression = binaryExpressionToDecimal(expression);

    // 输出结果
    printf("十进制表达式： %s\n", decimalExpression);

    // 释放内存
    free(decimalExpression);

    return 0;
}

// 将二进制表达式转为十进制表达式
char* binaryExpressionToDecimal(char* expression) {
    int length = strlen(expression);
    char* decimalExpression = (char*)malloc(100 * sizeof(char));
    char buffer[20];  // 用于存储中间的二进制数和十进制数的字符串表示

    int decimalIndex = 0;  // 记录十进制表达式的索引

    for (int i = 0; i < length; ++i) {
        // 当遇到运算符时，直接拼接到十进制表达式中
        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*') {
            decimalExpression[decimalIndex++] = expression[i];
        }
        // 当遇到数字时，获取完整的二进制数并转换为十进制数
        else if (expression[i] == '0' || expression[i] == '1') {
            int j = 0;
            while (expression[i] == '0' || expression[i] == '1') {
                buffer[j++] = expression[i++];
            }
            buffer[j] = '\0';

            // 将二进制数转换为十进制数并拼接到十进制表达式中
            long long decimalValue = binaryToDecimal(buffer);
            char* decimalString = decimalToBinary(decimalValue);

            // 将当前的十进制数拼接到结果中
            int k = 0;
            while (decimalString[k] != '\0') {
                decimalExpression[decimalIndex++] = decimalString[k++];
            }

            // 回退一个字符，因为在循环中有一个额外的i++
            --i;

            // 释放内存
            free(decimalString);
        }
    }

    // 在十进制表达式的末尾添加空字符
    decimalExpression[decimalIndex] = '\0';

    return decimalExpression;
}

// 辅助函数实现

long long binaryToDecimal(char* binary) {
    return strtol(binary, NULL, 2);
}

char* decimalToBinary(long long decimal) {
    char* binary = (char*)malloc(20 * sizeof(char));
    int index = 0;

    while (decimal > 0) {
        binary[index++] = (char)((decimal % 2) + '0');
        decimal /= 2;
    }

    // 反转字符串
    for (int i = 0; i < index / 2; ++i) {
        char temp = binary[i];
        binary[i] = binary[index - i - 1];
        binary[index - i - 1] = temp;
    }

    // 在二进制字符串的末尾添加空字符
    binary[index] = '\0';

    return binary;
}
