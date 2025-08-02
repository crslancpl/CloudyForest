#include "FileManager/FileManager.h"
#include "CFBackend.h"

#include <stdio.h>
#include <string>
#include <string.h>

using namespace std;

int main(int ArgCount, char **Args){
    printf("CloudyForest started >>\n");
    if (ArgCount == 1) {
        // CloudForest
        printf("No input file.\n If you want this to be a language server, run CloudForest with -server\n");
        return 0;
    }

    if (strcmp(Args[1], "-server") == 0) {
        // Start as http server
        StartAsServer();
    }else if(strcmp(Args[1], "-client") == 0){
        // Start as http client
        StartAsClient();
    }else if(strcmp(Args[1], "-project") == 0){
        // Start as CloudyForest project
        CompileProject();
    }else{
        // CloudForest filename
        //
    }
}
