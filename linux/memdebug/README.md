Three methods of application memory stat:
1. use glibc malloc_chunk

>reflink:https://sourceware.org/glibc/wiki/MallocInternals

2. encapsulation of malloc,like kernel kmem_cache or apache memory alloc

>reflink:https://ci.apache.org/projects/httpd/trunk/doxygen/group__apr__allocator.html
>reflink:kernel memory cache

3.transparent malloc with same api,but manage its info implicitly

>reference:kernel lockdep

4. glibc mtrace

use mtrace/muntrace function, `man 3 mtrace`

5. malloc hook

replace mtrace hook, `man 3 mtrace_hook`

6. strong symbol replace glibc weak symbol

>reflink:https://stackoverflow.com/questions/17803456/an-alternative-for-the-deprecated-malloc-hook-functionality-of-glibc
