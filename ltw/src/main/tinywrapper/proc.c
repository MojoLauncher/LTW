/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */
#include <EGL/egl.h>
#include <GLES3/gl31.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <android/log.h>
#include <string.h>
#include "proc.h"
#include "egl.h"
#include "libraryinternal.h"
#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GL/glext.h"

INTERNAL eglMustCastToProperFunctionPointerType (*host_eglGetProcAddress)(const char *procname);
INTERNAL es3_functions_t es3_functions;

static void error_sysegl() {
    __android_log_print(ANDROID_LOG_ERROR, "LTWInit", "Failed to load system EGL: %s", dlerror());
   // abort();
   return;
}

static void error_init(const char* functionName) {
    __android_log_print(ANDROID_LOG_ERROR, "LTWInit", "Failed to load function \"%s\"", functionName);
  //  abort();
  return;
}

static void init_es3_proc() {
#define GLESFUNC(name, type) es3_functions.name = (type)host_eglGetProcAddress(#name); if(es3_functions.name == NULL) error_init(#name);
#include "es3_functions.h"
#undef GLESFUNC
#define GLESFUNC(name, type) es3_functions.name = (type)host_eglGetProcAddress(#name);
#include "es3_extended.h"
#undef GLESFUNC
}

__attribute__((constructor, used)) void proc_init(){
    const char* systemEglPath = "libEGL.so";
    const char* eglPath = getenv("LIBGL_EGL") != NULL ? getenv("LIBGL_EGL") : systemEglPath;
    int flags = RTLD_LAZY | RTLD_LOCAL;
    void* eglHandle = dlopen(eglPath, flags);
    if(eglHandle == NULL){
        printf("LTWInit: failed loading custom libEGL, using default\n");
        eglHandle = dlopen(systemEglPath, flags);
        if(eglHandle == NULL)
            error_sysegl();
    }
    host_eglGetProcAddress = dlsym(eglHandle, "eglGetProcAddress");
    if(host_eglGetProcAddress == NULL) error_sysegl();
    init_egl();
    init_es3_proc();
}

// This is exported for it to be automatically picked up by LWJGL's symbol resolver.
__attribute__((used)) eglMustCastToProperFunctionPointerType glXGetProcAddress(const char *procname) {
    return eglGetProcAddress(procname);
}

static eglMustCastToProperFunctionPointerType resolve_stub(const char* procname) {
    size_t procnamelen = strlen(procname);
    size_t stublen = procnamelen + 6;
    char stub_procname[stublen];
    memcpy(stub_procname, "stub_", 5);
    memcpy(stub_procname + 5, procname, procnamelen);
    stub_procname[stublen] = 0;
    return dlsym(NULL, stub_procname);
}

static void unknown_stub() {
    printf("Unknown stub! Aborting to produce backtrace...\n");
   // abort();
   return;
}

eglMustCastToProperFunctionPointerType eglGetProcAddress(const char *procname) {
    // EGL functions that we implement.
    // All of the other platform EGL functions will be redirected into Android's default EGL implementation.
    if(!strncmp(procname, "egl", 3)) {
        if(!strcmp("eglCreateContext", procname)) return (eglMustCastToProperFunctionPointerType) eglCreateContext;
        if(!strcmp("eglDestroyContext", procname)) return (eglMustCastToProperFunctionPointerType) eglDestroyContext;
        if(!strcmp("eglMakeCurrent", procname)) return (eglMustCastToProperFunctionPointerType) eglMakeCurrent;
    }
    // If the function doesn't start with "gl", don't even bother, pass through immediately.
    if(strncmp(procname, "gl", 2) != 0) {
        return host_eglGetProcAddress(procname);
    }

#define GLESOVERRIDE(name)                                        \
    if(!strcmp(procname, #name)) {                                \
        printf("LTW: Overridden %s\n", #name);                        \
        return (eglMustCastToProperFunctionPointerType) name;     \
    }
#include "es3_overrides.h"
#undef GLESOVERRIDE
#define GLESFUNC(name, unused_type) \
    if(!strcmp(procname, #name)) {                                \
        return (eglMustCastToProperFunctionPointerType) es3_functions.name;     \
    }
#include "es3_functions.h"
#include "es3_extended.h"
#undef GLESFUNC

    // Route ALL unresolved functions to stubs. Just in case if they get routed into the driver and get
    // stubbed there.
    eglMustCastToProperFunctionPointerType stubFunction = resolve_stub(procname);
    if(stubFunction == NULL) return unknown_stub;
    return stubFunction;
}
