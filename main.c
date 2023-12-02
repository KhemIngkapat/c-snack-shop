#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct item {
    char name[20];
    double price;
    char exp[10];
    unsigned long timestamp;
    int id;
} Item;

int item_count = 0;

void start_shop();
void create_stock_from_product(char *file_name);
char *strtrim(char *str);
int is_leap(int n);
unsigned long date_to_day(char *date);
void create_item(Item *item, char *name, double price, char *exp, int id);
void sort_stock_file();
void add_to_stock(char *name, double price, char *exp,int id);
void remove_from_stock(int id);
Item *stock_from_file();
void selling();

int main() {
    start_shop();
    /* printf("item_count = %d\n",item_count); */
    /* add_to_stock("chocolate", 169, "2/24/2022",item_count); */
    /* remove_from_stock(2); */
    selling();


    return 0;
}

void start_shop() {
    char stock_file_name[] = "stock.csv";
    char product_file_name[] = "product.csv";

    FILE *stock_file = fopen(stock_file_name, "r");
    if (!stock_file) {
        create_stock_from_product("product.csv");
    }
    sort_stock_file();
}

void create_stock_from_product(char *file_name) {
    char name[10], exp[10];
    double price;
    FILE *product_file = fopen(file_name, "r");
    FILE *stock_file = fopen("stock.csv", "w");
    char buffer[255];

    while (fgets(buffer, 255, product_file) != NULL) {
        Item item;
        sscanf(buffer, "%20[^,],%lf,%s", name, &price, exp);
        fprintf(stock_file, "%s,%lf,%s,%d\n", name, price, exp, item_count);
        item_count++;
    }
    fclose(product_file);
    fclose(stock_file);
}

char *strtrim(char *str) {
    size_t len = strlen(str);
    if (len == 0) {
        return str;
    }
    size_t start = 0;
    while (isspace(str[start])) {
        start++;
    }
    size_t end = len - 1;
    while (isspace(str[end])) {
        end--;
    }
    size_t i;
    for (i = 0; i <= end - start; i++) {
        str[i] = str[start + i];
    }
    str[i] = '\0';
    return str;
}

int is_leap(int n) {
    if (n % 4 == 0 && n % 100 != 0 || n % 400 == 0) {
        return 1;
    }
    return 0;
}

unsigned long date_to_day(char *date) {
    int days, d, m, y;

    sscanf(date, "%d/%d/%d", &m, &d, &y);

    int days_in_month[12] = {
        31, 28 + is_leap(y), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    days += d - 1;

    for (int i = 0; i < m - 1; i++) {
        days += days_in_month[i];
    }

    for (int i = 1970; i < y; i++) {
        days += 365 + is_leap(i);
    }

    return days;
}

void create_item(Item *item, char *name, double price, char *exp, int id) {

    strcpy(item->name, strtrim(name));
    item->price = price;
    strcpy(item->exp, strtrim(exp));
    item->timestamp = date_to_day(exp);
    item->id = id;
}

void sort_stock_file() {
    char name[10], exp[10];
    double price;
    int id;
    FILE *stock_file = fopen("stock.csv", "r");
    FILE *temp_stock_file = fopen("temp_stock.csv", "w");
    Item *stock_arr = malloc(sizeof(Item) * 0);

    char buffer[255];
    int idx = 0;
    int product_exist = 0;
    if (item_count > 0) {
        product_exist = 1;
    }
    while (fgets(buffer, 255, stock_file) != NULL) {
        Item item;
        sscanf(buffer, "%20[^,],%lf,%10[^,],%d", name, &price, exp, &id);
        create_item(&item, name, price, exp, id);
        stock_arr = realloc(stock_arr, sizeof(Item) * (idx + 1));
        stock_arr[idx] = item;
        idx++;
        item_count++;
        if (product_exist) {
            item_count--;
        }
    }

    for (int slot = 1; slot < idx; slot++) {
        Item val = stock_arr[slot];
        int test_slot = slot - 1;
        while (test_slot > -1 &&
               stock_arr[test_slot].timestamp > val.timestamp) {
            stock_arr[test_slot + 1] = stock_arr[test_slot];
            test_slot -= 1;
        }
        stock_arr[test_slot + 1] = val;
    }
    for (int i = 0; i < idx; i++) {
        fprintf(temp_stock_file, "%s,%lf,%s,%d\n", stock_arr[i].name,
                stock_arr[i].price, stock_arr[i].exp, stock_arr[i].id);
    }
    fclose(stock_file);
    fclose(temp_stock_file);

    remove("stock.csv");
    rename("temp_stock.csv", "stock.csv");
}

void add_to_stock(char *name, double price, char *exp,int id) {
    FILE *stock_file = fopen("stock.csv", "a");
    fprintf(stock_file, "%s,%lf,%s,%d", name, price, exp,id);
    fclose(stock_file);
    sort_stock_file();
}

void remove_from_stock(int item_id) {
    if (item_id > item_count) {
        puts("Item Not Found");
        return;
    }
    FILE *stock_file, *temp_stock_file;
    char buffer[2048];
    char name[10], exp[10];
    double price;
    int id;
    stock_file = fopen("stock.csv", "r");
    temp_stock_file = fopen("temp_stock.csv", "w");

    int keep_reading = 1;
    do {
        fgets(buffer, 2048, stock_file);
        sscanf(buffer, "%20[^,],%lf,%10[^,],%d", name, &price, exp, &id);
        if (feof(stock_file)) {
            keep_reading = 0;
        } else if (id != item_id) {
            fputs(buffer, temp_stock_file);
        }
    } while (keep_reading);

    fclose(stock_file);
    fclose(temp_stock_file);

    remove("stock.csv");
    rename("temp_stock.csv", "stock.csv");
    item_count--;
}

Item *stock_from_file() {
    char name[20],exp[10];
    double price;
    int id;
    FILE *stock_file = fopen("stock.csv","r");
    Item *stock_arr = malloc(sizeof(Item) * 0);
    char buffer[255];

    int idx = 0;
    while (fgets(buffer, 255, stock_file) != NULL) {
        Item item;
        sscanf(buffer, "%20[^,],%lf,%10[^,],%d", name, &price, exp, &id);
        create_item(&item, name, price, exp, id);
        stock_arr = realloc(stock_arr, sizeof(Item) * (idx + 1));
        stock_arr[idx] = item;
        idx++;
    }

    return stock_arr;
    fclose(stock_file);

}
void print_line(int length){
    for(int i = 0;i<length;i++){
        printf("-");
    }
    puts("");
}

void selling(){
    static int cart_idx = 0;
    Item *stock = stock_from_file();
    int id_arr[item_count];
    for(int i = 0;i<item_count;i++){
        id_arr[i] = stock[i].id;
    }
    int cart[item_count];
    puts("Hello, Welcome to our snack shop");
    puts("this is our products");
    print_line(32);

    printf("|id|%-18s|%9s|\n","product","price");
    print_line(32);
    for(int i = 0;i<item_count;i++){
        printf("|%2d|%-18s|%9.2lf|\n",i,stock[i].name,stock[i].price);
    }
    print_line(32);

    char input[5];
    printf("Select item by putting the id in\n");
    printf("And to checkout enter \"c\"\n");
    printf("enter here : ");
    scanf("%s",input);
    if(input[0] == 99){
        printf("Checkout now!\n");
        return ;
    }else if(input[0] > 57 || atoi(input) >= item_count){
        puts("Invalid Input");
        selling();
    }else{
        cart[cart_idx] = atoi(input);
        remove_from_stock(id_arr[atoi(input)]);
        cart_idx++;
        selling();
    }
    
}
