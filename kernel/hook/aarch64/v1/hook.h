#ifndef __TPE_HEADER__
#define __TPE_HEADER__
#include <linux/types.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define OP_HOOK_SIZE 4
#define IN_ERR(x) (x < 0)
#define HOOK_ENABLE		1
#define HOOK_DISABLE	0

struct kernsym {
        void *addr; // orig addr
        void *end_addr;
        unsigned long size;
        char *name;
        bool name_alloc; // whether or not we alloc'd memory for char *name
        u8 orig_ins[OP_HOOK_SIZE];
        void *new_addr;
        unsigned long new_size;
        bool found;
        bool hijacked;
        void *run;
		atomic_t call_cnt;
};
typedef struct hook_s{ 
    char *name;
    struct kernsym *sym;
    unsigned long *func;
    int enable;
}hook_t;

int hook_register_symbol(struct kernsym *, const char *, unsigned long *);
void hook_restore_symbol(struct kernsym *);
int find_symbol_address(struct kernsym *sym, const char *symbol_name);
int hook_env_init(void);

extern struct file *fget(unsigned int fd);
extern int __close_fd(struct files_struct *files, unsigned fd);
extern void fput(struct file *);
extern int kallsyms_on_each_symbol(int (*fn)(void *, const char *, struct module *, unsigned long),
					    void *data);
#endif
