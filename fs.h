#include <quickjs/quickjs-libc.h>
#include <quickjs/quickjs.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSValue js_fs_read(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    const char *filename = JS_ToCString(ctx,args[0]);
    FILE *file = fopen(filename,"rb");
    char *data = (char*) malloc(200 * sizeof(char));
    if (!file ) {
        JSValue ex = JS_ThrowReferenceError(ctx,"'%s': %s\n",filename,strerror(errno));
    }
    else {
      int c;
      while ( ( c = fgetc(file) ) != EOF ) {
        sprintf(data,"%s%c",data,c);
      }
      return JS_NewString(ctx,data);
    }
    return JS_NULL;
}


static const JSCFunctionListEntry js_fs_functions[] = {
    JS_CFUNC_DEF("readFile",1,js_fs_read),
};

static int js_init_fs(JSContext *ctx,JSModuleDef *module) {
    return JS_SetModuleExportList(ctx,  module,js_fs_functions,countof(js_fs_functions));
}
static void js_fs_init(JSContext *ctx,const char *name) {
    JSModuleDef *module = JS_NewCModule(ctx,name, js_init_fs);
    JS_AddModuleExportList(ctx,  module,js_fs_functions,countof(js_fs_functions));
}