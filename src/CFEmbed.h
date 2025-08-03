#ifndef EMBED
#define EMBED

#include "FileManager/FileManager.h"
extern "C" {

enum MessageType:char{
    CONNECT, DRAW, ERROR, WARN, INFO, DOC, FILEREQ, FILERESP
};

/*
 * Remember to free the message after receive it in you app
 */

typedef struct Message{
    MessageType Type;
    void *Data;
}Message;

typedef struct Connect{
    void (*bridge)(const Message* m);
}Connect ;

typedef struct FileRequest{
    const char* FilePath;//reletive
}FileRequest;

typedef struct FileRespond{
    bool IsPath;
    const char* FilePath;// reletive, telling which file is this
    const char* Content;// can be path or whole file
}FileRespond;

void emb_Send_Message_To_CF(const Message* mes);

void emb_Connect(void (*func)(const Message*));

void emb_Send_Message(const Message *m);

void emb_Request_File(const char* filepath);

void emb_Respond_File_Req();

}
#endif
