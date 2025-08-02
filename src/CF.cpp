#include <stdio.h>

int main(int ArgCount, char **Args){
    printf("CloudyForest started\n");
    if (ArgCount == 1) {
        printf("No input file.\n If you want this to be a language server, run CloudForest with #server\n");
    }


}
