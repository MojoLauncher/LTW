//
// Created by whbex on 22.11.2025.
//

#include <GL/gl.h>
#include <GL/glext.h>
#include "egl.h"
#include "proc.h"

void glBlendEquationi(GLuint buf, GLenum mode){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendequationi(buf, mode);
}
void glBlendEquationSeparatei(GLuint buf, GLenum modeRGB, GLenum modeAlpha){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendequationseparatei(buf, modeRGB, modeAlpha);
}
void glBlendFunci(GLuint buf, GLenum src, GLenum dst){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendfunci(buf, src, dst);
}
void glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->colormaski(index, r, g, b, a);
}
void glBlendFuncSeparatei(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendfuncseparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}
void glBlendEquationiARB(GLuint buf, GLenum mode){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendequationi(buf, mode);
}
void glBlendEquationSeparateiARB(GLuint buf, GLenum modeRGB, GLenum modeAlpha){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendequationseparatei(buf, modeRGB, modeAlpha);
}
void glBlendFunciARB(GLuint buf, GLenum src, GLenum dst){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendfunci(buf, src, dst);
}
void glBlendFuncSeparateiARB(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha){
    if(!current_context || !current_context->blending_indexed)
        return;
    current_context->blendfuncseparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}