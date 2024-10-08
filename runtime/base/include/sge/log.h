/*
 *
 */
#ifndef SGE_LOG_H
#define SGE_LOG_H

#include <stdarg.h>

#include <sge/common.h>

SGE_BEGIN_DECLS

enum {
    SGE_LOG_NONE = 0,
    SGE_LOG_ERROR,
    SGE_LOG_WARN,
    SGE_LOG_INFO,
    SGE_LOG_DEBUG
};

bool sge_log_init();
void sge_logv(int level, const char* fmt, va_list args);
void sge_logf(int level, const char* fmt, ...);

SGE_END_DECLS

#define SGE_LOGE(fmt, ...) sge_logf(SGE_LOG_ERROR, fmt, ##__VA_ARGS__)
#define SGE_LOGW(fmt, ...) sge_logf(SGE_LOG_WARN, fmt, ##__VA_ARGS__)
#define SGE_LOGI(fmt, ...) sge_logf(SGE_LOG_INFO, fmt, ##__VA_ARGS__)

#ifdef SGE_DEBUG
#   define SGE_LOGD(fmt, ...) sge_logf(SGE_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
#   define SGE_LOGD(fmt, ...)
#endif

#endif /* SGE_LOG_H */
