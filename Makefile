# ZenithOS Makefile
# 构建系统

CC = gcc
NASM = nasm
LD = ld
QEMU = qemu-system-x86_64

# 目标
TARGET = zenithos
KERNEL = kernel.elf
BOOT = boot.bin

# 源文件
KERNEL_SRC = main.c microkernel_core.c microkernel_scheduler.c \
             capability_system.c ipc_manager.c memory_manager.c \
             cow_filesystem.c

# 汇编文件
ASM_SRC = boot.asm
ASM_OBJ = boot.o

# 编译标志
CFLAGS = -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2
LDFLAGS = -m elf_x86_64

# 默认目标
all: $(TARGET)

$(TARGET): $(KERNEL)
	@echo "Building boot image..."
	./build_image.sh $(TARGET)
	@echo "Build complete! Run with: $(QEMU) -drive format=raw,file=$(TARGET).img"

$(KERNEL): $(KERNEL_SRC)
	@echo "Compiling kernel..."
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) $(CFLAGS) -c microkernel_core.c -o microkernel_core.o
	$(CC) $(CFLAGS) -c microkernel_scheduler.c -o microkernel_scheduler.o
	$(CC) $(CFLAGS) -c capability_system.c -o capability_system.o
	$(CC) $(CFLAGS) -c ipc_manager.c -o ipc_manager.o
	$(CC) $(CFLAGS) -c memory_manager.c -o memory_manager.o
	$(CC) $(CFLAGS) -c cow_filesystem.c -o cow_filesystem.o
	$(LD) $(LDFLAGS) main.o microkernel_core.o microkernel_scheduler.o \
	     capability_system.o ipc_manager.o memory_manager.o cow_filesystem.o \
	     -o $(KERNEL)

boot.bin: $(ASM_SRC)
	@echo "Assembling boot sector..."
	$(NASM) -f bin $(ASM_SRC) -o $(BOOT)

# 测试
test: $(TARGET)
	@echo "Running tests..."
	python3 test_hack.py
	@echo "Tests complete!"

# 运行
run: $(TARGET)
	$(QEMU) -drive format=raw,file=$(TARGET).img -m 2G -enable-kvm

# 调试
debug: $(TARGET)
	$(QEMU) -drive format=raw,file=$(TARGET).img -m 2G -s -S
	@echo "QEMU running. Attach with: gdb -exec $(KERNEL) 'target remote localhost:1234'"

# 清理
clean:
	rm -f *.o $(KERNEL) $(TARGET).img boot.bin
	@echo "Cleaned!"

# 帮助
help:
	@echo "ZenithOS Build System"
	@echo "====================="
	@echo "make all      - Build the OS"
	@echo "make test     - Run tests"
	@echo "make run      - Run in QEMU"
	@echo "make debug    - Run with GDB debugging"
	@echo "make clean    - Clean build artifacts"
	@echo "make help     - Show this help"

.PHONY: all test run debug clean help
