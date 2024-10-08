/*
 *
 */
#ifndef SGE_VM_HOOKS_H
#define SGE_VM_HOOKS_H

#include <sge/common.h>
#include <sge/list.h>

#undef LUA_EXTRASPACE
#undef luai_userstateopen
#undef luai_userstateclose
#undef luai_userstatethread
#undef luai_userstatefree
#undef luai_userstateresume
#undef luai_userstateyield

SGE_BEGIN_DECLS

typedef struct sge_vm_kernel sge_vm_kernel_t;
typedef struct sge_vm_context sge_vm_context_t;

typedef struct {
    uv_loop_t* loop;
    uv_timer_t sleep_timer;
    sge_list_t wait_list;
    sge_list_node_t node;
    sge_vm_kernel_t* kernel;
    sge_vm_context_t* context;
    int last_error;
} sge_vm_task_t;

void sge_vm_open(lua_State* L);
void sge_vm_close(lua_State* L);
void sge_vm_newtask(lua_State* L, lua_State* T);
void sge_vm_removetask(lua_State* L, lua_State* T);
void sge_vm_resumetask(lua_State* T, int n);
void sge_vm_yieldtask(lua_State* T, int n);

SGE_END_DECLS

#define LUA_EXTRASPACE sizeof(sge_vm_task_t)
#define luai_userstateopen(L) sge_vm_open(L)
#define luai_userstateclose(L) sge_vm_close(L)
#define luai_userstatethread(L, T) sge_vm_newtask(L, T)
#define luai_userstatefree(L, T) sge_vm_removetask(L, T)
#define luai_userstateresume(T, n) sge_vm_resumetask(T, n)
#define luai_userstateyield(T, n) sge_vm_yieldtask(T, n)

#endif /* SGE_VM_HOOKS_H */
