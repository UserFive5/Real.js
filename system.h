#include <quickjs/quickjs-libc.h>
#include <quickjs/quickjs.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#define countof(x) (sizeof(x) / sizeof((x)[0]))
static JSValue js_system_getenv(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    char *r = getenv(JS_ToCString(ctx, args[0]));
    if ( r == NULL ) return JS_NULL;
    else return JS_NewString(ctx,r);
}
static JSValue js_system_setenv(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    const char *name = JS_ToCString(ctx,args[0]);
    const char *value = JS_ToCString(ctx,args[1]);
    int res = setenv(name,value,1);
    return JS_NewInt32(ctx,res);
}
static JSValue js_system_environ(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    char **envp;
    const char *name, *p, *value;
    JSValue obj;
    uint32_t idx;
    size_t name_len;
    JSAtom atom;
    int ret;
    obj = JS_NewObject(ctx);
    if (JS_IsException(obj))
        return JS_EXCEPTION;
    envp = __environ;
    for(idx = 0; envp[idx] != NULL; idx++) {
        name = envp[idx];
        p = strchr(name, '=');
        name_len = p - name;if (!p)
            continue;
        value = p + 1;
        atom = JS_NewAtomLen(ctx, name, name_len);
        if (atom == JS_ATOM_NULL)
            goto fail;
        ret = JS_DefinePropertyValue(ctx, obj, atom, JS_NewString(ctx, value),
                                     JS_PROP_C_W_E);
        JS_FreeAtom(ctx, atom);
        if (ret < 0)
            goto fail;
    }
    return obj;
 fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}
static JSValue js_system_exit(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    int code = 0;
    if ( JS_IsNumber(args[0]) ) JS_ToInt32(ctx,&code,args[0]);
    exit(code);
    return JS_NULL;
}
static JSValue js_system_popen(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    char *data = (char*) malloc(200 * sizeof(char));
    const char *command = JS_ToCString(ctx, args[0]);
    FILE *file = popen(command,"r");
    if (!file ) {
        JSValue ex = JS_ThrowReferenceError(ctx,"'%s': %s\n",command,strerror(errno));
        return ex;
    }
    else {
      int c;
      while ( ( c = fgetc(file) ) != EOF ) {
        sprintf(data,"%s%c",data,c);
      }
      return JS_NewString(ctx,data);
    }
}
static const JSCFunctionListEntry js_system_functions[] = {
    JS_CFUNC_DEF("getenv",1,js_system_getenv),
    JS_CFUNC_DEF("setenv",1,js_system_setenv),
    JS_CFUNC_DEF("environ",1,js_system_environ),
    JS_CFUNC_DEF("popen",1,js_system_popen),
    JS_CFUNC_DEF("exit",1,js_system_exit),
};

static int js_init_system(JSContext *ctx,JSModuleDef *module) {
    return JS_SetModuleExportList(ctx,  module,js_system_functions,countof(js_system_functions));
}
static void js_system_init(JSContext *ctx,const char *name) {
    JSModuleDef *module = JS_NewCModule(ctx,name, js_init_system);
    JS_AddModuleExportList(ctx,  module,js_system_functions,countof(js_system_functions));
}
