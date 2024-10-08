/*
 *
 */
#ifndef SGE_VM_KERNEL_H
#define SGE_VM_KERNEL_H

#include <sge/vm/common.h>

SGE_BEGIN_DECLS

typedef struct {
	lua_CFunction initrc_loader;
	void* user_data;
} sge_vm_kernel_config_t;

struct sge_vm_kernel {
	lua_State* L;
	uv_loop_t loop;
	void* user_data;
	const luaL_Reg* syscalls;
};

SGE_FORCE_INLINE lua_State* sge_vm_kernel_state(sge_vm_kernel_t* kernel) {
	SGE_ASSERT(kernel != NULL);
	return kernel->L;
}

SGE_FORCE_INLINE uv_loop_t* sge_vm_kernel_loop(sge_vm_kernel_t* kernel) {
	SGE_ASSERT(kernel != NULL);
	return &(kernel->loop);
}

SGE_FORCE_INLINE void* sge_vm_kernel_user_data(sge_vm_kernel_t* kernel) {
	SGE_ASSERT(kernel != NULL);
	return kernel->user_data;
}

bool sge_vm_kernel_init(sge_vm_kernel_t* kernel, const sge_vm_kernel_config_t* config);
void sge_vm_kernel_close(sge_vm_kernel_t* kernel);
bool sge_vm_kernel_run(sge_vm_kernel_t* kernel, uv_run_mode mode);
void sge_vm_kernel_stop(sge_vm_kernel_t* kernel);

SGE_END_DECLS

#endif /* SGE_VM_H */

