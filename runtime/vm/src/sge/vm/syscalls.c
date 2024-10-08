/*
 *
 */
#include "syscalls.h"
#include "state.h"

static int sge_vm_do_trap(lua_State* T) {
    sge_vm_task_t* task = sge_vm_task_from_state(T);
    sge_list_node_unlink(&task->node);
    sge_list_append(&sge_vm.task_list_trapped, &task->node);
}

static void sge_vm_sleep_done(uv_timer_t* p) {
    sge_vm_task_t* task = (sge_vm_task_t*)(p->data);
    sge_list_append(&sge_vm.task_list_ready, &task->node);
}

static int sge_vm_sys_sleep(lua_State* T) {
    sge_vm_task_t* task = sge_vm_task_from_state(T);
    lua_Integer ms = luaL_checkinteger(T, 1);
    if (ms > 0) {
        uv_timer_start(&task->sleep_timer, &sge_vm_sleep_done, ms, 0);
        return lua_yield(T, 0);
    }
    return 0;
}

static int sge_vm_sys_run(lua_State* T) {
    int type;

    switch (lua_gettop(T)) {
    case 2:
    case 1:
        type = lua_type(T, 1);
        break;
    default:
        return luaL_error(T, "too many arguments.");
    }

    switch (type) {
    case LUA_TSTRING:
        break;
    case LUA_TFUNCTION:
        break;
    default:
        return luaL_argerror(T, 1, "not a runnable.");
    }

    lua_State* N = lua_newthread(T);
    sge_vm_task_t* new_task = sge_vm_task_from_state(N);
    // new_task->context = XXX
    // TODO add new_task to sge_vm.enabled_objects by its context.
    lua_xmove(T, N, 1);

    return 1;
}

void sge_vm_init_syscalls(lua_State* L) {
    /* basic */

    lua_pushcfunction(L, &sge_vm_sys_sleep);
    lua_setglobal(L, "sleep");

    lua_pushcfunction(L, &sge_vm_sys_run);
    lua_setglobal(L, "run");

    /* context */

    int n = lua_gettop(L);
    lua_getmetatable(L, LUA_RIDX_GLOBALS);
    n = lua_gettop(L);

    luaL_newmetatable(L, "sge.context");
}
