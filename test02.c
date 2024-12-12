#include <stdio.h>
#include <locale.h>


int main(){

    // 设置区域设置为中文 UTF-8
    if (setlocale(LC_ALL, "zh_CN.UTF-8") == NULL) {
        printf("无法设置区域设置！\n");
        return 1;
    }

    printf("你好，世界！\n"); // 测试中文输出
    return 0;
}

