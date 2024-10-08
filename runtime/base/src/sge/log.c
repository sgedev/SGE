/*
 *
 */
#include <stdio.h>
#include <inttypes.h>

#include <sge/log.h>

static uv_mutex_t sge_log_mutex;
static int sge_log_level;

bool sge_log_init() {
    int ret = uv_mutex_init(&sge_log_mutex);
    if (ret < 0) {
        return false;
    }

    const char* env = getenv("SGE_LOG_LEVEL");
    if (env != NULL) {
        int level = atoi(env);
        if (0 <= level && level <= SGE_LOG_DEBUG) {
            sge_log_level = level;
        } else {
            sge_log_level = SGE_LOG_INFO;
        }
    }

    return true;
}

void sge_logv(int level, const char* fmt, va_list args) {
    if (level <= 0) {
        return;
    }
    if (level <= sge_log_level) {
        SGE_ASSERT(0 < level && level <= SGE_LOG_DEBUG);
        uv_mutex_lock(&sge_log_mutex);
        if (level <= sge_log_level) {
            static const char ltid_table[] = { 'E', 'W', 'I', 'D' };
            char ltid = ltid_table[level - 1];
            uint64_t ns = uv_hrtime();
            printf("[%c] %06" PRId64 ".%06" PRId64 " ", ltid, ns / 1000000000, ns % 1000000000);
            vprintf(fmt, args);
        }
        uv_mutex_unlock(&sge_log_mutex);
    }
}

void sge_logf(int level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    sge_logv(level, fmt, args);
    va_end(args);
}
