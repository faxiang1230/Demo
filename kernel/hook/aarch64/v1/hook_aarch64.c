#include "hook.h"
#include <asm/fixmap.h>
extern void flush_icache_range(unsigned long start, unsigned long end);

// callback for find_symbol_address
static int find_symbol_callback(struct kernsym *sym, const char *name, struct module *mod,
		unsigned long addr) {

	if (sym->found) {
		sym->end_addr = (unsigned long *)addr;
		return 1;
	}

	// this symbol was found. the next callback will be the address of the next symbol
	if (name && sym->name && !strcmp(name, sym->name)) {
		sym->addr = (unsigned long *)addr;
		sym->found = true;
	}

	return 0;
}

// find this symbol
int find_symbol_address(struct kernsym *sym, const char *symbol_name) {

	int ret;

	sym->name = (char *)symbol_name;
	sym->found = 0;

	ret = kallsyms_on_each_symbol((void *)find_symbol_callback, sym);

	if (!ret)
		return -EFAULT;

	sym->size = sym->end_addr - sym->addr;
	sym->run = sym->addr;

	return 0;
}

static struct kernsym sym_module_alloc;
static struct kernsym sym_module_free;
static struct kernsym sym_aarch64_insn_gen_branch_imm;
static struct kernsym sym_aarch64_insn_gen_cond_branch_imm;
static struct kernsym sym_aarch64_insn_patch_text_sync;
static struct kernsym sym___set_fixmap;
static struct kernsym sym___close_fd;

int hook_env_init(void)
{
	int ret = 0;

	ret = find_symbol_address(&sym_module_alloc, "module_alloc");

	if (IN_ERR(ret)){
		printk(KERN_ERR "find module_alloc failed:%d\n", ret);
		return ret;
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,0,0)
	ret = find_symbol_address(&sym_module_free, "module_memfree");
#else
	ret = find_symbol_address(&sym_module_free, "module_free");
#endif

	if (IN_ERR(ret)){
		printk(KERN_ERR "find sys_module_free failed:%d\n", ret);
		return ret;
	}

	ret = find_symbol_address(&sym___set_fixmap, "__set_fixmap");

	if (IN_ERR(ret)){
		printk(KERN_ERR "find __set_fixmap:%d\n", ret);
		return ret;
	}

	ret = find_symbol_address(&sym___close_fd, "__close_fd");

	if (IN_ERR(ret)){
		printk(KERN_ERR "find __set_fixmap:%d\n", ret);
		return ret;
	}
	ret = find_symbol_address(&sym_aarch64_insn_gen_cond_branch_imm, "aarch64_insn_gen_cond_branch_imm");
	if (IN_ERR(ret)) {
        printk("find comp_branch_imm:%d\n" ,ret);
        return ret;
	}
	ret = find_symbol_address(&sym_aarch64_insn_gen_branch_imm, "aarch64_insn_gen_branch_imm");
	if (IN_ERR(ret)) {
        printk("find branch_imm:%d\n" ,ret);
        return ret;
	}
	ret = find_symbol_address(&sym_aarch64_insn_patch_text_sync, "aarch64_insn_patch_text_sync");
	if (IN_ERR(ret)) {
        printk("find patch_sync:%d\n" ,ret);
        return ret;
	}
	return ret;
}
int __close_fd(struct files_struct *files, unsigned fd)
{
	int (*run)(struct files_struct *, unsigned) = sym___close_fd.run;
	return run(files, fd);
}
void __set_fixmap(enum fixed_addresses idx, phys_addr_t phys, pgprot_t flags)
{
	void (*run)(enum fixed_addresses idx, phys_addr_t phys, pgprot_t flags) = sym___set_fixmap.run;
	run(idx, phys, flags);
}
void *malloc(unsigned long size) {
	void *(*run)(unsigned long) = sym_module_alloc.run;
	return run(size);
}

// call to module_free

void malloc_free(void *buf) {
	void (*run)(struct module *, void *) = sym_module_free.run;
	if (buf != NULL)
		run(NULL, buf);
}
u32 arch_gen_cond_branch_imm(unsigned long pc, unsigned long addr, enum aarch64_insn_condition cond)
{
	u32 (*run)(unsigned long, unsigned long, enum aarch64_insn_condition) =
		sym_aarch64_insn_gen_cond_branch_imm.run;
	if (run)
		return run(pc, addr, cond);
	else
		return 0;
}
u32 arch_gen_branch_imm(unsigned long pc, unsigned long addr, enum aarch64_insn_branch_type type)
{
	u32 (*run)(unsigned long, unsigned long, enum aarch64_insn_branch_type) =
		sym_aarch64_insn_gen_branch_imm.run;
	if (run)
		return run(pc, addr, type);
	else
		return 0;
}
int hsm_patch_text_sync(void *addrs[], u32 insns[], int cnt)
{
	u32 (*run)(void *[], u32 [], int) =
		sym_aarch64_insn_patch_text_sync.run;
	if (run)
		return run(addrs, insns, cnt);
	else
		return 0;
}
static int arch_patch_text(u32 *addr, u32 opcode)
{
	void *addrs[1];
	u32 insns[1];

	addrs[0] = (void *)addr;
	insns[0] = opcode;
	return hsm_patch_text_sync(addrs, insns, 1);
}
char mb_ins[4] = {0x9f, 0x3f, 0x03, 0xd5};
char nop_ins[4] = {0x1f, 0x20, 0x03, 0xd5};
int nop_insn = 0xd503201f;
int ret_insn = 0xd65f03c0;
/*
 * d10143ff    sub sp, sp, #0x50
 * a90007e0    stp x0, x1, [sp]
 * a9010fe2    stp x2, x3, [sp,#16]
 * a90217e4    stp x4, x5, [sp,#32]
 * a9031fe6    stp x6, x7, [sp,#48]
 * a9047bfd    stp x29, x30, [sp,#64]
 */
char save_stack_ins[] = {
	0xff, 0x43, 0x01, 0xd1,
	0xe0, 0x07, 0x00, 0xa9,
	0xe2, 0x0f, 0x01, 0xa9,
	0xe4, 0x17, 0x02, 0xa9,
	0xe6, 0x1f, 0x03, 0xa9,
	0xfd, 0x7b, 0x04, 0xa9,
};
/*
 * a94007e0	ldp	x0, x1, [sp]
 * a9410fe2	ldp	x2, x3, [sp,#16]
 * a94217e4	ldp	x4, x5, [sp,#32]
 * a9431fe6	ldp	x6, x7, [sp,#48]
 * a9447bfd	ldp	x29, x30, [sp,#64]
 * 910143ff	add	sp, sp, #0x50
 * */
char restore_stack_ins[] = {
	0xe0, 0x07, 0x40, 0xa9,
	0xe2, 0x0f, 0x41, 0xa9,
	0xe4, 0x17, 0x42, 0xa9,
	0xe6, 0x1f, 0x43, 0xa9,
	0xfd, 0x7b, 0x44, 0xa9,
	0xff, 0x43, 0x01, 0x91,
};
/*
 * a9447bfd	ldp	x29, x30, [sp,#64]
 * 910143ff	add	sp, sp, #0x50
 * d65f03c0	ret
 */
char fail_ins[] = {
	0xfd, 0x7b, 0x44, 0xa9,
	0xff, 0x43, 0x01, 0x91,
	0xc0, 0x03, 0x5f, 0xd6,
};
/*
 * f100001f		cmp	x0, #0x0
 * */
char comp_ins[] = {
	0x1f, 0x00, 0x00, 0xf1
};
char mov_x0_0[] = {
	//d2800000	mov	x0, #0x0
	0x00, 0x00, 0x80, 0xd2,
};
char push_arg_ins[][4] = {
	//aa0903e0	mov	x0, x9
	{0xe0, 0x03, 0x09, 0xaa},
	//aa0903e1	mov	x1, x9
	{0xe1, 0x03, 0x09, 0xaa},
	//aa0903e2	mov	x2, x9
	{0xe2, 0x03, 0x09, 0xaa},
	//aa0903e3	mov	x3, x9
	{0xe3, 0x03, 0x09, 0xaa},
	//aa0903e4	mov	x4, x9
	{0xe4, 0x03, 0x09, 0xaa},
	//aa0903e5	mov	x5, x
	{0xe5, 0x03, 0x09, 0xaa},
};
char save_ret_ins[] = {
	//aa0003e9	mov	x9, x0
	0xe9, 0x03, 0x00, 0xaa,
};
char mod_lr_ins[] = {
	//1000001e	adr	x30, .
	//910033de	add	x30, x30, #0xc
	0x1e, 0x00, 0x00, 0x10,
	0xde, 0x43, 0x00, 0x91,
};
int hook_register_symbol(struct kernsym *sym, const char *symbol_name, unsigned long *code) {
	int ret;
	u32 tmp_insn;

	ret = find_symbol_address(sym, symbol_name);

	if (IN_ERR(ret)) {
		printk(KERN_ERR "failed to find %s symbol\n", symbol_name);
		return ret;
	}

	sym->new_addr = malloc(sizeof(u32) * 4);
	memcpy(sym->orig_ins, sym->addr, sizeof(u32));

	memcpy(sym->new_addr, sym->addr, sizeof(u32));
	tmp_insn = arch_gen_branch_imm((unsigned long)(((u32 *)sym->new_addr) + 1), 
			(unsigned long)(((u32 *)sym->addr) + 1), AARCH64_INSN_BRANCH_NOLINK);
	*(((u32 *)sym->new_addr) + 1) = cpu_to_le32(tmp_insn);
	*(((u32 *)sym->new_addr) + 2) = cpu_to_le32(nop_insn);

	flush_icache_range((unsigned long)sym->new_addr, (unsigned long)sym->new_addr + 4);

	tmp_insn = arch_gen_branch_imm((unsigned long)sym->addr, (unsigned long)code, AARCH64_INSN_BRANCH_NOLINK);
	arch_patch_text(sym->addr, tmp_insn);

	sym->hijacked  = 1;

	return 0;
}
void hook_restore_symbol(struct kernsym *sym) {
	u32 opcode = *(((u32 *)sym->orig_ins));
	if (!sym->hijacked)
		return;
	arch_patch_text(sym->addr, opcode);
	malloc_free(sym->new_addr);
	sym->hijacked = 0;
	return;
}
