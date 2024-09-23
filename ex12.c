#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;

    if(argc == 1) {
        printf("You only provided the program name: %s\n", argv[0]);
    } 
    //else if(argc > 1 && argc < 4) {
    else{
        printf("You provided %d arguments (including the program name).\n", argc);

        for (i = 0; i < argc; i++) {
            printf("arg %d: %s\n", i, argv[i]);
        }
        printf("\n");
    } 
    /*else {
        printf("You have too many arguments. You suck.\n");
    }*/

    return 0;
}