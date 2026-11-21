#!/bin/bash
# ZenithOS 构建脚本

echo "=== ZenithOS v6.0 构建系统 ==="
echo "日期: $(date)"

# 检查依赖
check_dependencies() {
    echo "检查构建依赖..."
    
    # 检查 nasm
    if ! command -v nasm &> /dev/null; then
        echo "错误: nasm 未安装"
        echo "安装: sudo apt install nasm"
        exit 1
    fi
    
    # 检查 gcc
    if ! command -v gcc &> /dev/null; then
        echo "错误: gcc 未安装"
        echo "安装: sudo apt install gcc"
        exit 1
    fi
    
    echo "✓ 依赖检查通过"
}

# 编译引导加载器
build_bootloader() {
    echo "编译引导加载器..."
    nasm -f bin boot.asm -o boot.bin
    echo "✓ boot.bin 生成完成"
}

# 编译内核 C 代码
build_kernel() {
    echo "编译内核 C 代码..."
    
    # 编译所有 C 源文件
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c main.c -o main.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c microkernel_core.c -o microkernel_core.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c microkernel_scheduler.c -o microkernel_scheduler.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c capability_system.c -o capability_system.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c ipc_manager.c -o ipc_manager.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c memory_manager.c -o memory_manager.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c cow_filesystem.c -o cow_filesystem.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c ai_scheduler.c -o ai_scheduler.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c secure_boot.c -o secure_boot.o
    gcc -m64 -ffreestanding -fno-pie -no-pie -Wall -Wextra -O2 -c isolation_unit.c -o isolation_unit.o
    
    echo "✓ 所有 C 文件编译完成"
}

# 链接内核
link_kernel() {
    echo "链接内核..."
    ld -m elf_x86_64 \
        main.o microkernel_core.o microkernel_scheduler.o \
        capability_system.o ipc_manager.o memory_manager.o \
        cow_filesystem.o ai_scheduler.o secure_boot.o \
        isolation_unit.o \
        -o kernel.elf
    
    echo "✓ kernel.elf 生成完成"
}

# 创建启动镜像
create_image() {
    echo "创建启动镜像..."
    dd if=/dev/zero of=zenithos.img bs=1M count=100 2>/dev/null
    
    # 写入引导加载器
    dd if=boot.bin of=zenithos.img conv=notrunc 2>/dev/null
    
    # 注意：实际项目中需要更复杂的链接和镜像生成
    echo "✓ zenithos.img 生成完成 (100MB)"
    ls -lh zenithos.img
}

# 运行测试
run_tests() {
    echo "运行安全测试..."
    if [ -f "test_hack.py" ]; then
        python3 test_hack.py
    else
        echo "⚠ test_hack.py 不存在，跳过测试"
    fi
}

# 清理
clean() {
    echo "清理构建产物..."
    rm -f *.o kernel.elf boot.bin zenithos.img
    echo "✓ 清理完成"
}

# 主流程
main() {
    case "$1" in
        all)
            check_dependencies
            build_bootloader
            build_kernel
            link_kernel
            create_image
            run_tests
            echo "=== 构建完成 ==="
            echo "运行: qemu-system-x86_64 -drive format=raw,file=zenithos.img"
            ;;
        boot)
            check_dependencies
            build_bootloader
            ;;
        kernel)
            check_dependencies
            build_kernel
            link_kernel
            ;;
        clean)
            clean
            ;;
        test)
            run_tests
            ;;
        *)
            echo "用法: $0 {all|boot|kernel|clean|test}"
            exit 1
            ;;
    esac
}

main "$@"
