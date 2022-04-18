#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#ifndef QJS_INCLUDED
#define QJS_INCLUDED
#include <quickjs/quickjs.h>
#include <quickjs/quickjs-libc.h>
#endif
#define countof(x) (sizeof(x) / sizeof((x)[0]))

static JSValue js_console_log(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    if ( argc > 1 ) {
        for ( int i = 0; i <= argc - 1; i++) {
            printf("%s",JS_ToCString(ctx, args[i]));
        }
        printf("\n");
    }
    else {
        printf("%s\n",JS_ToCString(ctx, args[0]));
    }
    return JS_NewInt32(ctx, argc);
}

static JSValue js_console_assert(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
  if ( argc > 1 ) {
      bool success = JS_ToBool(ctx, args[0]);
      if ( !success ) {
          if ( !JS_IsFunction(ctx,args[1]) ) printf("%s\n",JS_ToCString(ctx, args[1]));
          else {
              return JS_Call(ctx,args[1],self,argc,args);
          }
      }
  }
  else {
      return JS_FALSE;
  }
  return JS_NULL;
}

static JSValue js_console_print(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    if ( argc > 1 ) {
        for ( int i = 0; i <= argc - 1; i++) {
            printf("%s",JS_ToCString(ctx, args[i]));
        }
    }
    else {
        printf("%s",JS_ToCString(ctx, args[0]));
    }
    return JS_NewInt32(ctx, argc);
}

static JSValue js_console_getpass(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    char *prompt = (char*) malloc(200 * sizeof(char) );
    char *data = (char*)malloc(200 * sizeof(char) );
    if ( argc < 1) strcpy(prompt,"_");
    else strcpy(prompt,JS_ToCString(ctx, args[0]));
    data = getpass(prompt);
    return JS_NewString(ctx,data);
}

static JSValue js_console_prompt(JSContext *ctx,JSValueConst self,int argc,JSValueConst *args) {
    char *prompt = (char*) malloc(200 * sizeof(char));
    char *data = (char*) malloc(200 * sizeof(char));
    if ( argc < 1 ) strcpy(prompt,"");
    else strcpy(prompt,JS_ToCString(ctx, args[0]));
    printf("%s",prompt);
    fgets(data,200 * sizeof(char),stdin);
    return JS_NewString(ctx,data);
}

static void js_console_init(JSContext *ctx,const char *name) {
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue console = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx,console,"log",JS_NewCFunction(ctx,js_console_log,"log",1));
    JS_SetPropertyStr(ctx,console,"assert",JS_NewCFunction(ctx,js_console_assert, "assert", 1));
    JS_SetPropertyStr(ctx,console,"print",JS_NewCFunction(ctx,js_console_print,"print",1));
    JS_SetPropertyStr(ctx,console,"prompt",JS_NewCFunction(ctx,js_console_prompt,"prompt",1));
    JS_SetPropertyStr(ctx,console,"getpass",JS_NewCFunction(ctx,js_console_getpass,"getpass",1));
    JS_SetPropertyStr(ctx,global,name,console);
    JS_FreeValue(ctx,global);
}