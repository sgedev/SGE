/*
 *
 */
#include "state.h"

void sge_vm_open(lua_State* L) {
}

void sge_vm_close(lua_State* L) {
}

void sge_vm_new_task(lua_State* L, lua_State* T) {
    sge_vm_task_t* main = sge_vm_task_from_state(L);
    sge_vm_task_t* task = sge_vm_task_from_state(T);
    task->loop = main->loop;
    uv_timer_init(task->loop, &task->sleep_timer);
    task->sleep_timer.data = task;
    sge_list_reset(&task->wait_list);
    sge_list_node_reset(&task->node);
    sge_list_append(&sge_vm.task_list_ready, &task->node);
}

void sge_vm_remove_task(lua_State* L, lua_State* T) {
    sge_vm_task_t* task = sge_vm_task_from_state(T);
    sge_list_node_unlink(&task->node);
    uv_timer_stop(&task->sleep_timer);
    uv_close((uv_handle_t*)&task->sleep_timer, NULL);
    // TODO wait_list
}

void sge_vm_resume_task(lua_State* T, int n) {
}

void sge_vm_yield_task(lua_State* T, int n) {
}