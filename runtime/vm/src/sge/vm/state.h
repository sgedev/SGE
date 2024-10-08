/*
 *
 */
#ifndef SGE_VM_STATE_H
#define SGE_VM_STATE_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <GLFW/glfw3.h>

#include <sge/vm.h>

#include "context.h"

SGE_BEGIN_DECLS

typedef enum {
    SGE_VM_STATE_IDLE = 0,
    SGE_VM_STATE_STOPPED,
    SGE_VM_STATE_STARTING,
    SGE_VM_STATE_RUNNING,
    SGE_VM_STATE_TRAPPED,
    SGE_VM_STATE_STOPPING
} sge_vm_state_t;

typedef struct {
    uv_loop_t* loop;
    uv_mutex_t mutex;
    uv_cond_t cond;
    uv_thread_t thread;
    sge_vm_state_t state;
    GLFWwindow* window;
    bool graphics_enabled;
    bool physics_enabled;
    bool network_enabled;
    const sge_vm_traps_t* traps;
    uv_async_t async_trap;
    uv_async_t async_stop;
    sge_list_t task_list_ready;
    sge_list_t task_list_trapped;

    struct {
        sge_vm_task_t* world;
        sge_list_t directional_light_list;
        sge_list_t point_light_list;
        sge_list_t spot_light_list;
        sge_list_t entity_list;
    } enabled_objects;
} sge_vm_t;

extern sge_vm_t sge_vm;

SGE_FORCE_INLINE lua_State* sge_vm_task_to_state(sge_vm_task_t* task) {
    return (lua_State*)SGE_PMOVB(task, sizeof(sge_vm_task_t));
}

SGE_FORCE_INLINE sge_vm_task_t* sge_vm_task_from_state(lua_State* L) {
    return (sge_vm_task_t*)SGE_PMOVB(L, sizeof(sge_vm_task_t));
}

SGE_END_DECLS

#endif /* SGE_VM_STATE_H */
