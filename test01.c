#include <stdio.h>
#include <stdlib.h>

void import_books_from_json(const char* filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return;
    }
    
    printf("文件 %s 成功打开。\n", filename);

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // 分配足够的内存来存储文件内容
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        printf("内存分配失败！\n");
        fclose(file);
        return;
    }

    // 读取文件内容
    fread(content, 1, file_size, file);
    content[file_size] = '\0'; // 确保字符串以 NULL 结尾

    printf("文件内容:\n%s\n", content);

    free(content); // 释放内存
    fclose(file);
}

int main() {
    const char *file_name = "books.json";
    import_books_from_json(file_name);
    return 0;
}

