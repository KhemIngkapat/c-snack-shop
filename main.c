#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

typedef struct item {
    char name[20];
    double price;
    char exp[10];
    unsigned long timestamp;
    int id;
} Item;

static int item_count = 0;

char *strtrim(char *str);
void create_item(Item *item, char *name, double price, char *exp,int id);
Item *sort_stock(Item *stock,int size);
void add_to_stock(Item item,int *item_count);
void remove_from_stock(Item item);
void apply_promotion(Item stock[],int stock_size,double discount);
void create_stock_file(Item *item,int size);

int main() {
    char name[10],exp[10];
    double price;
    Item *stock = malloc(sizeof(Item) * 0);
    FILE *file = fopen("final/product.csv", "r");
    char buffer[255];
    int size = 0;
    while(fgets(buffer,255,file) != NULL){
        item_count++;
        Item item;
        sscanf(buffer, "%20[^,],%lf,%s", name, &price, exp);
        create_item(&item, name, price, exp,item_count);
        size++;
        // dynamically allocating memeory size by stock size;
        stock = realloc(stock, sizeof(Item) *size);
        stock[size-1] = item;
    }
    fclose(file);
    sort_stock(stock,size);
    create_stock_file(stock,size);


    for (int i = 0; i < size; i++) {
        printf("%s\n", stock[i].name);
        printf("\t%lf", stock[i].price);
        printf("\t%s", stock[i].exp);
        printf("\t%ld", stock[i].timestamp);
        printf("\t%d\n", stock[i].id);
    }

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
    
    days += d-1;

    for(int i = 0;i<m-1;i++){
        days += days_in_month[i];
    }

    for(int i = 1970;i<y;i++){
        days += 365 + is_leap(i);
    }

    return days;
}

void create_item(Item *item, char *name, double price, char *exp,int id) {

    strcpy(item->name, strtrim(name));
    item->price = price;
    strcpy(item->exp, strtrim(exp));
    item->timestamp = date_to_day(exp);
    item->id = id;
}


Item* sort_stock(Item* stock,int size){
    for(int slot=1;slot<size;slot++){
        Item val = stock[slot];
        int test_slot = slot-1;
        while(test_slot > -1 && stock[test_slot].timestamp > val.timestamp){
            stock[test_slot + 1] = stock[test_slot];
            test_slot -= 1;
        }
        stock[test_slot+1] = val;
    }
    return stock;
}

void add_to_stock(Item item,int *item_count){
    FILE *file;
    file = fopen("final/stock.csv","a");

    fprintf(file, "%s,%lf,%s,%d\n",item.name,item.price,item.exp,item.id);

    fclose(file);
    item_count++;
}

void remove_from_stock(Item item){
    FILE *file, *temp;
    char buffer[2048];
    char name[10],exp[10];
    double price;
    int id;
    file = fopen("final/stock.csv","r");
    temp = fopen("final/temp_stock.csv","w");

    int keep_reading = 1;
    do{
        fgets(buffer,2048,file);
        sscanf(buffer, "%20[^,],%lf,%10[^,],%d", name, &price, exp,&id);
        if(feof(file)){
            keep_reading = 0;
        }else if(id != item.id){
            fputs(buffer,temp);
        }
    }while(keep_reading);
    
    fclose(file);
    fclose(temp);

    remove("final/stock.csv");
    rename("final/temp_stock.csv","final/stock.csv");


}

void apply_promotion(Item stock[],int stock_size,double discount){
    for(int i = 0;i<stock_size;i++){
        float discounted = (double)1 - discount;
        stock[i].price = stock[i].price * discounted;
    }
}


void create_stock_file(Item *item,int size){
    FILE *file;
    file = fopen("final/stock.csv","w");

    for(int i = 0;i<size;i++){
        fprintf(file,"%s,%lf,%s,%d\n",item[i].name,item[i].price,item[i].exp,item[i].id);
    }
    fclose(file);

}
