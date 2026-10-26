//
// Created by maks on 04.02.2026.
//

#include <byteswap.h>
#include <math.h>

#define COLOR_OF_TYPE_RGBA(TYPE, TYPE_SHORT) \
union color_##TYPE_SHORT { \
    __attribute__((packed)) struct { TYPE r, g, b, a; }; \
    TYPE channels[4]; \
};                                           \
typedef union color_##TYPE_SHORT color_##TYPE_SHORT##_t;\

#define COLOR_OF_TYPE_RGB(TYPE, TYPE_SHORT) \
union color_##TYPE_SHORT##_rgb { \
    __attribute__((packed)) struct { TYPE r, g, b; }; \
    TYPE channels[3]; \
};                                          \
typedef union color_##TYPE_SHORT##_rgb color_##TYPE_SHORT##_rgb_t; \

#define COLOR_OF_TYPE(...) COLOR_OF_TYPE_RGB(__VA_ARGS__) COLOR_OF_TYPE_RGBA(__VA_ARGS__)

COLOR_OF_TYPE(GLubyte, byte)
COLOR_OF_TYPE(GLuint, uint)
COLOR_OF_TYPE(GLfloat, float)

typedef union color_byte_rgb color_byte_332_t;
typedef union color_byte_rgb color_byte_565_t;

typedef union color_byte color_byte_4444_t;
typedef union color_byte color_byte_5551_t;
typedef union color_byte color_byte_8888_t;

typedef union color_uint color_uint_2101010_t;

#define REVERSE_RGBA(POSTFIX, COLOR_TYPE) \
static inline COLOR_TYPE rev_##POSTFIX(COLOR_TYPE in) { \
    COLOR_TYPE ret = { \
            .r = in.a, \
            .g = in.b, \
            .b = in.g, \
            .a = in.r \
    }; \
    return ret; \
}

static inline color_byte_332_t unpack_ubyte_3_3_2(GLubyte pix) {
    color_byte_332_t ret = {
            .r = ((pix >> 5) & 0b111),
            .g = ((pix >> 2) & 0b111),
            .b = (pix & 0b11)
    };
    return ret;
}

static inline color_byte_332_t unpack_ubyte_2_3_3_rev(GLubyte pix) {
    color_byte_332_t ret = {
            .b = ((pix >> 6) & 0b11),
            .g = ((pix >> 3) & 0b111),
            .r = (pix & 0b111)
    };
    return ret;
}

static inline color_byte_565_t unpack_ushort_5_6_5(GLushort pix) {
#ifdef __LITTLE_ENDIAN__
    pix = bswap_16(pix);
#endif
    color_byte_565_t ret = {
            .r = ((pix >> 11) & 0b11111),
            .g = ((pix >> 5) & 0b111111),
            .b = (pix & 0b11111)
    };
    return ret;
}

static inline color_byte_565_t rev_ushort_5_6_5(color_byte_565_t byte565) {
    color_byte_565_t ret = {
            ret.r = byte565.b,
            ret.g = byte565.g,
            ret.b = byte565.r
    };
    return ret;
}

static inline color_byte_4444_t unpack_ushort_4_4_4_4(GLushort pix) {
#ifdef __LITTLE_ENDIAN__
    pix = bswap_16(pix);
#endif
    color_byte_4444_t ret = {
            .r = ((pix >> 12) & 0b1111),
            .g = ((pix >> 8) & 0b1111),
            .b = ((pix >> 4) & 0b1111),
            .a = (pix & 0b1111),
    };
    return ret;
}

REVERSE_RGBA(ushort_4_4_4_4, color_byte_4444_t)

static inline color_byte_5551_t unpack_ushort_5_5_5_1(GLushort pix) {
#ifdef __LITTLE_ENDIAN__
    pix = bswap_16(pix);
#endif
    color_byte_5551_t ret = {
            .r = ((pix >> 11) & 0b11111),
            .g = ((pix >> 6) & 0b11111),
            .b = ((pix >> 1) & 0b11111),
            .a = (pix & 0b1),
    };
    return ret;
}

static inline color_byte_5551_t unpack_ushort_1_5_5_5_rev(GLushort pix) {
#ifdef __LITTLE_ENDIAN__
    pix = bswap_16(pix);
#endif
    color_byte_5551_t ret = {
            .a = ((pix >> 15) & 0b1),
            .b = ((pix >> 10) & 0b11111),
            .g = ((pix >> 5) & 0b11111),
            .r = (pix & 0b11111),
    };
    return ret;
}

static inline color_byte_8888_t unpack_uint_8_8_8_8(GLuint pix) {
#ifdef __LITTLE_ENDIAN__
    pix = bswap_32(pix);
#endif
    color_byte_8888_t ret = {
            .r = (pix >> 24) & 0xFF,
            .g = (pix >> 16) & 0xFF,
            .b = (pix >> 8) & 0xFF,
            .a = pix & 0xFF
    };
    return ret;
}

REVERSE_RGBA(uint_8_8_8_8, color_byte_8888_t)

static inline color_uint_2101010_t unpack_uint_10_10_10_2(GLuint pix) {
#ifdef __LITTLE_ENDIAN__
    pix = bswap_32(pix);
#endif
    color_uint_2101010_t ret = {
            .r = ((pix >> 22) & 0x3FF),
            .g = ((pix >> 12) & 0x3FF),
            .b = ((pix >> 2) & 0x3FF),
            .a = (pix & 0b11),
    };
    return ret;
}

static inline color_uint_2101010_t unpack_uint_2_10_10_10_rev(GLuint pix) {
#ifdef __LITTLE_ENDIAN__
    pix = bswap_32(pix);
#endif
    color_uint_2101010_t ret = {
            .a = ((pix >> 30) & 0b11),
            .b = ((pix >> 20) & 0x3FF),
            .g = ((pix >> 10) & 0x3FF),
            .r = (pix & 0x3FF)
    };
    return ret;
}

#define EXPAND_10B_32B (4198404)
#define EXPAND_2B_32B (1431655765)

static inline color_uint_t normalize_uint_10_10_10_2(color_uint_2101010_t input) {
    input.r *= EXPAND_10B_32B;
    input.g *= EXPAND_10B_32B;
    input.b *= EXPAND_10B_32B;
    input.a *= EXPAND_2B_32B;
    return input;
}

#undef EXPAND_2B_32B
#undef EXPAND_10B_32B

static inline GLubyte bnormalize1(GLubyte in) {
    return in * 255;
}

static inline GLubyte bnormalize2(GLubyte in) {
    return (0xFFF00F00 >> (in << 3)) & 0xFF;
}

static inline GLubyte bnormalize3(GLubyte in) {
#if defined(__amd64__) | defined(__aarch64__)
    return (0xFFDBB7926E492500 >> (in << 3)) & 0xFF;
#else
    return (GLubyte)ceilf(((float)in / 7.0f) * 255.0f);
#endif
}

static inline GLubyte bnormalize4(GLubyte in) {
    return in | (in << 4);
}

static inline GLubyte bnormalize5(GLubyte in) {
    return (in << 3) | ((in >> 2) & 0x7);
}

static inline GLubyte bnormalize6(GLubyte in) {
    return (in << 3) | ((in >> 2) & 0x3);
}

static inline GLubyte bnormalize10(GLuint in) {
    return (in >> 2) & 0xFF;
}

#define RGB_BYTE_NORMALIZE(INTYPE, RBITS, GBITS, BBITS) \
static inline color_byte_rgb_t INTYPE##_to_color_byte_rgb_t(INTYPE input) { \
    input.r = bnormalize##RBITS(input.r); \
    input.g = bnormalize##GBITS(input.g); \
    input.b = bnormalize##BBITS(input.b); \
    return input; \
}                                                          \

#define RGBA_BYTE_NORMALIZE(INTYPE, RBITS, GBITS, BBITS, ABITS) \
static inline color_byte_t INTYPE##_to_color_byte_t(INTYPE input) { \
    color_byte_t ret = {                                        \
       .r = bnormalize##RBITS(input.r),  \
       .g = bnormalize##GBITS(input.g),  \
       .b = bnormalize##BBITS(input.b),  \
       .a = bnormalize##ABITS(input.a),  \
    }; \
    return ret; \
}                                                               \

RGB_BYTE_NORMALIZE(color_byte_332_t, 3, 3, 2)
RGB_BYTE_NORMALIZE(color_byte_565_t, 5, 6, 5)
RGBA_BYTE_NORMALIZE(color_byte_4444_t, 4, 4, 4, 4)
RGBA_BYTE_NORMALIZE(color_byte_5551_t, 5, 5, 5, 1)
RGBA_BYTE_NORMALIZE(color_uint_2101010_t, 10, 10, 10, 2)

static inline color_byte_t color_byte_8888_t_to_color_byte_t(color_byte_8888_t in) {
    return in;
}

#define SWIZZLE_RGB(TYPE) \
static inline TYPE swizzle_color_##TYPE(color_byte_t color, const unsigned char swizzle[4]) { \
   TYPE ret = { \
            .r = color.channels[swizzle[0]], \
            .g = color.channels[swizzle[1]], \
            .b = color.channels[swizzle[2]], \
    }; \
    return ret; \
}

#define SWIZZLE_RGBA(TYPE) \
static inline TYPE swizzle_color_##TYPE(color_byte_t color, const unsigned char swizzle[4]) { \
    TYPE ret = { \
            .r = color.channels[swizzle[0]], \
            .g = color.channels[swizzle[1]], \
            .b = color.channels[swizzle[2]], \
            .a = color.channels[swizzle[3]] \
    }; \
    return ret; \
}

#define SWIZZLE(TYPE_SHORT) SWIZZLE_RGBA(color_##TYPE_SHORT##_t) SWIZZLE_RGB(color_##TYPE_SHORT##_rgb_t)

SWIZZLE(byte)

#define COLOR_CONVERTER(POSTFIX, COLOR_TYPE, COLOR_CONVERSION, COLOR_INPUT_TYPE) \
static void convert_texture2d_##POSTFIX(char const** data, COLOR_TYPE* output, GLuint width, GLuint height, const unsigned char swizzle[4]) { \
    const unsigned input_size = sizeof(COLOR_INPUT_TYPE); \
    GLuint global_alignment = current_context->unpack.alignment; \
    GLuint alignment = global_alignment < input_size ? input_size : global_alignment; \
    GLuint skip = width * alignment; \
    for(GLuint y = 0; y < height; y++) { \
        COLOR_INPUT_TYPE const* row = (COLOR_INPUT_TYPE const*)&data[y * skip]; \
        for(GLuint x = 0; x < width; x++) {                                      \
            COLOR_INPUT_TYPE color = row[x]; \
            output[x + (y * width)] = COLOR_CONVERSION; \
        } \
    } \
} \


#define COLOR_CONVERTER_NORM(POSTFIX, COLOR_TYPE, COLOR_CONVERSION, COLOR_INPUT_TYPE, CHANNELS) \
  COLOR_CONVERTER(POSTFIX, COLOR_TYPE, COLOR_CONVERSION, COLOR_INPUT_TYPE) \
  COLOR_CONVERTER(POSTFIX##_normb, color_byte_##CHANNELS##t, COLOR_TYPE##_to_color_byte_##CHANNELS##t(COLOR_CONVERSION), COLOR_INPUT_TYPE) \

COLOR_CONVERTER_NORM(ubyte_3_3_2, color_byte_332_t, unpack_ubyte_3_3_2(color), GLubyte, rgb_)
COLOR_CONVERTER_NORM(ubyte_2_3_3_rev, color_byte_332_t, unpack_ubyte_2_3_3_rev(color), GLubyte, rgb_)
COLOR_CONVERTER_NORM(ushort_5_6_5, color_byte_565_t, unpack_ushort_5_6_5(color), GLushort, rgb_)
COLOR_CONVERTER_NORM(ushort_5_6_5_rev, color_byte_565_t, rev_ushort_5_6_5(unpack_ushort_5_6_5(color)), GLushort, rgb_)
COLOR_CONVERTER_NORM(ushort_4_4_4_4, color_byte_4444_t, unpack_ushort_4_4_4_4(color), GLushort,)
COLOR_CONVERTER_NORM(ushort_4_4_4_4_rev, color_byte_4444_t, rev_ushort_4_4_4_4(unpack_ushort_4_4_4_4(color)), GLushort,)
COLOR_CONVERTER_NORM(ushort_5_5_5_1, color_byte_5551_t, unpack_ushort_5_5_5_1(color), GLushort,)
COLOR_CONVERTER_NORM(ushort_1_5_5_5_rev, color_byte_5551_t, unpack_ushort_1_5_5_5_rev(color), GLushort,)
COLOR_CONVERTER_NORM(uint_8_8_8_8, color_byte_8888_t, unpack_uint_8_8_8_8(color),GLuint,)
COLOR_CONVERTER_NORM(uint_8_8_8_8_rev, color_byte_8888_t, rev_uint_8_8_8_8(unpack_uint_8_8_8_8(color)),GLuint,)
COLOR_CONVERTER_NORM(uint_10_10_10_2, color_uint_2101010_t, unpack_uint_10_10_10_2(color), GLuint,)
COLOR_CONVERTER_NORM(uint_2_10_10_10_rev, color_uint_2101010_t, unpack_uint_2_10_10_10_rev(color), GLuint,)