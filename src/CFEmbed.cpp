#include "CFEmbed.h"
#include "CFBackend.h"
#include "FileManager/FileManager.h"
#include "FileProcessor/FileData.h"
#include "SectionData.h"
#include "Tools.h"
#include <cstdio>

void (*emb_Bridge)(cf_MessageType type, void* data);

void cf_Send_Message(cf_MessageType type, void* data){
    if(type == cf_MessageType::CONNECT){
        cf_Connect_msg* c = (cf_Connect_msg*)(data);
        emb_Bridge = c->bridge;
        SetRequestFileFunc(cf_Request_File);
        SetAppType(AppType::Embed);
        cf_Post_Message(cf_MessageType::CONNECT, nullptr);
    }else if(type == cf_MessageType::FILERESP){
        cf_FileResponse_msg* r = (cf_FileResponse_msg*) data;
        RespondFile(r->FilePath,r->Content, r->IsPath);
    }else if (type == cf_MessageType::LANG) {
        cf_LoadLang_msg *l = (cf_LoadLang_msg*)(data);
        string langname(l->LangName);
        LoadLangTemplate(langname);
    }else if(type == cf_MessageType::ENTRYFILE){
        cf_Entry_msg* e = (cf_Entry_msg*)(data);
        ProcessFile(e->FileName,e->language, CFFileType::SourceCode);
    }else if(type == cf_MessageType::RELOAD){
        Reload();
    }else if(type == cf_MessageType::TOGGLEOUTPUT){
        cf_ToggleOutput_msg* o = (cf_ToggleOutput_msg*)data;
        SetOutput(o->AcceptOutput);
    }
}

void cf_Post_Message(cf_MessageType type, void *data){
    // For readability we will not use emb_Bridge directly, using emb_SendMessage instead
    if(emb_Bridge == NULL){
        printf("Use emb_Connect() to connect CF and your app first");
        return;
    }
    emb_Bridge(type,data);
}

void cf_Request_File(const char *filepath){
    cf_FileRequest_msg *req = new cf_FileRequest_msg;
    req->FilePath = filepath;
    cf_Post_Message(cf_MessageType::FILEREQ, req);
}
