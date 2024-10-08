/*
 *
 */
#include <sge/log.h>

#include "state.h"

sge_vm_t sge_vm;

static void sge_vm_handle_traps() {
    sge_list_node_t* node;
    sge_vm_task_t* task;
    int ret;

    while (!sge_list_is_empty(&sge_vm.task_list_trapped)) {
        node = sge_list_remove_first(&sge_vm.task_list_trapped);
        task = SGE_MEMBEROF(node, sge_vm_task_t, node);

    }
}

bool sge_vm_init(uv_loop_t* loop, sge_vm_kernel_t* kernel) {
    SGE_ASSERT(loop != NULL);
    SGE_ASSERT(kernel != NULL);

    SGE_LOGI("Initializing VM kernel...\n");

    int ret = uv_mutex_init(&sge_vm.mutex);
    if (ret < 0) {
        SGE_LOGE("Failed to initialize VM kernel mutex.\n");
        goto bad0;
    }

    ret = uv_cond_init(&sge_vm.cond);
    if (ret < 0) {
        SGE_LOGE("Failed to initialize VM kernel cond.\n");
        goto bad1;
    }

    ret = uv_async_init(loop, &sge_vm.async_trap, &sge_vm_handle_traps);
    if (ret < 0) {
        SGE_LOGE("Failed to initialize VM kernel async trap.\n");
        goto bad2;
    }

    sge_vm.loop = loop;
    sge_vm.state = SGE_VM_STATE_STOPPED;

    SGE_LOGI("VM initialized.\n");

    return true;

bad2:
    uv_cond_destroy(&sge_vm.cond);

bad1:
    uv_mutex_destroy(&sge_vm.mutex);

bad0:
    return false;
}

void sge_vm_shutdown() {
    SGE_ASSERT(sge_vm.state == SGE_VM_STATE_STOPPED);

    SGE_LOGI("vm shuting down..\n");
    uv_cond_destroy(&sge_vm.cond);
    uv_mutex_destroy(&sge_vm.mutex);

    sge_vm.state = SGE_VM_STATE_IDLE;
}

bool sge_vm_start() {
    SGE_LOGI("vm starting...\n");

    uv_mutex_lock(&sge_vm.mutex);

    if (sge_vm.state != SGE_VM_STATE_STOPPED) {
        SGE_LOGE("invalid vm state for starting.\n");
        goto bad0;
    }

    sge_vm.state = SGE_VM_STATE_STARTING;

    extern void sge_vm_main(void *p);
    int ret = uv_thread_create(&sge_vm.thread, &sge_vm_main, NULL);
    if (ret < 0) {
        sge_vm.state = SGE_VM_STATE_STOPPED;
        SGE_LOGE("failed to create vm thread.\n");
        goto bad0;
    }

    ret = uv_cond_timedwait(&sge_vm.cond, &sge_vm.mutex, 5000);
    if (ret < 0 || sge_vm.state != SGE_VM_STATE_RUNNING) {
        SGE_LOGE("failed to start vm.");
        goto bad1;
    }

    uv_mutex_unlock(&sge_vm.mutex);

    SGE_LOGI("vm started.\n");

    return true;

bad1:
    sge_vm.state = SGE_VM_STATE_STOPPED;
    uv_mutex_unlock(&sge_vm.mutex);
    uv_async_send(&sge_vm.async_stop);
    uv_thread_join(&sge_vm.thread);

bad0:
    return false;
}

void sge_vm_stop() {
    uv_mutex_lock(&sge_vm.mutex);
    if (sge_vm.state == SGE_VM_STATE_RUNNING) {
        SGE_LOGI("stopping vm...\n");
        uv_async_send(&sge_vm.async_trap);
        uv_thread_join(&sge_vm.thread);
        uv_close((uv_handle_t*)&sge_vm.async_trap, NULL);
        sge_vm.state = SGE_VM_STATE_STOPPED;
        SGE_LOGI("vm stopped.\n");
    }
    uv_mutex_unlock(&sge_vm.mutex);
}
