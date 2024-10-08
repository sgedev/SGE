/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include <uv.h>
#include <HandmadeMath.h>

#include <sge/config.h>

#ifdef SGE_DEBUG
#   include <assert.h>
#   define SGE_ASSERT(expr) assert(expr)
#else
#   define SGE_ASSERT(expr)
#endif

#ifdef __cplusplus
#   define SGE_BEGIN_DECLS extern "C" {
#   define SGE_END_DECLS }
#else
#   define SGE_BEGIN_DECLS
#   define SGE_END_DECLS
#endif

#if defined(__GNUC__)
#   define SGE_FORCE_INLINE __inline __attribute__ ((__always_inline__))
#   define SGE_LIKELY(expr) __builtin_expect((expr), 1)
#   define SGE_UNLIKELY(expr) __builtin_expect((expr), 0)
#elif defined(_MSC_VER)
#   define SGE_FORCE_INLINE __forceinline
#   define SGE_LIKELY(expr) (expr)
#   define SGE_UNLIKELY(expr) (expr)
#else
#   error unknown compiler.
#endif

#define SGE_CON(a, b) SGE_CON_(a, b)
#define SGE_CON_(a, b) a##b

#define SGE_CON3(a, b, c) SGE_CON3_(a, b, c)
#define SGE_CON3_(a, b, c) a##b##c

#define SGE_STRIZE(x) SGE_STRIZE_(x)
#define SGE_STRIZE_(x) #x

#define SGE_PMOVB(p, offset) \
    ((intptr_t)(((uint8_t*)p) + offset))

#define SGE_OFFSETOF(struct_type, member_name) \
    ((intptr_t)(&(((struct_type*)0)->member_name)))

#define SGE_MEMBEROF(p, struct_type, member_name) \
    ((struct_type*)SGE_PMOVB(p, -SGE_OFFSETOF(struct_type, member_name)))

#define SGE_VERSION_TEXT \
    SGE_STRIZE(SGE_VERSION_MAJOR) "." \
    SGE_STRIZE(SGE_VERSION_MINOR) "." \
    SGE_STRIZE(SGE_VERSION_PATCH)

SGE_BEGIN_DECLS

typedef struct {
    void* (*alloc)(size_t size);
    void* (*realloc)(void* p, size_t new_size);
    void (*free)(void* p);
} sge_allocator_t;

extern const sge_allocator_t sge_default_allocator;

SGE_END_DECLS

#endif /* SGE_COMMON_H */
