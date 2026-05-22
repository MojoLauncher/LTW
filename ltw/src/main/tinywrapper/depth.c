//
// Created by whbex on 29.12.2025.
//

#include "egl.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "string.h"

const char * v_shader_source = ""
                             "#version 300 es\n"
                             "void main() {\n"
                             "gl_Position = vec4(2.0 * vec2(gl_VertexID % 2, gl_VertexID / 2) - 1.0, 0.0, 1.0);\n"
                             "}\0";
const char * f_shader_source = "#version 300 es\n"
                               "void main() {\n"
                               "}\0";

static GLuint current_program = 0;
static GLuint current_vao = 0;

static GLuint build_shader(GLuint program, GLenum type, const char* source){
    GLuint shader = es3_functions.glCreateShader(type);
    es3_functions.glShaderSource(shader, 1, &source, NULL);
    es3_functions.glCompileShader(shader);
    GLint status;
    char log[512];
    es3_functions.glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status){
        printf("LTW: Depth clear shader compile failed!\n");
        es3_functions.glGetShaderInfoLog(shader, sizeof(log), NULL,log);
        printf("LTW: Compile log: %s\n", log);
        return shader;
    }
    es3_functions.glAttachShader(program, shader);
    return shader;
}

void depthfix_init_program(){
    if (!current_context->clear_depth_emu) return;

    current_program = es3_functions.glCreateProgram();
    GLuint vertex = build_shader(current_program, GL_VERTEX_SHADER, v_shader_source);
    GLuint frag = build_shader(current_program, GL_FRAGMENT_SHADER, f_shader_source);
    es3_functions.glLinkProgram(current_program);
    es3_functions.glDeleteShader(vertex);
    es3_functions.glDeleteShader(frag);

    es3_functions.glGenVertexArrays(1, &current_vao);
}

void glClear(GLbitfield mask){
    if (!current_context->clear_depth_emu) goto real;
  //  printf("LTW: CLEAR BEGIN!!");
    if (mask != GL_DEPTH_BUFFER_BIT) goto real;

    // This is hard
    GLint depth_func;
    GLboolean depth_mask;
    GLboolean color_mask[4];
    es3_functions.glGetIntegerv(GL_DEPTH_FUNC, &depth_func);
    es3_functions.glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask);
    es3_functions.glGetBooleanv(GL_COLOR_WRITEMASK, color_mask);
    es3_functions.glDepthFunc(GL_ALWAYS);
    es3_functions.glEnable(GL_DEPTH_TEST);
    es3_functions.glDepthMask(GL_TRUE);
    es3_functions.glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    es3_functions.glUseProgram(current_program);
    es3_functions.glBindVertexArray(current_vao);
    es3_functions.glDrawArrays(GL_QUADS, 0, 4);
    es3_functions.glUseProgram(0);
    es3_functions.glBindVertexArray(0);
   // printf("LTW: CLEAR END!!");


    es3_functions.glDisable(GL_DEPTH_TEST);
    es3_functions.glDepthFunc(depth_func);
    es3_functions.glDepthMask(depth_mask);
   // es3_functions.glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    es3_functions.glColorMask(color_mask[0], color_mask[1], color_mask[2], color_mask[3]);

    return;
    real:
    es3_functions.glClear(mask);
}