#include <stdio.h>
#include <locale.h>


int main(){

    // ������������Ϊ���� UTF-8
    if (setlocale(LC_ALL, "zh_CN.UTF-8") == NULL) {
        printf("�޷������������ã�\n");
        return 1;
    }

    printf("��ã����磡\n"); // �����������
    return 0;
}

