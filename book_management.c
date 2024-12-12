#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h" // 需要将 cJSON 库的头文件加入工程中
#include "cJSON.c" 
#include <locale.h>

//#include <locale.h>
// 定义书籍结构体
typedef struct {
    int id;
    char title[100];
    char author[100];
    float price;
} Book;

// 定义文件名
#define FILENAME "books.dat"

// 函数声明
void addBook();
void viewBooks();
void searchBook();
void deleteBook();
void importBooksFromJSON(const char *filename);
void menu();


   
int main() {
	menu();
    return 0;
}

// 主菜单
void menu() {
	int sortBy; 
    int choice;
    while (1) {
        printf("\n=== 在线图书管理系统 ===\n");
        printf("1. 添加图书\n");
        printf("2. 查看所有图书\n");
        printf("3. 搜索图书\n");
        printf("4. 删除图书\n");
        printf("5. 从 JSON 文件导入图书\n");
        printf("6. 导出图书到 JSON 文件\n"); // 新增选项
        printf("7. 更新图书信息\n"); // 新增选项
        printf("8. 按价格或作者排序\n"); // 新增选项
		printf("9. 图书统计信息\n"); // 新增选项
		printf("10. 生成图书报告\n"); // 新增选项
        printf("11. 退出\n");
        printf("请选择 (1-11): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                viewBooks();
                break;
            case 3:
                searchBook();
                break;
            case 4:
                deleteBook();
                break;
            case 5:
                importBooksFromJSON("books.json");
                break;
            case 6:
                exportBooksToJSON("exported_books.json"); // 调用导出函数
                break;
            case 7:
                updateBook();
                break;
            case 8: 
			    //int sortBy;
			    printf("选择排序依据:\n");
			    printf("1. 按 ID 排序\n");
			    printf("2. 按标题排序\n");
			    printf("3. 按作者排序\n");
			    printf("4. 按价格排序\n");
			    printf("请输入 (1-4): ");
			    scanf("%d", &sortBy);
			    sortBooks(sortBy);
			    break;
			case 9:
				 statistics();
				 break;
			case 10: {
			    char filename[100];
			    printf("请输入生成报告的文件名: ");
			    scanf("%s", filename);
			    generateReport(filename);  // 调用 generateReport 函数
			    break;
			}	

            case 11:
                printf("退出系统...\n");
                exit(0);
            default:
                printf("无效的选择，请重新输入！\n");
        }
    }
}

// 添加图书
//void addBook() {
//    FILE *file = fopen(FILENAME, "ab");
//    if (!file) {
//        printf("无法打开文件！\n");
//        return;
//    }
//
//    Book book;
//    printf("\n输入图书ID: ");
//    scanf("%d", &book.id);
//    printf("输入图书名称: ");
//    getchar(); // 清除缓冲区
//    fgets(book.title, 100, stdin);
//    book.title[strcspn(book.title, "\n")] = 0; // 移除换行符
//    printf("输入作者名称: ");
//    fgets(book.author, 100, stdin);
//    book.author[strcspn(book.author, "\n")] = 0; // 移除换行符
//    printf("输入图书价格: ");
//    scanf("%f", &book.price);
//
//    fwrite(&book, sizeof(Book), 1, file);
//    fclose(file);
//
//    printf("图书添加成功！\n");
//}


// 添加图书
void addBook() {
    FILE *file = fopen(FILENAME, "ab+"); // 以读取和追加模式打开文件
    if (!file) {
        printf("无法打开文件！\n");
        return;
    }

    // 查找当前文件中的最大 ID
    int maxId = 0;
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id > maxId) {
            maxId = book.id;
        }
    }

    // 新图书的 ID 设置为最大 ID + 1
    book.id = maxId + 1;

    // 获取其他图书信息
    printf("\n输入图书名称: ");
    getchar(); // 清除缓冲区
    fgets(book.title, 100, stdin);
    book.title[strcspn(book.title, "\n")] = 0; // 移除换行符
    printf("输入作者名称: ");
    fgets(book.author, 100, stdin);
    book.author[strcspn(book.author, "\n")] = 0; // 移除换行符
    printf("输入图书价格: ");
    scanf("%f", &book.price);

    // 写入新图书信息
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);

    printf("图书添加成功！ID: %d\n", book.id);
}





// 查看所有图书
void viewBooks() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("无图书记录！\n");
        return;
    }

    Book book;
    printf("\n=== 图书列表 ===\n");
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("ID: %d, 名称: %s, 作者: %s, 价格: %.2f\n",
               book.id, book.title, book.author, book.price);
    }
    fclose(file);
}

// 搜索图书
void searchBook() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("无图书记录！\n");
        return;
    }

    int searchId;
    printf("\n输入要搜索的图书ID: ");
    scanf("%d", &searchId);

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == searchId) {
            printf("找到图书！\n");
            printf("ID: %d, 名称: %s, 作者: %s, 价格: %.2f\n",
                   book.id, book.title, book.author, book.price);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("未找到图书！\n");
    }
}

// 删除图书
void deleteBook() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("无图书记录！\n");
        return;
    }

    int deleteId;
    printf("\n输入要删除的图书ID: ");
    scanf("%d", &deleteId);

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("无法创建临时文件！\n");
        fclose(file);
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == deleteId) {
            found = 1;
            printf("图书ID %d 已删除。\n", deleteId);
        } else {
            fwrite(&book, sizeof(Book), 1, tempFile);
        }
    }
    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (!found) {
        printf("未找到要删除的图书！\n");
    }
}



// 从 JSON 文件导入图书
void importBooksFromJSON(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("无法打开 JSON 文件：%s\n", filename);
        return;
    }

    // 获取文件内容
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *jsonString = (char *)malloc(fileSize + 1);
    if (!jsonString) {
        printf("内存分配失败！\n");
        fclose(file);
        return;
    }
    fread(jsonString, 1, fileSize, file);
    jsonString[fileSize] = '\0';
    fclose(file);

    // 解析 JSON 数据
    cJSON *json = cJSON_Parse(jsonString);
    free(jsonString);
    if (!json) {
        printf("JSON 解析失败：%s\n", cJSON_GetErrorPtr());
        return;
    }

    // 获取图书数组
    cJSON *booksArray = cJSON_GetObjectItem(json, "books");
    if (!cJSON_IsArray(booksArray)) {
        printf("无效的 JSON 格式：未找到 'books' 数组！\n");
        cJSON_Delete(json);
        return;
    }

    // 打开二进制文件以写入数据
    FILE *outputFile = fopen(FILENAME, "ab");
    if (!outputFile) {
        printf("无法打开文件：%s\n", FILENAME);
        cJSON_Delete(json);
        return;
    }

    // 遍历数组并写入数据
    cJSON *bookItem;
    cJSON_ArrayForEach(bookItem, booksArray) {
        Book book;

        // 从 JSON 对象中提取数据
        cJSON *id = cJSON_GetObjectItem(bookItem, "id");
        cJSON *title = cJSON_GetObjectItem(bookItem, "title");
        cJSON *author = cJSON_GetObjectItem(bookItem, "author");
        cJSON *price = cJSON_GetObjectItem(bookItem, "price");

        if (cJSON_IsNumber(id) && cJSON_IsString(title) && cJSON_IsString(author) && cJSON_IsNumber(price)) {
            book.id = id->valueint;
            strncpy(book.title, title->valuestring, sizeof(book.title) - 1);
            strncpy(book.author, author->valuestring, sizeof(book.author) - 1);
            book.title[sizeof(book.title) - 1] = '\0';
            book.author[sizeof(book.author) - 1] = '\0';
            book.price = (float)price->valuedouble;

            fwrite(&book, sizeof(Book), 1, outputFile);
            printf("已导入图书：ID=%d, 名称=%s, 作者=%s, 价格=%.2f\n",
                   book.id, book.title, book.author, book.price);
        } else {
            printf("无效的图书数据，跳过！\n");
        }
    }

    fclose(outputFile);
    cJSON_Delete(json);
    printf("图书数据导入完成！\n");
}



void exportBooksToJSON(const char *filename) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("没有可导出的图书记录！\n");
        return;
    }

    // 创建 JSON 对象
    cJSON *root = cJSON_CreateObject();
    cJSON *booksArray = cJSON_CreateArray();

    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        cJSON *bookObject = cJSON_CreateObject();
        cJSON_AddNumberToObject(bookObject, "id", book.id);
        cJSON_AddStringToObject(bookObject, "title", book.title);
        cJSON_AddStringToObject(bookObject, "author", book.author);
        cJSON_AddNumberToObject(bookObject, "price", book.price);
        cJSON_AddItemToArray(booksArray, bookObject);
    }
    fclose(file);

    cJSON_AddItemToObject(root, "books", booksArray);

    // 将 JSON 数据写入文件
    char *jsonString = cJSON_Print(root);
    FILE *outputFile = fopen(filename, "w");
    if (!outputFile) {
        printf("无法创建导出文件：%s\n", filename);
        cJSON_Delete(root);
        free(jsonString);
        return;
    }

    fprintf(outputFile, "%s", jsonString);
    fclose(outputFile);

    // 释放 JSON 内存
    cJSON_Delete(root);
    free(jsonString);

    printf("图书成功导出到文件：%s\n", filename);
}



void updateBook() {
    int id;
    printf("请输入要更新的图书ID: ");
    scanf("%d", &id);

    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("图书记录文件不存在！\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            found = 1;
            printf("找到图书: %s by %s, 价格: %.2f\n", book.title, book.author, book.price);
            printf("请输入新的图书信息:\n");
            printf("名称: ");
            getchar(); // 清理缓冲区
            fgets(book.title, sizeof(book.title), stdin);
            book.title[strcspn(book.title, "\n")] = 0; // 去掉换行符
            printf("作者: ");
            fgets(book.author, sizeof(book.author), stdin);
            book.author[strcspn(book.author, "\n")] = 0;
            printf("价格: ");
            scanf("%f", &book.price);

            // 定位到该记录位置并更新
            fseek(file, -sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            printf("图书信息已更新成功！\n");
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("未找到ID为 %d 的图书！\n", id);
    }
}




void sortBooks(int sortBy) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("图书记录文件不存在！\n");
        return;
    }

    // 读取所有图书记录
    Book books[100];
    int count = 0;
    while (fread(&books[count], sizeof(Book), 1, file)) {
        count++;
    }
    fclose(file);

    // 排序
    int i,j;
    for ( i = 0; i < count - 1; i++) {
        for ( j = i + 1; j < count; j++) {
            if ((sortBy == 1 && books[i].price > books[j].price) ||  // 按价格升序
                (sortBy == 2 && strcmp(books[i].author, books[j].author) > 0)) { // 按作者姓名
                Book temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }

    // 显示排序结果
    printf("排序后的图书列表:\n");
    
    for ( i = 0; i < count; i++) {
        printf("ID: %d, 标题: %s, 作者: %s, 价格: %.2f\n",
               books[i].id, books[i].title, books[i].author, books[i].price);
    }
}



void statistics() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("图书记录文件不存在！\n");
        return;
    }

    int count = 0;
    float totalPrice = 0.0;
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        count++;
        totalPrice += book.price;
    }
    fclose(file);

    printf("图书总数: %d\n", count);
    if (count > 0) {
        printf("图书平均价格: %.2f\n", totalPrice / count);
    } else {
        printf("没有图书记录可供统计。\n");
    }
}


void generateReport(const char *filename) {
    FILE *file = fopen("books.dat", "rb");  // 打开图书记录文件，假设文件名为 "books.dat"
    if (!file) {
        printf("图书记录文件不存在！\n");
        return;
    }

    FILE *reportFile = fopen(filename, "w");  // 打开用户指定的报告文件
    if (!reportFile) {
        printf("无法创建报告文件！\n");
        fclose(file);
        return;
    }

    fprintf(reportFile, "=== 图书报告 ===\n\n");
    fprintf(reportFile, "ID\t标题\t\t作者\t\t价格\n");
    fprintf(reportFile, "---------------------------------------------------\n");

    Book book;
    float totalPrice = 0.0;
    int count = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        fprintf(reportFile, "%d\t%-20s\t%-20s\t%.2f\n",
                book.id, book.title, book.author, book.price);
        totalPrice += book.price;
        count++;
    }

    fprintf(reportFile, "\n总书籍数量: %d\n", count);
    fprintf(reportFile, "总价格: %.2f\n", totalPrice);

    fclose(file);
    fclose(reportFile);

    printf("图书报告已生成：%s\n", filename);
}



