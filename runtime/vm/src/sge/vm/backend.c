/*
 *
 */
#include <sge/fs.h>
#include <sge/physics.h>

#include "state.h"
#include "syscalls.h"

static void sge_vm_schedule(uv_prepare_t* p) {
    sge_list_node_t* node;
    sge_vm_task_t* task;
    int ret;

    while (!sge_list_is_empty(&sge_vm.task_list_ready)) {
        node = sge_list_remove_first(&sge_vm.task_list_ready);
        task = SGE_MEMBEROF(node, sge_vm_task_t, node);
        ret = lua_resume(sge_vm_task_to_state(task), (lua_State*)(p->data), 0, NULL);
        if (ret != LUA_OK && ret != LUA_YIELD) {
            // TODO
        }
    }

    if (!sge_list_is_empty(&sge_vm.task_list_trapped)) {
        uv_async_send(&sge_vm.async_trap);
    }
}

static void sge_vm_build_view() {
    sge_list_node_t* node;
    sge_vm_task_t* task;
    int ret;

    // sge_graphics_begin();

    while (!sge_list_is_empty(&sge_vm.enabled_objects.directional_light_list)) {
        node = sge_list_remove_first(&sge_vm.task_list_ready);
        task = SGE_MEMBEROF(node, sge_vm_task_t, node);
    }

    // sge_graphics_end();
}

static void sge_vm_update(uv_timer_t* p) {
    if (sge_vm.graphics_enabled) {
        sge_vm_build_view();
    }

    if (sge_vm.physics_enabled) {
        sge_physics_update(20);
    }

    if (sge_vm.network_enabled) {
        //sge_network_update(20);
    }

    uv_update_time(p->loop);
}

static void sge_vm_load_initrc(lua_State* L) {
    //sge_fs_stat("/init.lua")
}

static void sge_vm_quit(uv_async_t* p) {
    uv_stop(p->loop);
}

static int sge_vm_pmain(lua_State* L) {
    if (sge_vm.state != SGE_VM_STATE_STARTING) {
        return luaL_error(L, "state");
    }

    uv_loop_t loop;
    int ret = uv_loop_init(&loop);
    if (ret < 0) {
        sge_vm.state = SGE_VM_STATE_STOPPED;
        uv_cond_signal(&sge_vm.cond);
        return luaL_error(L, "loop");
    }

    sge_vm_task_t* main = sge_vm_task_from_state(L);
    main->loop = &loop;
    main->context = NULL;

    luaL_openlibs(L);
    sge_vm_init_syscalls(L);
    sge_vm_load_initrc(L);

    uv_async_init(&loop, &sge_vm.async_stop, &sge_vm_quit);

    uv_prepare_t scheduler;
    uv_prepare_init(&loop, &scheduler);
    scheduler.data = L;
    uv_prepare_start(&scheduler, &sge_vm_schedule);

    uv_timer_t updater;
    uv_timer_init(&loop, &updater);
    uv_timer_start(&updater, &sge_vm_update, 0, 20);

    sge_vm.state = SGE_VM_STATE_RUNNING;
    uv_cond_signal(&sge_vm.cond);
    uv_run(&loop, UV_RUN_DEFAULT);

    uv_timer_stop(&updater);
    uv_close((uv_handle_t*)&updater, NULL);

    uv_prepare_stop(&scheduler);
    uv_close((uv_handle_t*)&scheduler, NULL);

    uv_close((uv_handle_t*)&sge_vm.async_stop, NULL);

    uv_loop_close(&loop);

    return 0;
}

void sge_vm_main(void *p) {
    lua_State* L = luaL_newstate();
    if (L == NULL) {
        return;
    }

    lua_pushcfunction(L, &sge_vm_pmain);

    uv_mutex_lock(&sge_vm.mutex);
    int ret = lua_pcall(L, 1, 0, 0);
    uv_mutex_unlock(&sge_vm.mutex);

    if (ret != LUA_OK) {
    }

    lua_close(L);
}
