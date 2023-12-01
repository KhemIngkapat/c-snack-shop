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

static int item_count = 0;

char *strtrim(char *str);
void create_item(Item *item, char *name, double price, char *exp, int id);
Item *read_product(char *file_name);
Item *sort_stock(Item *stock, int size);
void add_to_stock(Item item);
void remove_from_stock(Item item);
void apply_promotion(Item stock[], int stock_size, double discount);
void create_stock_file(Item *item, int size);
void display_item(Item *stock);

int main() {
    puts("lksjdflkj");
    Item *stock = read_product("product.csv");

    stock = sort_stock(stock, item_count);
    create_stock_file(stock, item_count);
    Item chocolate = {"chocolate",22.5,"6/15/2008",item_count};
    add_to_stock(chocolate);

    display_item(stock);


    // display all item
    // recursively ask customer what they want to pick
    // add the item to cart
    // check out -> add all item price total
    // randomly pick number and apply promotion if number meet the criteria
    // thats it

    return 0;
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

Item *read_product(char *file_name){
    char name[10], exp[10];
    double price;
    Item *stock = malloc(sizeof(Item) * 0);
    FILE *file = fopen(file_name, "r");
    char buffer[255];
    while (fgets(buffer, 255, file) != NULL) {
        Item item;
        sscanf(buffer, "%20[^,],%lf,%s", name, &price, exp);
        create_item(&item, name, price, exp, item_count+1);
        item_count++;
        // dynamically allocating memeory size by stock size;
        stock = realloc(stock, sizeof(Item) * item_count);
        stock[item_count - 1] = item;
    }
    fclose(file);
    return stock;

}

Item *sort_stock(Item *stock, int size) {
    for (int slot = 1; slot < size; slot++) {
        Item val = stock[slot];
        int test_slot = slot - 1;
        while (test_slot > -1 && stock[test_slot].timestamp > val.timestamp) {
            stock[test_slot + 1] = stock[test_slot];
            test_slot -= 1;
        }
        stock[test_slot + 1] = val;
    }
    return stock;
}

void add_to_stock(Item item) {
    FILE *file;
    file = fopen("stock.csv", "a");

    fprintf(file, "%s,%lf,%s,%d\n", item.name, item.price, item.exp, item.id);

    fclose(file);
    item_count++;
}

void remove_from_stock(Item item) {
    FILE *file, *temp;
    char buffer[2048];
    char name[10], exp[10];
    double price;
    int id;
    file = fopen("stock.csv", "r");
    temp = fopen("temp_stock.csv", "w");

    int keep_reading = 1;
    do {
        fgets(buffer, 2048, file);
        sscanf(buffer, "%20[^,],%lf,%10[^,],%d", name, &price, exp, &id);
        if (feof(file)) {
            keep_reading = 0;
        } else if (id != item.id) {
            fputs(buffer, temp);
        }
    } while (keep_reading);

    fclose(file);
    fclose(temp);

    remove("stock.csv");
    rename("temp_stock.csv", "final/stock.csv");
}

void apply_promotion(Item stock[], int stock_size, double discount) {
    for (int i = 0; i < stock_size; i++) {
        float discounted = (double)1 - discount;
        stock[i].price = stock[i].price * discounted;
    }
}

void create_stock_file(Item *item, int size) {
    FILE *file;
    file = fopen("stock.csv", "w");

    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%lf,%s,%d\n", item[i].name, item[i].price,
                item[i].exp, item[i].id);
    }
    fclose(file);
}

void print_line(int length){
    for(int i = 0;i<length;i++){
        printf("-");
    }
    puts("");
}

void display_item(Item *stock){
    puts("Hello, Welcome to our snack shop");
    puts("this is our products");
    print_line(30);

    printf("|%-18s|%9s|\n","product","price");
    print_line(30);
    for(int i = 0;i<item_count;i++){
        printf("|%-18s|%9.2lf|\n",stock[i].name,stock[i].price);
    }
    print_line(30);
}
