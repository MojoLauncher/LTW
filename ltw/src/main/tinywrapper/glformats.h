/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#ifndef POJAVLAUNCHER_GLFORMATS_H
#define POJAVLAUNCHER_GLFORMATS_H

#include <GLES3/gl3.h>

extern void convert_texture2d(GLenum type, GLenum format, GLuint width, GLuint height, GLvoid const* data, GLenum outtype, GLenum outformat, GLvoid** outdata);
extern bool make_format_non_generic(GLint *internalformat, GLenum* type, GLenum* format);
extern void pick_store_format(GLint *internalformat, GLenum* type, GLenum* format);
extern void pick_store_format_pure(GLint internalformat, GLenum* type, GLenum* format);

#endif //POJAVLAUNCHER_GLFORMATS_H
