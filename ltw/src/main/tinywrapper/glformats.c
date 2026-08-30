/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#include <stdbool.h>
#include "egl.h"
#include "glformats.h"
#include "libraryinternal.h"
#include "GL/gl.h"
#include "conversion.h"
#include <stdio.h>
#include <string.h>

#include <android/log.h>

#if defined(__aarch64__) || defined(__arm__)
#include <arm_neon.h>
#define ARM_NEON
#endif

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "LTW", __VA_ARGS__)

static GLint pick_depth_internalformat(GLenum* type, bool* convert) {
    switch (*type) {
        case GL_UNSIGNED_SHORT:
        case GL_UNSIGNED_INT:
            return GL_DEPTH_COMPONENT16;
        case GL_FLOAT:
            return GL_DEPTH_COMPONENT32F;
        default:
            *convert = true;
            *type = GL_UNSIGNED_SHORT;
            return GL_DEPTH_COMPONENT16;
    }
}

static GLint pick_depth_stencil_internalformat(GLenum* type, bool* convert) {
    switch (*type) {
        case GL_UNSIGNED_INT_24_8:
            return GL_DEPTH24_STENCIL8;
        case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
            return GL_DEPTH32F_STENCIL8;
        default:
            *convert = true;
            *type = GL_UNSIGNED_INT_24_8;
            return GL_DEPTH24_STENCIL8;
    }
}

static GLint pick_red_internalformat(GLenum* type, bool* convert) {
    switch(*type) {
        case GL_BYTE:
            return GL_R8_SNORM;
        case GL_UNSIGNED_BYTE:
            return GL_R8;
        case GL_HALF_FLOAT:
            return GL_R16F;
        case GL_FLOAT:
            return GL_R32F;
        default:
            *convert = true;
            *type = GL_UNSIGNED_BYTE;
            return GL_R8;
    }
}

static GLint pick_rg_internalformat(GLenum* type, bool* convert) {
    switch(*type) {
        case GL_BYTE:
            return GL_RG8_SNORM;
        case GL_UNSIGNED_BYTE:
            return GL_RG8;
        case GL_HALF_FLOAT:
            return GL_RG16F;
        case GL_FLOAT:
            return GL_RG32F;
        default:
            *convert = true;
            *type = GL_UNSIGNED_BYTE;
            return GL_RG8;
    }
}

INTERNAL bool make_format_non_generic(GLint *internalformat, GLenum* type, GLenum* format) {
    bool convert_data = false;
    switch (*internalformat) {
        case GL_DEPTH_COMPONENT32:
            // Select the equivalent type (32f for float, 24 for int)
            if(*type == GL_FLOAT) {
                *internalformat = GL_DEPTH_COMPONENT32F;
            } else {
                *internalformat = GL_DEPTH_COMPONENT24;
                if(*type != GL_UNSIGNED_INT) convert_data = true;
                *type = GL_UNSIGNED_INT;
            }
            break;
        case GL_DEPTH_COMPONENT:
            *internalformat =  pick_depth_internalformat(type, &convert_data);
            break;
        case GL_DEPTH_STENCIL:
            *internalformat = pick_depth_stencil_internalformat(type, &convert_data);
            break;
        case GL_RED:
            *internalformat = pick_red_internalformat(type, &convert_data);
            break;
        case GL_RG:
            *internalformat = pick_rg_internalformat(type, &convert_data);
            break;
    }
    return convert_data;
}

INTERNAL void pick_store_format_pure(GLint internalformat, GLenum* type, GLenum* format) {
    // GLES 3.2 format table
    switch (internalformat) {
        // Unsized formats. In this case we always prefer the "byte" versions of them (meaning 32bit/24bit color)
        case GL_RGB: *format=GL_RGB; *type = GL_UNSIGNED_BYTE; break;
        case GL_RGBA: *format=GL_RGBA; *type = GL_UNSIGNED_BYTE; break;
        case GL_LUMINANCE_ALPHA: *format=GL_LUMINANCE_ALPHA; *type = GL_UNSIGNED_BYTE; break;
        case GL_LUMINANCE: *format=GL_LUMINANCE; *type = GL_UNSIGNED_BYTE; break;
        case GL_ALPHA: *format=GL_ALPHA; *type = GL_UNSIGNED_BYTE; break;
            // Sized Formats
        case GL_R8: *format=GL_RED; *type=GL_UNSIGNED_BYTE; break;
        case GL_R8_SNORM: *format=GL_RED; *type=GL_BYTE; break;
        case GL_R16F: *format=GL_RED; *type=GL_HALF_FLOAT; break;
        case GL_R32F: *format=GL_RED; *type=GL_FLOAT; break;
        case GL_R8UI: *format=GL_RED_INTEGER; *type=GL_UNSIGNED_BYTE; break;
        case GL_R8I: *format=GL_RED_INTEGER; *type=GL_BYTE; break;
        case GL_R16UI: *format=GL_RED_INTEGER; *type=GL_UNSIGNED_SHORT; break;
        case GL_R16I: *format=GL_RED_INTEGER; *type=GL_SHORT; break;
        case GL_R32UI: *format=GL_RED_INTEGER; *type=GL_UNSIGNED_INT; break;
        case GL_R32I: *format=GL_RED_INTEGER; *type=GL_INT; break;
        case GL_RG8: *format=GL_RG; *type=GL_UNSIGNED_BYTE; break;
        case GL_RG8_SNORM: *format=GL_RG; *type=GL_BYTE; break;
        case GL_RG16F: *format=GL_RG; *type=GL_HALF_FLOAT; break;
        case GL_RG32F: *format=GL_RG; *type=GL_FLOAT; break;
        case GL_RG8UI: *format=GL_RG_INTEGER; *type=GL_UNSIGNED_BYTE; break;
        case GL_RG8I: *format=GL_RG_INTEGER; *type=GL_BYTE; break;
        case GL_RG16UI: *format=GL_RG_INTEGER; *type=GL_UNSIGNED_SHORT; break;
        case GL_RG16I: *format=GL_RG_INTEGER; *type=GL_SHORT; break;
        case GL_RG32UI: *format=GL_RG_INTEGER; *type=GL_UNSIGNED_INT; break;
        case GL_RG32I: *format=GL_RG_INTEGER; *type=GL_INT; break;
        case GL_RGB8: *format=GL_RGB; *type=GL_UNSIGNED_BYTE; break;
        case GL_SRGB8: *format=GL_RGB; *type=GL_UNSIGNED_BYTE; break;
        case GL_RGB565: *format=GL_RGB; *type=GL_UNSIGNED_BYTE; break;
        case GL_RGB8_SNORM: *format=GL_RGB; *type=GL_BYTE; break;
        case GL_R11F_G11F_B10F: *format=GL_RGB; *type=GL_FLOAT; break;
        case GL_RGB9_E5: *format=GL_RGB; *type=GL_UNSIGNED_INT_5_9_9_9_REV; break;
        case GL_RGB16F: *format=GL_RGB; *type=GL_HALF_FLOAT; break;
        case GL_RGB32F: *format=GL_RGB; *type=GL_FLOAT; break;
        case GL_RGB8UI: *format=GL_RGB_INTEGER; *type=GL_UNSIGNED_BYTE; break;
        case GL_RGB8I: *format=GL_RGB_INTEGER; *type=GL_BYTE; break;
        case GL_RGB16UI: *format=GL_RGB_INTEGER; *type=GL_UNSIGNED_SHORT; break;
        case GL_RGB16I: *format=GL_RGB_INTEGER; *type=GL_SHORT; break;
        case GL_RGB32UI: *format=GL_RGB_INTEGER; *type=GL_UNSIGNED_INT; break;
        case GL_RGB32I: *format=GL_RGB_INTEGER; *type=GL_INT; break;
        case GL_RGBA8: *format=GL_RGBA; *type=GL_UNSIGNED_BYTE; break;
        case GL_SRGB8_ALPHA8: *format=GL_RGBA; *type=GL_UNSIGNED_BYTE; break;
        case GL_RGBA8_SNORM: *format=GL_RGBA; *type=GL_BYTE; break;
        case GL_RGB5_A1: *format=GL_RGBA; *type=GL_UNSIGNED_BYTE; break;
        case GL_RGBA4: *format=GL_RGBA; *type=GL_UNSIGNED_BYTE; break;
        case GL_RGB10_A2: *format=GL_RGBA; *type=GL_UNSIGNED_INT_2_10_10_10_REV; break;
        case GL_RGBA16F: *format=GL_RGBA; *type=GL_HALF_FLOAT; break;
        case GL_RGBA32F: *format=GL_RGBA; *type=GL_FLOAT; break;
        case GL_RGBA8UI: *format=GL_RGBA_INTEGER; *type=GL_UNSIGNED_BYTE; break;
        case GL_RGBA8I: *format=GL_RGBA_INTEGER; *type=GL_BYTE; break;
        case GL_RGB10_A2UI: *format=GL_RGBA_INTEGER; *type=GL_UNSIGNED_INT_2_10_10_10_REV; break;
        case GL_RGBA16UI: *format=GL_RGBA_INTEGER; *type=GL_UNSIGNED_SHORT; break;
        case GL_RGBA16I: *format=GL_RGBA_INTEGER; *type=GL_SHORT; break;
        case GL_RGBA32I: *format=GL_RGBA_INTEGER; *type=GL_INT; break;
        case GL_RGBA32UI: *format=GL_RGBA_INTEGER; *type=GL_UNSIGNED_INT; break;
            // Sized depth formats
        case GL_DEPTH_COMPONENT16: *format = GL_DEPTH_COMPONENT; *type = GL_UNSIGNED_SHORT; break;
        case GL_DEPTH_COMPONENT24: *format = GL_DEPTH_COMPONENT; *type = GL_UNSIGNED_INT; break;
        case GL_DEPTH_COMPONENT32F: *format = GL_DEPTH_COMPONENT; *type = GL_FLOAT; break;
        case GL_DEPTH24_STENCIL8: *format = GL_DEPTH_STENCIL; *type = GL_UNSIGNED_INT_24_8; break;
        case GL_DEPTH32F_STENCIL8: *format = GL_DEPTH_STENCIL; *type = GL_FLOAT_32_UNSIGNED_INT_24_8_REV; break;
        case GL_STENCIL_INDEX8: *format = GL_STENCIL_INDEX; *type = GL_UNSIGNED_BYTE; break;
        default:
            printf("LTW: pick_store_format fallthrough: %x\n", internalformat);
    }
}

INTERNAL void pick_store_format(GLint *internalformat, GLenum* type, GLenum* format) {
    // Workarounds!
    switch (*internalformat) {
        // Two legacy GL formats. From testing, OptiFine wants these to be floats.
        case GL_RGBA12:
        case GL_RGBA16:
            *internalformat = GL_RGBA16F;
            break;
        // Always use 32-bit float depth for GL_DEPTH_COMPONENT, because the 16-bit depth buffer
        // causes z-fighting in the distance
        case GL_DEPTH_COMPONENT:
            *internalformat = GL_DEPTH_COMPONENT32F;
            break;
        // This appears to be one of the legacy formats from the FPE days, and is not even
        // listed in the format tables in 3.3 core. Still, MC uses it for the depth buffers.
        case GL_DEPTH_COMPONENT32:
            *internalformat = GL_DEPTH_COMPONENT32F;
            break;
        // Unsized depth-stencil. Not sure what uses it but we'll fall back to 24-bit + 8-bit stencil
        case GL_DEPTH_STENCIL:
            *internalformat = GL_DEPTH24_STENCIL8;
            break;
        // Color-renderability workarounds. Yes, those probably decrease performance but they sure do improve compatibility with shaderpacks!
        // Ideally these should only be used on framebuffers, but whatever.
        // In GL, the SNORM formats are color-renderable and support signed normalized values from -1 to 1.
        // Sadly, the only alternative format with the same capabilities that *is* color-renderable in ES is 16-bit float.
        // So, switch to that.
        case GL_R8_SNORM:
            *internalformat = GL_R16F;
        case GL_RG8_SNORM:
            *internalformat = GL_RG16F;
        case GL_RGBA8_SNORM:
            *internalformat = GL_RGBA16F;
            break;
        // Fun fact: the only color renderable formats in GLES that have 3 components are
        // GL_R11F_G11F_B10F and GL_RGB8. And only GL_R11F_G11F_B10F supports signed values.
        case GL_RGB8I:
        case GL_RGB16I:
        case GL_RGB32I:
        case GL_RGB8_SNORM:
        case GL_RGB12:
        case GL_RGB16:
        case GL_RGB16F:
        case GL_RGB32F:
            *internalformat = GL_R11F_G11F_B10F;
        case GL_RGB8UI:
            *internalformat = GL_RGB8;
            break;
    }
    pick_store_format_pure(*internalformat, type, format);
}

INTERNAL bool is_type_basic(GLenum type) {
    switch (type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_FLOAT:
        case GL_HALF_FLOAT:
            return true;
        default:
            return false;
    }
}

static bool is_rgba8_special(GLenum format, GLenum type) {
    switch (format) {
        case GL_RGBA:
        case GL_BGRA:
        case GL_ABGR_EXT:
            break;
        default:
            return false;
    }
    switch (type) {
        case GL_UNSIGNED_INT_8_8_8_8_REV:
        case GL_UNSIGNED_INT_8_8_8_8:
            return true;
        default:
            return false;
    }
}

static int num_color_channels(GLenum format) {
    switch (format) {
        case GL_RED:
        case GL_RED_INTEGER:
            return 1;
        case GL_RG:
        case GL_RG_INTEGER:
            return 2;
        case GL_RGB:
        case GL_BGR:
        case GL_RGB_INTEGER:
        case GL_BGR_INTEGER:
            return 3;
        case GL_RGBA:
        case GL_RGBA_INTEGER:
        case GL_BGRA:
        case GL_BGRA_INTEGER:
        case GL_ABGR_EXT:
            return 4;
        default:
            return -1;
    }
}

static int num_channel_bits(GLenum type) {
    switch (type) {
        case GL_BYTE: return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        case GL_SHORT: return sizeof(GLshort);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_INT: return sizeof(GLint);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_HALF_FLOAT: return sizeof(GLhalf);
        case GL_FLOAT: return sizeof(GLfloat);
        default:
            return -1;
    }
}

#define INTEGER_NARROW_NORMALIZE(INTYPE, OUTTYPE) \
static inline OUTTYPE normalize_##INTYPE##_to_##OUTTYPE(INTYPE input) { \
    const unsigned input_bits = sizeof(INTYPE) * 8; \
    const unsigned output_bits = sizeof(INTYPE) * 8; \
    return (OUTTYPE)(input >> (input_bits - output_bits)); \
}

INTEGER_NARROW_NORMALIZE(GLuint, GLubyte)
INTEGER_NARROW_NORMALIZE(GLint, GLbyte)
INTEGER_NARROW_NORMALIZE(GLushort, GLubyte)
INTEGER_NARROW_NORMALIZE(GLshort, GLbyte)
INTEGER_NARROW_NORMALIZE(GLuint, GLushort)
INTEGER_NARROW_NORMALIZE(GLint, GLshort)

static inline GLuint normalize_GLubyte_to_GLuint(GLubyte input) {
    return input | input << 8 | input << 16 | input << 24;
}

static inline GLuint normalize_GLushort_to_GLuint(GLushort input) {
    return input | input << 16;
}

static inline GLuint normalize_GLubyte_to_GLushort(GLubyte input) {
    return input | input << 8;
}

#define FLOAT_NARROW_NORMALIZE(INTYPE, INMAX) \
static inline GLfloat normalize_##INTYPE##_to_GLfloat(INTYPE input) { \
    return (float)input / (float)INMAX; \
} \

#define FLOAT_EXPAND_NORMALIZE(OUTTYPE, OUTMAX) \
static inline OUTTYPE normalize_GLfloat_to_##OUTTYPE(GLfloat input) { \
    return (OUTTYPE) (input * (float)OUTMAX); \
} \

#define FLOAT_NORMALIZE(...) FLOAT_NARROW_NORMALIZE(__VA_ARGS__) FLOAT_EXPAND_NORMALIZE(__VA_ARGS__)

FLOAT_NORMALIZE(GLubyte, UINT8_MAX)
FLOAT_NORMALIZE(GLbyte, INT8_MAX)
FLOAT_NORMALIZE(GLushort, UINT16_MAX)
FLOAT_NORMALIZE(GLshort, INT16_MAX)
FLOAT_NORMALIZE(GLuint, UINT32_MAX)
FLOAT_NORMALIZE(GLint, INT32_MAX)

#define CONVERT(INTYPE, OUTTYPE, INCHANNELS, OUTCHANNELS, WRITER) \
static void convert_##INTYPE##_##INCHANNELS##_to_##OUTTYPE##_##OUTCHANNELS(GLuint unpack_row_length, GLuint height, const INTYPE* in, OUTTYPE* out, GLuint outw, const unsigned char swizzle[4]) { \
    GLint s = sizeof(INTYPE); \
    GLint a = current_context->unpack.alignment; \
    GLuint k; \
    if(s >= a) k = unpack_row_length * INCHANNELS; \
    else k = (GLuint) (((double)a / s) * ceil((s * INCHANNELS##.0 * unpack_row_length) / a)); \
    for(GLuint y = 0; y < height; y++) { \
        const INTYPE* inrow = &in[k * y];    \
        OUTTYPE* outrow = &out[outw * y * OUTCHANNELS];                                   \
        for(GLuint x = 0; x < unpack_row_length; x++) {                                                                                                                  \
            GLuint outbase = x * OUTCHANNELS;                         \
            GLuint inbase = x * INCHANNELS;\
            WRITER \
        } \
    } \
} \

#ifdef ARM_NEON
void
convert_GLubyte_4_to_GLubyte_4(GLuint unpack_row_length, GLuint height, const GLubyte *in,
                               GLubyte *out, GLuint outw, const unsigned char swizzle[4]) {
    GLint s = sizeof(GLubyte);
    GLint a = current_context->unpack.alignment;
    GLuint k;
    if (s >= a)k = unpack_row_length * 4;
    else
        k = (GLuint) (((double) a / s) * ceil((s * 4.0 * unpack_row_length) / a));

    uint64_t swizzle_raw = *(uint32_t*)swizzle;
    uint8x8_t index = vadd_u8(vcreate_u8((uint64_t) swizzle_raw | swizzle_raw << 32) , vcreate_u8(0x0404040400000000L));

    for (GLuint y = 0; y < height; y++) {
        const GLubyte *inrow = &in[k * y];
        GLubyte *outrow = &out[outw * y * 4];

        for (GLuint x = 0; x < unpack_row_length; x += 2) {
            GLuint outbase = x * 4;
            GLuint inbase = x * 4;
            {
                uint8x8_t rowbase = *(uint8x8_t*) &inrow[inbase];
                *(uint8x8_t*)&outrow[outbase] = vtbl1_u8(rowbase, index);
            }
        }
    }
}
#else
CONVERT(GLubyte, GLubyte, 4, 4, {
    outrow[outbase + 0] = inrow[inbase + swizzle[0]];
    outrow[outbase + 1] = inrow[inbase + swizzle[1]];
    outrow[outbase + 2] = inrow[inbase + swizzle[2]];
    outrow[outbase + 3] = inrow[inbase + swizzle[3]];
})
#endif

CONVERT(GLubyte, GLubyte, 3, 4, {
    outrow[outbase + 0] = inrow[inbase + 0];
    outrow[outbase + 1] = inrow[inbase + 1];
    outrow[outbase + 2] = inrow[inbase + 2];
    outrow[outbase + 3] = UINT8_MAX;
})

static inline void reverse_swizzle(unsigned char swizzle[4]) {
    unsigned char swizzle_temp[4] = {swizzle[0], swizzle[1], swizzle[2], swizzle[3]};
    swizzle[0] = swizzle_temp[3];
    swizzle[1] = swizzle_temp[2];
    swizzle[2] = swizzle_temp[1];
    swizzle[3] = swizzle_temp[0];
}

INTERNAL void convert_texture2d(GLenum type, GLenum format, GLuint width, GLuint height, GLvoid const* data, GLenum outtype, GLenum outformat, GLvoid** outdata) {
    if((!is_type_basic(type) && !is_rgba8_special(format, type)) || !is_type_basic(outtype)) {
        LOGI("conversion between non-basic types %x and %x", type, outtype);
        return;
    }
    unsigned int color_channels_in = num_color_channels(format);
    unsigned int color_channels_out = num_color_channels(outformat);

    unsigned char swizzle[4] = {0, 1, 2, 3};
    bool normalize = true;
    bool swizzle_bgra = false;
    bool swizzle_abgr = false;
    switch (format) {
        case GL_RED_INTEGER:
        case GL_RG_INTEGER:
        case GL_RGB_INTEGER:
        case GL_BGR_INTEGER:
        case GL_RGBA_INTEGER:
        case GL_BGRA_INTEGER:
            normalize = false;
            break;
    }
    switch(format) {
        case GL_BGR:
        case GL_BGR_INTEGER:
        case GL_BGRA_INTEGER:
        case GL_BGRA:
            swizzle[0] = 2;
            swizzle[1] = 1;
            swizzle[2] = 0;
            swizzle[3] = 3;
            break;
        case GL_ABGR_EXT:
            swizzle[0] = 3;
            swizzle[1] = 2;
            swizzle[2] = 1;
            swizzle[3] = 0;
            break;
        case GL_RGBA:
            swizzle[1] = 3;
            swizzle[3] = 1;
            break;
    }
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define UNSIGNED_DIRECT_FORMAT GL_UNSIGNED_INT_8_8_8_8_REV
#define UNSIGNED_REVERSE_FORMAT  GL_UNSIGNED_INT_8_8_8_8
#else
#define UNSIGNED_DIRECT_FORMAT GL_UNSIGNED_INT_8_8_8_8
#define UNSIGNED_REVERSE_FORMAT  GL_UNSIGNED_INT_8_8_8_8_REV
#endif
    switch(type) {
        case UNSIGNED_REVERSE_FORMAT:
            reverse_swizzle(swizzle);
        case UNSIGNED_DIRECT_FORMAT:
            type = GL_UNSIGNED_BYTE;
            break;
    }
#undef UNSIGNED_DIRECT_FORMAT
#undef UNSIGNED_REVERSE_FORMAT

    unpack_state_t *state = &current_context->unpack;

    printf("SWIZ %d %d %d %d\n", swizzle[0], swizzle[1], swizzle[2], swizzle[3]);

    *outdata = malloc(width * height * color_channels_in * num_channel_bits(outtype));
    GLuint unpack_row_length = current_context->unpack.row_length;
    if(unpack_row_length == 0) unpack_row_length = width;
    // Vintage Story BGRA uploads
    // TODO: Xaero's Minimap handling
    // TODO: literally everything else, too...
    if(type == GL_UNSIGNED_BYTE && outtype == GL_UNSIGNED_BYTE) {
        if(color_channels_in == 4 && color_channels_out == 4)
            convert_GLubyte_4_to_GLubyte_4(unpack_row_length, height, data, *outdata, width, swizzle);
        else if(color_channels_in == 3 && color_channels_out == 4)
            convert_GLubyte_3_to_GLubyte_4(unpack_row_length, height, data, *outdata, width, swizzle);
        else goto undefined;
    }else goto undefined;
    return;

    undefined:
    free(*outdata);
    *outdata = NULL;
    LOGI("undefined conversion between types %x[%i] and %x[%i]", type, color_channels_in, outtype, color_channels_out);
}


