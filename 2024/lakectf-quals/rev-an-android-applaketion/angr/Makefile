NDK_TOOLCHAIN ?= /opt/android-ndk/toolchains/llvm/prebuilt/linux-x86_64
WAYDROID_SYSROOT ?= /mnt
CC = $(NDK_TOOLCHAIN)/bin/clang
TARGET = x86_64-linux-android
API = 21
CFLAGS = -Wall -O2
LIBS = -L. -lohgreat

ohgreat: ohgreat.c
	$(CC) $(CFLAGS) --target=$(TARGET)$(API) -o $@ $< $(LIBS)

run: ohgreat
	export LD_LIBRARY_PATH=$(WAYDROID_SYSROOT)/system/apex/com.android.runtime/lib64/bionic:$(WAYDROID_SYSROOT)/system/lib64:$(WAYDROID_SYSROOT)/system/apex/com.android.art.release/lib64:$(WAYDROID_SYSROOT)/system/apex/com.android.os.statsd/lib64:.; \
	$(WAYDROID_SYSROOT)/system/apex/com.android.runtime/bin/linker64 $(shell readlink -f ./$<)

clean:
	rm -f ohgreat

.PHONY: run clean
