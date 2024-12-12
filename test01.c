#include <stdio.h>
#include <stdlib.h>

void import_books_from_json(const char* filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("�޷����ļ� %s\n", filename);
        return;
    }
    
    printf("�ļ� %s �ɹ��򿪡�\n", filename);

    // ��ȡ�ļ���С
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // �����㹻���ڴ����洢�ļ�����
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
        fclose(file);
        return;
    }

    // ��ȡ�ļ�����
    fread(content, 1, file_size, file);
    content[file_size] = '\0'; // ȷ���ַ����� NULL ��β

    printf("�ļ�����:\n%s\n", content);

    free(content); // �ͷ��ڴ�
    fclose(file);
}

int main() {
    const char *file_name = "books.json";
    import_books_from_json(file_name);
    return 0;
}

