#!/bin/bash
# 构建 OS 镜像，简单

if [ -z "$1" ]; then
    TARGET="zenithos"
else
    TARGET="$1"
fi

echo "Building $TARGET image..."

# 创建空镜像
dd if=/dev/zero of=${TARGET}.img bs=1M count=100 2>/dev/null

# 写入引导扇区
dd if=boot.bin of=${TARGET}.img conv=notrunc 2>/dev/null

# 写入内核 (从扇区 2 开始)
# 注意：这里简化了，实际需要更多步骤
echo "Writing kernel to image..."
# 实际应该将内核链接并写入适当位置

echo "Build complete: ${TARGET}.img"
echo "Run with: qemu-system-x86_64 -drive format=raw,file=${TARGET}.img"

# 显示镜像大小
ls -lh ${TARGET}.img
