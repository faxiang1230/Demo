DME=/home/linux/project/android/android-7.1.0_r1/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-
CFLAGS = -DABI_arm64-v8a=1 --sysroot=/home/linux/project/android/android-7.1.0_r1/prebuilts/ndk/current/platforms/android-21/arch-arm64
CC = $(DME)gcc
AR = $(DME)ar
STRIP = $(DME)strip
LDFLAGS = -pie -fPIE -lm -lz -llog

inotify:inotify.c
	$(CC) inotify.c -o inotify $(CFLAGS) $(LDFLAGS)
