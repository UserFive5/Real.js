//#include <curl/easy.h>
#include <quickjs/quickjs.h>
#include <stdio.h>
#include <errno.h>
#include <curl/curl.h>
#include <quickjs/quickjs-libc.h>
#define countof(x) (sizeof(x) / sizeof((x)[0]))
static char *last_res = "";
static size_t write_res(void *res,size_t size,size_t nmemb,void *userdata) {
    last_res = (char*) res;
    return size * nmemb;
}

static JSValue js_http_fetch(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    const char *url = JS_ToCString(ctx,args[0]);
    char *res = (char*) malloc(200* sizeof(char));
    CURL *req = curl_easy_init();
    curl_easy_setopt(req, CURLOPT_URL,url);
    curl_easy_setopt(req,CURLOPT_WRITEFUNCTION,write_res);
    curl_easy_perform(req);
    curl_easy_cleanup(req);
    return JS_NewString(ctx,last_res);
}

static const JSCFunctionListEntry js_http_functions[] = {
    JS_CFUNC_DEF("fetch",1,js_http_fetch),
};

static int js_init_http(JSContext *ctx,JSModuleDef *module) {
    return JS_SetModuleExportList(ctx,  module,js_http_functions,countof(js_http_functions));
}
static void js_http_init(JSContext *ctx,const char *name) {
    JSModuleDef *module = JS_NewCModule(ctx,name, js_init_http);
    JS_AddModuleExportList(ctx,  module,js_http_functions,countof(js_http_functions));
}