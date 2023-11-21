// Modified from:
// https://blog.csdn.net/qq_60678811/article/details/124762176

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../Inc/calc.h"

calcPair CalcOrSolve(char *str){
    if (strstr(str, "=") != NULL) {
        return (calcPair){2, (Pair){0, 0}, SolveEquation(str)};
    }
    else {
        return (calcPair){1, CalcExpression(str), (Pair2){0, (Root){0, 0, 0}}};
    }
}
int main() {
    char temp[100];
    printf("Enter the string: ");
    fgets(temp, sizeof(temp), stdin);
    calcPair ans = CalcOrSolve(temp);
    if (ans.inputType==1) printf("calc\n"); else printf("solve\n");
    if (ans.inputType == 1){
        printf("The answer is %.2f, the errcode is %d\n", ans.p1.result,ans.p1.errorCode);
    }
    else{
        printf("the errorcode is %d\n",ans.p2.errorCode);
        printf("There are %d roots\n",ans.p2.root.rootNum);
        printf("root1 = %f, root2 = %f\n", ans.p2.root.root1, ans.p2.root.root2);
    }
    return 0;
}