#include <unistd.h>
#ifndef ENTRY
# define ENTRY(name)			\
	.type name,  @function; 	\
	.globl name;			\
	.p2align 4;			\
	name:					\
	cfi_startproc
#endif

#ifndef END
# define END(name)			\
	cfi_endproc;			\
	.size name, .-name
#endif
ENTRY(getxuid)
    movl    $__NR_getuid, %eax
    syscall
    cmpq    $-MAX_ERRNO, %rax
    jb      1f
    negl    %eax
    movl    %eax, %edi
    call    __set_errno_internal
1:
    ret
END(getxuid)
#define MAX 100000000
int main() {
  int ii;
  for (ii=0; ii<MAX; ii++) getxuid();
  return 0;
}
