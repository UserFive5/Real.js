#include <SDL2/SDL_video.h>
#include <quickjs/quickjs.h>
#include <stdio.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <curl/curl.h>
#include <quickjs/quickjs-libc.h>
#define countof(x) (sizeof(x) / sizeof((x)[0]))
static SDL_Window *global_window;

static JSValue js_sdl2_init(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {

}

static JSCFunctionListEntry js_sdl2_functions[] = {
    JS_CFUNC_DEF("init",1,js_sdl2_init),
};

static int js_init_sdl2(JSContext *ctx,JSModuleDef *module) {
    return JS_SetModuleExportList(ctx,module,js_sdl2_functions,countof(js_sdl2_functions));
}
static void js_http_init(JSContext *ctx,const char *name) {
    JSModuleDef *module = JS_NewCModule(ctx,name, js_init_sdl2);

}