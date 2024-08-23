#include <stdio.h>
int min(int a,int b){
    int output;
    output = (a>b)? b:a;

    return output;
}
int main(int argc, char *argv[])
{
    int i = 0;

    // go through each string in argv
    // why am I skipping argv[0]?
    for(i = 1; i < argc; i++) {
        printf("arg %d: %s\n", i, argv[i]);
    }

    // let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas",NULL
    };
    int num_states = 5;

 
    for(i = 0; i < num_states; i++) {
        printf("state %d: %s\n", i, states[i]);
    }

    if (argc > 1) {
        i = 0;
    
        //states[0] = argv[1];
        while (i < min(num_states,argc)){
            states[i] = argv[i];
            i++;
        }
    }

    int j = 4;
    while (j >= 0){
        printf("state %d: %s\n", j, states[j]);
        if(j != 0)
            j--;
        else 
            break;
    }

    //modify argv
    argv[1][1] = 'X';
    printf("state %d: %s\n", 1, states[1]);

    return 0;
}
