//
// Created by whbex on 25.11.2025.
//

#include <GL/gl.h>
#include <GL/glext.h>
#include "egl.h"
#include "proc.h"


void glGetQueryObjecti64v(GLuint id, GLenum pname, int64_t* params){
    if(!current_context || !current_context->timer_query)
        return;
   es3_functions.glGetQueryObjecti64vEXT(id, pname, params);
}
void glGetQueryObjectui64v(GLuint id, GLenum pname, uint64_t* params){
    if(!current_context || !current_context->timer_query)
        return;
    es3_functions.glGetQueryObjectui64vEXT(id, pname, params);
}
void glQueryCounter(GLuint id, GLenum target){
    if(!current_context || !current_context->timer_query)
        return;
    es3_functions.glQueryCounterEXT(id, target);
}
