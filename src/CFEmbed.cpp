#include "CFEmbed.h"
#include "CFBackend.h"
#include "FileManager/FileManager.h"
#include "FileProcessor/FileData.h"
#include "SectionData.h"
#include <cstdio>

void (*emb_Bridge)(Message* m);

void emb_Send_Message_To_CF(Message* mes){
    if(mes->Type == MessageType::CONNECT){
        Connect* c = (Connect*)(mes->Data);
        emb_Connect( c->bridge);
    }else if(mes->Type == MessageType::FILERESP){
        FileRespond* r = (FileRespond*) mes->Data;
        RespondFile(r->FilePath,r->Content, r->IsPath);
    }else if (mes->Type == MessageType::LANG) {
        Lang *l = (Lang*)(mes->Data);
        string langname(l->LangName);
        LoadLangTemplate(langname);
    }else if(mes->Type == MessageType::ENTRYFILE){
        Entry* e = (Entry*)(mes->Data);
        CFFile::ProcessFile(e->FileName, CFFileType::SourceCode);
    }else if(mes->Type == MessageType::RELOAD){
        Reload();
    }
}

void emb_Connect(void (*func)(Message*)){
    emb_Bridge = func;
    SetRequestFileFunc(emb_Request_File);
    SetAppType(AppType::Embed);
    Message *m = new Message();
    m->Type = MessageType::CONNECT;
    emb_Send_Message(m);
}

void emb_Send_Message(Message *m){
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
