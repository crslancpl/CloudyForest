#include "CFBackend.h"

#include <stdio.h>
#include <string.h>

using namespace std;

int main(int argcount, char **args){
    printf("CloudyForest started >>\n");
    if (argcount == 1) {
        // CloudForest
        printf("No input file.\n If you want this to be a language server, run CloudForest with -server\n");
        return 0;
    }

    if (strcmp(args[1], "-server") == 0) {
        // Start as http server
        StartAsServer();
    }else if(strcmp(args[1], "-client") == 0){
        // Start as http client
        StartAsClient();
    }else if(strcmp(args[1], "-project") == 0){
        // Start as CloudyForest project
        CompileProject();
    }else{
        // CloudForest filename
        if(argcount == 3){
            QuickCompile(args[1], args[2]);
        }else{
            QuickCompile(args[1]);
        }
    }
}
