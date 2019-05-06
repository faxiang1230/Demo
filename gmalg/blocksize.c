/*************************************************************************
    > File Name: blocksize.c
    > Author: wangjx
    > Mail: wangjianxing5210@163.com 
    > Created Time: 2019年05月05日 星期日 18时06分55秒
 ************************************************************************/

#include <stdio.h>
#include <linux/fs.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>
size_t bfx_bdev_size(char *path)
{
    int fd = 0;
    size_t len = 0;

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("%s open failed\n", path);
        return -1;
    }

    if (ioctl(3, BLKGETSIZE64, &len) == 0) {
        close(fd);
        return len;
    } else {
        close(fd);
        return -1;
    }
}
void main()
{
    printf("%lu\n", bfx_bdev_size("/dev/sda"));
}
