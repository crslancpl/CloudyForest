#include "CFEmbed.h"
#include "CFBackend.h"
#include "FileManager/FileManager.h"
#include "FileProcessor/FileData.h"
#include "SectionData.h"
#include <cstdio>

void (*emb_Bridge)(MessageType type, void* data);

void emb_Send_Message_To_CF(MessageType type, void* data){
    if(type == MessageType::CONNECT){
        Connect* c = (Connect*)(data);
        emb_Connect( c->bridge);
    }else if(type == MessageType::FILERESP){
        FileRespond* r = (FileRespond*) data;
        RespondFile(r->FilePath,r->Content, r->IsPath);
    }else if (type == MessageType::LANG) {
        Lang *l = (Lang*)(data);
        string langname(l->LangName);
        LoadLangTemplate(langname);
    }else if(type == MessageType::ENTRYFILE){
        Entry* e = (Entry*)(data);
        ProcessFile(e->FileName,e->language, CFFileType::SourceCode);
    }else if(type == MessageType::RELOAD){
        Reload();
    }
}

void emb_Connect(void (*func)(MessageType, void*)){
    emb_Bridge = func;
    SetRequestFileFunc(emb_Request_File);
    SetAppType(AppType::Embed);
    emb_Send_Message(MessageType::CONNECT, nullptr);
}

void emb_Send_Message(MessageType type, void *data){
    // For readability we will not use emb_Bridge directly, using emb_SendMessage instead
    if(emb_Bridge == NULL){
        printf("Use emb_Connect() to connect CF and your app first");
        return;
    }
    emb_Bridge(type,data);
}

void emb_Request_File(const char *filepath){
    FileRequest *req = new FileRequest;
    req->FilePath = filepath;
    emb_Send_Message(MessageType::FILEREQ, req);
}
