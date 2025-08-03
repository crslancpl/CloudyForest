#include "CFEmbed.h"
#include "FileManager/FileManager.h"
#include "SectionData.h"
#include <cstdio>

void (*emb_Bridge)(const Message* m);

void emb_Send_Message_To_CF(const Message* mes){
    if(mes->Type == MessageType::CONNECT){
        Connect* c = (Connect*)(mes->Data);
        emb_Connect( c->bridge);
    }else if(mes->Type == MessageType::FILERESP){
        FileRespond* r = (FileRespond*) mes->Data;
        RespondFile(r->FilePath,r->Content, r->IsPath);
    }
}

void emb_Connect(void (*func)(const Message*)){
    emb_Bridge = func;
    SetRequestFileFunc(emb_Request_File);
    SetAppType(AppType::Embed);
}

void emb_Send_Message(const Message *m){
    // For readability we will not use emb_Bridge directly, using emb_SendMessage instead
    if(emb_Bridge == NULL){
        printf("Use emb_Connect() to connect CF and your app first");
        return;
    }
    emb_Bridge(m);
}

void emb_Request_File(const char *filepath){
    Message *mes = new Message;
    mes->Type = MessageType::FILEREQ;
    FileRequest *req = new FileRequest;
    req->FilePath = filepath;
    mes->Data = req;
    emb_Send_Message(mes);
}
