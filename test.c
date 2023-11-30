#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"


int main(void){
    FILE *file, *temp;
    char buffer[2048];
    int delete_line;

    file = fopen("final/file.txt","r");
    temp = fopen("final/temp.txt","w");

    printf("Which line you want to delete : ");
    scanf("%d",&delete_line);

    bool keep_reading = true;
    int cur_line = 1;
    do{
        fgets(buffer,2048,file);
        if(feof(file)){
            keep_reading = false;
        }else if(cur_line != delete_line){
            fputs(buffer,temp);
        }
        cur_line++;           

    } while(keep_reading);
    fclose(file);
    fclose(temp);

    remove("final/file.txt");
    rename("final/temp.txt","final/file.txt");

    return 0;
}
