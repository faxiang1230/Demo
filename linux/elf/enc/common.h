#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "md5.h"

#define MAX_DATA    (1024 * 1024)
#define STUB_FILE	"mystub"
typedef struct {
    int payload_len;
    int key_len;
    char digest[64];
    char key[64];
    char inter[64];
    char data[MAX_DATA];
}payload_t;
#endif /*__COMMON_H*/