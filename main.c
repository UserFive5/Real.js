#include "fs.h"
#include <curl/curl.h>
#include <stdio.h>
#include <errno.h>
#include "http.h"
#include <stdlib.h>
#include <string.h>
#include "sstring.h"
#include "system.h"
#include "console.h"
#include <quickjs/quickjs-libc.h>
#include <quickjs/quickjs.h>
char *readfile(const char *filename) {
    if ( filename == NULL ) filename = "";
    char *data = malloc(200*sizeof(char*));
    char *line = malloc(200*sizeof(char*));
    strcpy(data,"");
    strcpy(data,"");
    FILE *file = fopen(filename,"r");
    if (!file ) {
        printf("'%s': %s\n",filename,strerror(errno));
        exit(1);
    }
    while ( fgets(line,1024,file)!= NULL ) {
        sprintf(data,"%s%s",data,line);
    }
    fclose(file);
    return data;
};
static JSValue js_require(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    const char *filename = JS_ToCString(ctx,args[0]);
    char *source = readfile(filename);
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue module = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx,global,"module",module);
    JS_SetPropertyStr(ctx,module,"exports",JS_NULL);
    JSValue val = JS_Eval(ctx,source,strlen(source),filename,JS_EVAL_FLAG_STRICT | JS_EVAL_TYPE_GLOBAL );
    JSValue exports = JS_GetProperty(ctx,module,JS_NewAtom(ctx,"exports"));
    JS_DeleteProperty(ctx,global,JS_NewAtom(ctx,"module"),1);
    JS_FreeValue(ctx,val);
    JS_FreeValue(ctx,global);
    return exports;
}
int main(int argc,char **argv) {
    JSRuntime *rt = JS_NewRuntime();
    JSContext *ctx = JS_NewContext(rt);
    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx,global,"main",JS_NULL);
    JS_SetPropertyStr(ctx,global,"require",JS_NewCFunction(ctx,js_require,"require",1));
    JSValue args = JS_NewArray(ctx);
    if (argc >= 0) {
        int i;
        for(i = 0; i < argc; i++) {
            JS_SetPropertyUint32(ctx, args, i, JS_NewString(ctx, argv[i]));
        }
        JS_SetPropertyStr(ctx, global, "args", args);
    }
    js_console_init(ctx,"console");
    js_system_init(ctx, "@real/sys");
    js_fs_init(ctx, "@real/fs");
    js_http_init(ctx,"@real/http");
    char *source = readfile(argv[1]);
    JSValue res = JS_Eval(ctx,source,strlen(source),argv[1],JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_STRICT);
    if ( JS_IsException(res) ) js_std_dump_error(ctx);
    else {
        JSValue main = JS_GetPropertyStr(ctx,global,"main");
        if ( JS_IsFunction(ctx,main))  {
            JSValue _self = JS_NewObject(ctx);
            JSValue *args ;
            JS_Call(ctx,main,_self,0,args);
            JS_FreeValue(ctx,_self);
        }
        JS_FreeValue(ctx,main);
    }
    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx,res);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}