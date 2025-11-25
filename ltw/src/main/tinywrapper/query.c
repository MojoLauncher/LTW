//
// Created by whbex on 25.11.2025.
//

#include <GL/gl.h>
#include <GL/glext.h>
#include "egl.h"


void glGetQueryObjecti64v(GLuint id, GLenum pname, int64_t* params){
    if(!current_context || !current_context->query.available)
        return;
    current_context->query.getQueryObjecti64v(id, pname, params);
}
void glGetQueryObjectui64v(GLuint id, GLenum pname, uint64_t* params){
    if(!current_context || !current_context->query.available)
        return;
    current_context->query.getQueryObjectui64v(id, pname, params);
}
