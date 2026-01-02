//
// Created by whbex on 25.11.2025.
//

#include <GL/gl.h>
#include <GL/glext.h>
#include "egl.h"
#include "proc.h"

#define CTX_CHECK() if (!current_context) return;

void glGetQueryObjecti64v(GLuint id, GLenum pname, int64_t* params){
    CTX_CHECK();
    // May be not needed, added just in case
    if(!current_context->timer_query){
        *params = 1;
        return;
    }
   es3_functions.glGetQueryObjecti64vEXT(id, pname, params);
}

void glGetQueryObjectui64v(GLuint id, GLenum pname, uint64_t* params){
    CTX_CHECK();
    if(!current_context->timer_query){
        *params = 1;
        return;
    }
    es3_functions.glGetQueryObjectui64vEXT(id, pname, params);
}
void glQueryCounter(GLuint id, GLenum target){
    if(!current_context || !current_context->timer_query)
        return;
    es3_functions.glQueryCounterEXT(id, target);
}

void glGetQueryObjectiv( 	GLuint id,
                            GLenum name,
                            GLint * params) {
    CTX_CHECK();
    if(!current_context->timer_query && name == GL_QUERY_RESULT_AVAILABLE){
        *params = 0;
        return;
    }
    // This is not recommended but i don't care
    es3_functions.glGetQueryObjectuiv(id, name, (GLuint*)params);
}

void glGetQueryObjectuiv(GLuint id, GLenum name, GLuint* params){
    CTX_CHECK();
    if(!current_context->timer_query && name == GL_QUERY_RESULT_AVAILABLE){
        *params = 0;
        return;
    }
    es3_functions.glGetQueryObjectuiv(id, name, params);
}

// Just in case
void glBeginQuery(GLenum target, GLuint id){
    CTX_CHECK();
    if(!current_context->timer_query && target == GL_TIME_ELAPSED)
        return;
    es3_functions.glBeginQuery(target, id);
}
void glEndQuery(GLenum target){
    CTX_CHECK();
    if(!current_context->timer_query && target == GL_TIME_ELAPSED)
        return;
    es3_functions.glEndQuery(target);
}