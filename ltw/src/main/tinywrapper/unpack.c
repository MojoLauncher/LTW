//
// Created by maks on 05.02.2026.
//

#include <GL/gl.h>
#include "egl.h"
#include "libraryinternal.h"
#include "unpack.h"

// TODO: implement getters for unpack state

static unpack_state_t default_state = {
    .alignment = 4,
    .skip_images = 0,
    .skip_pixels = 0,
    .skip_rows = 0,
    .image_height = 0,
    .row_length = 0,
};

INTERNAL void apply_unpack_state_to_hw(unpack_state_t *state) {
#define SET_HW_PARAM(NAME, ENUM) \
    if(state->NAME != current_context->hwunpack.NAME) { \
        es3_functions.glPixelStorei(ENUM, state->NAME); \
        current_context->hwunpack.NAME = state->NAME;                  \
    }
    SET_HW_PARAM(row_length, GL_UNPACK_ROW_LENGTH)
    SET_HW_PARAM(image_height, GL_UNPACK_IMAGE_HEIGHT)
    SET_HW_PARAM(skip_pixels, GL_UNPACK_SKIP_PIXELS)
    SET_HW_PARAM(skip_rows, GL_UNPACK_SKIP_ROWS)
    SET_HW_PARAM(skip_images, GL_UNPACK_SKIP_IMAGES)
    SET_HW_PARAM(alignment, GL_UNPACK_ALIGNMENT)
#undef SET_HW_PARAM
}

INTERNAL void apply_default_state_to_hw() {
    apply_unpack_state_to_hw(&default_state);
}

INTERNAL bool is_unpack_compatible(GLenum internalformat, GLenum format, GLenum type) {
    switch (internalformat) {
        case GL_RGB:
            if (format == GL_RGB && type == GL_UNSIGNED_BYTE) return true;
            if (format == GL_RGB && type == GL_UNSIGNED_SHORT_5_6_5) return true;
            break;
        case GL_RGBA:
            if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) return true;
            if (format == GL_RGBA && type == GL_UNSIGNED_SHORT_4_4_4_4) return true;
            if (format == GL_RGBA && type == GL_UNSIGNED_SHORT_5_5_5_1) return true;
            break;
        case GL_R8:
            if (format == GL_RED && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_R8_SNORM:
            if (format == GL_RED && type == GL_BYTE) return true;
            break;
        case GL_R16F:
            if (format == GL_RED && type == GL_HALF_FLOAT) return true;
            if (format == GL_RED && type == GL_FLOAT) return true;
            break;
        case GL_R32F:
            if (format == GL_RED && type == GL_FLOAT) return true;
            break;
        case GL_R8UI:
            if (format == GL_RED_INTEGER && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_R8I:
            if (format == GL_RED_INTEGER && type == GL_BYTE) return true;
            break;
        case GL_R16UI:
            if (format == GL_RED_INTEGER && type == GL_UNSIGNED_SHORT) return true;
            break;
        case GL_R16I:
            if (format == GL_RED_INTEGER && type == GL_SHORT) return true;
            break;
        case GL_R32UI:
            if (format == GL_RED_INTEGER && type == GL_UNSIGNED_INT) return true;
            break;
        case GL_R32I:
            if (format == GL_RED_INTEGER && type == GL_INT) return true;
            break;
        case GL_RG8:
            if (format == GL_RG && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_RG8_SNORM:
            if (format == GL_RG && type == GL_BYTE) return true;
            break;
        case GL_RG16F:
            if (format == GL_RG && type == GL_HALF_FLOAT) return true;
            if (format == GL_RG && type == GL_FLOAT) return true;
            break;
        case GL_RG32F:
            if (format == GL_RG && type == GL_FLOAT) return true;
            break;
        case GL_RG8UI:
            if (format == GL_RG_INTEGER && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_RG8I:
            if (format == GL_RG_INTEGER && type == GL_BYTE) return true;
            break;
        case GL_RG16UI:
            if (format == GL_RG_INTEGER && type == GL_UNSIGNED_SHORT) return true;
            break;
        case GL_RG16I:
            if (format == GL_RG_INTEGER && type == GL_SHORT) return true;
            break;
        case GL_RG32UI:
            if (format == GL_RG_INTEGER && type == GL_UNSIGNED_INT) return true;
            break;
        case GL_RG32I:
            if (format == GL_RG_INTEGER && type == GL_INT) return true;
            break;
        case GL_RGB8:
        case GL_SRGB8:
            if (format == GL_RGB && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_RGB565:
            if (format == GL_RGB && type == GL_UNSIGNED_BYTE) return true;
            if (format == GL_RGB && type == GL_UNSIGNED_SHORT_5_6_5) return true;
            break;
        case GL_RGB8_SNORM:
            if (format == GL_RGB && type == GL_BYTE) return true;
            break;
        case GL_R11F_G11F_B10F:
            if (format == GL_RGB && type == GL_UNSIGNED_INT_10F_11F_11F_REV) return true;
            if (format == GL_RGB && type == GL_HALF_FLOAT) return true;
            if (format == GL_RGB && type == GL_FLOAT) return true;
            break;
        case GL_RGB9_E5:
            if (format == GL_RGB && type == GL_UNSIGNED_INT_5_9_9_9_REV) return true;
            if (format == GL_RGB && type == GL_HALF_FLOAT) return true;
            if (format == GL_RGB && type == GL_FLOAT) return true;
            break;
        case GL_RGB16F:
            if (format == GL_RGB && type == GL_HALF_FLOAT) return true;
            if (format == GL_RGB && type == GL_FLOAT) return true;
            break;
        case GL_RGB32F:
            if (format == GL_RGB && type == GL_FLOAT) return true;
            break;
        case GL_RGB8UI:
            if (format == GL_RGB_INTEGER && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_RGB8I:
            if (format == GL_RGB_INTEGER && type == GL_BYTE) return true;
            break;
        case GL_RGB16UI:
            if (format == GL_RGB_INTEGER && type == GL_UNSIGNED_SHORT) return true;
            break;
        case GL_RGB16I:
            if (format == GL_RGB_INTEGER && type == GL_SHORT) return true;
            break;
        case GL_RGB32UI:
            if (format == GL_RGB_INTEGER && type == GL_UNSIGNED_INT) return true;
            break;
        case GL_RGB32I:
            if (format == GL_RGB_INTEGER && type == GL_INT) return true;
            break;
        case GL_RGBA8:
        case GL_SRGB8_ALPHA8:
            if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_RGBA8_SNORM:
            if (format == GL_RGBA && type == GL_BYTE) return true;
            break;
        case GL_RGB5_A1:
            if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) return true;
            if (format == GL_RGBA && type == GL_UNSIGNED_SHORT_5_5_5_1) return true;
            if (format == GL_RGBA && type == GL_UNSIGNED_INT_2_10_10_10_REV) return true;
            break;
        case GL_RGBA4:
            if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) return true;
            if (format == GL_RGBA && type == GL_UNSIGNED_SHORT_4_4_4_4) return true;
            break;
        case GL_RGB10_A2:
            if (format == GL_RGBA && type == GL_UNSIGNED_INT_2_10_10_10_REV) return true;
            break;
        case GL_RGBA16F:
            if (format == GL_RGBA && type == GL_HALF_FLOAT) return true;
            if (format == GL_RGBA && type == GL_FLOAT) return true;
            break;
        case GL_RGBA32F:
            if (format == GL_RGBA && type == GL_FLOAT) return true;
            break;
        case GL_RGBA8UI:
            if (format == GL_RGBA_INTEGER && type == GL_UNSIGNED_BYTE) return true;
            break;
        case GL_RGBA8I:
            if (format == GL_RGBA_INTEGER && type == GL_BYTE) return true;
            break;
        case GL_RGB10_A2UI:
            if (format == GL_RGBA_INTEGER && type == GL_UNSIGNED_INT_2_10_10_10_REV) return true;
            break;
        case GL_RGBA16UI:
            if (format == GL_RGBA_INTEGER && type == GL_UNSIGNED_SHORT) return true;
            break;
        case GL_RGBA16I:
            if (format == GL_RGBA_INTEGER && type == GL_SHORT) return true;
            break;
        case GL_RGBA32I:
            if (format == GL_RGBA_INTEGER && type == GL_INT) return true;
            break;
        case GL_RGBA32UI:
            if (format == GL_RGBA_INTEGER && type == GL_UNSIGNED_INT) return true;
            break;
        case GL_DEPTH_COMPONENT16:
            if(format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_SHORT) return true;
            if(format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_INT) return true;
            break;
        case GL_DEPTH_COMPONENT24:
            if(format == GL_DEPTH_COMPONENT && type == GL_UNSIGNED_INT) return true;
            break;
        case GL_DEPTH_COMPONENT32F:
            if(format == GL_DEPTH_COMPONENT && type == GL_FLOAT) return true;
            break;
        case GL_DEPTH24_STENCIL8:
            if(format == GL_DEPTH_STENCIL && type == GL_UNSIGNED_INT_24_8) return true;
            break;
        case GL_DEPTH32F_STENCIL8:
            if(format == GL_DEPTH_STENCIL && type == GL_FLOAT_32_UNSIGNED_INT_24_8_REV) return true;
            break;
        case GL_STENCIL_INDEX8:
            if(format == GL_STENCIL_INDEX && type == GL_UNSIGNED_BYTE) return true;
            break;
        default:
            printf("LTW: unknown internal format %x\n", internalformat);
            return false;
    }
    return false;
}

void glPixelStoref( 	GLenum pname,
                       GLfloat param) {
    if(current_context == NULL) return;
    switch (pname) {
        case GL_UNPACK_SWAP_BYTES:
            current_context->unpack.swap_bytes = param == GL_TRUE;
            break;
        case GL_UNPACK_LSB_FIRST:
            current_context->unpack.lsb_first = param == GL_TRUE;
            break;
        case GL_UNPACK_ROW_LENGTH:
            current_context->unpack.row_length = (GLint) param;
            break;
        case GL_UNPACK_IMAGE_HEIGHT:
            current_context->unpack.image_height = (GLint) param;
            break;
        case GL_UNPACK_SKIP_PIXELS:
            current_context->unpack.skip_pixels = (GLint) param;
            break;
        case GL_UNPACK_SKIP_ROWS:
            current_context->unpack.skip_rows = (GLint) param;
            break;
        case GL_UNPACK_SKIP_IMAGES:
            current_context->unpack.skip_images = (GLint) param;
            break;
        case GL_PACK_ALIGNMENT:
            current_context->unpack.alignment = (GLint) param;
        default:
            es3_functions.glPixelStorei(pname, (GLint) param);
    }
}

void glPixelStorei( 	GLenum pname,
                       GLint param) {
    switch (pname) {
        case GL_UNPACK_SWAP_BYTES:
            current_context->unpack.swap_bytes = param == GL_TRUE;
            break;
        case GL_UNPACK_LSB_FIRST:
            current_context->unpack.lsb_first = param == GL_TRUE;
            break;
        case GL_UNPACK_ROW_LENGTH:
            current_context->unpack.row_length = (GLint) param;
            break;
        case GL_UNPACK_IMAGE_HEIGHT:
            current_context->unpack.image_height = (GLint) param;
            break;
        case GL_UNPACK_SKIP_PIXELS:
            current_context->unpack.skip_pixels = (GLint) param;
            break;
        case GL_UNPACK_SKIP_ROWS:
            current_context->unpack.skip_rows = (GLint) param;
            break;
        case GL_UNPACK_SKIP_IMAGES:
            current_context->unpack.skip_images = (GLint) param;
            break;
        case GL_PACK_ALIGNMENT:
            current_context->unpack.alignment = (GLint) param;
        default:
            es3_functions.glPixelStorei(pname, (GLint) param);
    }
}
