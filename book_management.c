#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h" // ��Ҫ�� cJSON ���ͷ�ļ����빤����
#include "cJSON.c" 
#include <locale.h>

//#include <locale.h>
// �����鼮�ṹ��
typedef struct {
    int id;
    char title[100];
    char author[100];
    float price;
} Book;

// �����ļ���
#define FILENAME "books.dat"

// ��������
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

// ���˵�
void menu() {
	int sortBy; 
    int choice;
    while (1) {
        printf("\n=== ����ͼ�����ϵͳ ===\n");
        printf("1. ���ͼ��\n");
        printf("2. �鿴����ͼ��\n");
        printf("3. ����ͼ��\n");
        printf("4. ɾ��ͼ��\n");
        printf("5. �� JSON �ļ�����ͼ��\n");
        printf("6. ����ͼ�鵽 JSON �ļ�\n"); // ����ѡ��
        printf("7. ����ͼ����Ϣ\n"); // ����ѡ��
        printf("8. ���۸����������\n"); // ����ѡ��
		printf("9. ͼ��ͳ����Ϣ\n"); // ����ѡ��
		printf("10. ����ͼ�鱨��\n"); // ����ѡ��
        printf("11. �˳�\n");
        printf("��ѡ�� (1-11): ");
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
                exportBooksToJSON("exported_books.json"); // ���õ�������
                break;
            case 7:
                updateBook();
                break;
            case 8: 
			    //int sortBy;
			    printf("ѡ����������:\n");
			    printf("1. �� ID ����\n");
			    printf("2. ����������\n");
			    printf("3. ����������\n");
			    printf("4. ���۸�����\n");
			    printf("������ (1-4): ");
			    scanf("%d", &sortBy);
			    sortBooks(sortBy);
			    break;
			case 9:
				 statistics();
				 break;
			case 10: {
			    char filename[100];
			    printf("���������ɱ�����ļ���: ");
			    scanf("%s", filename);
			    generateReport(filename);  // ���� generateReport ����
			    break;
			}	

            case 11:
                printf("�˳�ϵͳ...\n");
                exit(0);
            default:
                printf("��Ч��ѡ�����������룡\n");
        }
    }
}

// ���ͼ��
//void addBook() {
//    FILE *file = fopen(FILENAME, "ab");
//    if (!file) {
//        printf("�޷����ļ���\n");
//        return;
//    }
//
//    Book book;
//    printf("\n����ͼ��ID: ");
//    scanf("%d", &book.id);
//    printf("����ͼ������: ");
//    getchar(); // ���������
//    fgets(book.title, 100, stdin);
//    book.title[strcspn(book.title, "\n")] = 0; // �Ƴ����з�
//    printf("������������: ");
//    fgets(book.author, 100, stdin);
//    book.author[strcspn(book.author, "\n")] = 0; // �Ƴ����з�
//    printf("����ͼ��۸�: ");
//    scanf("%f", &book.price);
//
//    fwrite(&book, sizeof(Book), 1, file);
//    fclose(file);
//
//    printf("ͼ����ӳɹ���\n");
//}


// ���ͼ��
void addBook() {
    FILE *file = fopen(FILENAME, "ab+"); // �Զ�ȡ��׷��ģʽ���ļ�
    if (!file) {
        printf("�޷����ļ���\n");
        return;
    }

    // ���ҵ�ǰ�ļ��е���� ID
    int maxId = 0;
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id > maxId) {
            maxId = book.id;
        }
    }

    // ��ͼ��� ID ����Ϊ��� ID + 1
    book.id = maxId + 1;

    // ��ȡ����ͼ����Ϣ
    printf("\n����ͼ������: ");
    getchar(); // ���������
    fgets(book.title, 100, stdin);
    book.title[strcspn(book.title, "\n")] = 0; // �Ƴ����з�
    printf("������������: ");
    fgets(book.author, 100, stdin);
    book.author[strcspn(book.author, "\n")] = 0; // �Ƴ����з�
    printf("����ͼ��۸�: ");
    scanf("%f", &book.price);

    // д����ͼ����Ϣ
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);

    printf("ͼ����ӳɹ���ID: %d\n", book.id);
}





// �鿴����ͼ��
void viewBooks() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("��ͼ���¼��\n");
        return;
    }

    Book book;
    printf("\n=== ͼ���б� ===\n");
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("ID: %d, ����: %s, ����: %s, �۸�: %.2f\n",
               book.id, book.title, book.author, book.price);
    }
    fclose(file);
}

// ����ͼ��
void searchBook() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("��ͼ���¼��\n");
        return;
    }

    int searchId;
    printf("\n����Ҫ������ͼ��ID: ");
    scanf("%d", &searchId);

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == searchId) {
            printf("�ҵ�ͼ�飡\n");
            printf("ID: %d, ����: %s, ����: %s, �۸�: %.2f\n",
                   book.id, book.title, book.author, book.price);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("δ�ҵ�ͼ�飡\n");
    }
}

// ɾ��ͼ��
void deleteBook() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("��ͼ���¼��\n");
        return;
    }

    int deleteId;
    printf("\n����Ҫɾ����ͼ��ID: ");
    scanf("%d", &deleteId);

    FILE *tempFile = fopen("temp.dat", "wb");
    if (!tempFile) {
        printf("�޷�������ʱ�ļ���\n");
        fclose(file);
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == deleteId) {
            found = 1;
            printf("ͼ��ID %d ��ɾ����\n", deleteId);
        } else {
            fwrite(&book, sizeof(Book), 1, tempFile);
        }
    }
    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (!found) {
        printf("δ�ҵ�Ҫɾ����ͼ�飡\n");
    }
}



// �� JSON �ļ�����ͼ��
void importBooksFromJSON(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("�޷��� JSON �ļ���%s\n", filename);
        return;
    }

    // ��ȡ�ļ�����
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *jsonString = (char *)malloc(fileSize + 1);
    if (!jsonString) {
        printf("�ڴ����ʧ�ܣ�\n");
        fclose(file);
        return;
    }
    fread(jsonString, 1, fileSize, file);
    jsonString[fileSize] = '\0';
    fclose(file);

    // ���� JSON ����
    cJSON *json = cJSON_Parse(jsonString);
    free(jsonString);
    if (!json) {
        printf("JSON ����ʧ�ܣ�%s\n", cJSON_GetErrorPtr());
        return;
    }

    // ��ȡͼ������
    cJSON *booksArray = cJSON_GetObjectItem(json, "books");
    if (!cJSON_IsArray(booksArray)) {
        printf("��Ч�� JSON ��ʽ��δ�ҵ� 'books' ���飡\n");
        cJSON_Delete(json);
        return;
    }

    // �򿪶������ļ���д������
    FILE *outputFile = fopen(FILENAME, "ab");
    if (!outputFile) {
        printf("�޷����ļ���%s\n", FILENAME);
        cJSON_Delete(json);
        return;
    }

    // �������鲢д������
    cJSON *bookItem;
    cJSON_ArrayForEach(bookItem, booksArray) {
        Book book;

        // �� JSON ��������ȡ����
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
            printf("�ѵ���ͼ�飺ID=%d, ����=%s, ����=%s, �۸�=%.2f\n",
                   book.id, book.title, book.author, book.price);
        } else {
            printf("��Ч��ͼ�����ݣ�������\n");
        }
    }

    fclose(outputFile);
    cJSON_Delete(json);
    printf("ͼ�����ݵ�����ɣ�\n");
}



void exportBooksToJSON(const char *filename) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("û�пɵ�����ͼ���¼��\n");
        return;
    }

    // ���� JSON ����
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

    // �� JSON ����д���ļ�
    char *jsonString = cJSON_Print(root);
    FILE *outputFile = fopen(filename, "w");
    if (!outputFile) {
        printf("�޷����������ļ���%s\n", filename);
        cJSON_Delete(root);
        free(jsonString);
        return;
    }

    fprintf(outputFile, "%s", jsonString);
    fclose(outputFile);

    // �ͷ� JSON �ڴ�
    cJSON_Delete(root);
    free(jsonString);

    printf("ͼ��ɹ��������ļ���%s\n", filename);
}



void updateBook() {
    int id;
    printf("������Ҫ���µ�ͼ��ID: ");
    scanf("%d", &id);

    FILE *file = fopen(FILENAME, "rb+");
    if (!file) {
        printf("ͼ���¼�ļ������ڣ�\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            found = 1;
            printf("�ҵ�ͼ��: %s by %s, �۸�: %.2f\n", book.title, book.author, book.price);
            printf("�������µ�ͼ����Ϣ:\n");
            printf("����: ");
            getchar(); // ��������
            fgets(book.title, sizeof(book.title), stdin);
            book.title[strcspn(book.title, "\n")] = 0; // ȥ�����з�
            printf("����: ");
            fgets(book.author, sizeof(book.author), stdin);
            book.author[strcspn(book.author, "\n")] = 0;
            printf("�۸�: ");
            scanf("%f", &book.price);

            // ��λ���ü�¼λ�ò�����
            fseek(file, -sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            printf("ͼ����Ϣ�Ѹ��³ɹ���\n");
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("δ�ҵ�IDΪ %d ��ͼ�飡\n", id);
    }
}




void sortBooks(int sortBy) {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("ͼ���¼�ļ������ڣ�\n");
        return;
    }

    // ��ȡ����ͼ���¼
    Book books[100];
    int count = 0;
    while (fread(&books[count], sizeof(Book), 1, file)) {
        count++;
    }
    fclose(file);

    // ����
    int i,j;
    for ( i = 0; i < count - 1; i++) {
        for ( j = i + 1; j < count; j++) {
            if ((sortBy == 1 && books[i].price > books[j].price) ||  // ���۸�����
                (sortBy == 2 && strcmp(books[i].author, books[j].author) > 0)) { // ����������
                Book temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }

    // ��ʾ������
    printf("������ͼ���б�:\n");
    
    for ( i = 0; i < count; i++) {
        printf("ID: %d, ����: %s, ����: %s, �۸�: %.2f\n",
               books[i].id, books[i].title, books[i].author, books[i].price);
    }
}



void statistics() {
    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("ͼ���¼�ļ������ڣ�\n");
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

    printf("ͼ������: %d\n", count);
    if (count > 0) {
        printf("ͼ��ƽ���۸�: %.2f\n", totalPrice / count);
    } else {
        printf("û��ͼ���¼�ɹ�ͳ�ơ�\n");
    }
}


void generateReport(const char *filename) {
    FILE *file = fopen("books.dat", "rb");  // ��ͼ���¼�ļ��������ļ���Ϊ "books.dat"
    if (!file) {
        printf("ͼ���¼�ļ������ڣ�\n");
        return;
    }

    FILE *reportFile = fopen(filename, "w");  // ���û�ָ���ı����ļ�
    if (!reportFile) {
        printf("�޷����������ļ���\n");
        fclose(file);
        return;
    }

    fprintf(reportFile, "=== ͼ�鱨�� ===\n\n");
    fprintf(reportFile, "ID\t����\t\t����\t\t�۸�\n");
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

    fprintf(reportFile, "\n���鼮����: %d\n", count);
    fprintf(reportFile, "�ܼ۸�: %.2f\n", totalPrice);

    fclose(file);
    fclose(reportFile);

    printf("ͼ�鱨�������ɣ�%s\n", filename);
}



