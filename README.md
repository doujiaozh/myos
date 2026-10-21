# My Super OS (32-bit Bare Metal)

[![CI Status](https://github.com/doujiaozh/myos/actions/workflows/main.yml/badge.svg)](https://github.com/doujiaozh/myos/actions)

A minimalist 32-bit operating system built from scratch in pure NASM Assembly, featuring a RAM file system, hardware interrupts (PIT/Keyboard), and VGA graphics.

## Features
- **Bootloader**: Custom 16-bit boot sector loading kernel into memory.
- **Protected Mode**: GDT setup, A20 line enabled, 32-bit execution.
- **Interrupts**: IDT configured with IRQ0 (Timer) and IRQ1 (Keyboard).
- **RAM File System**: Implemented `ls`, `touch`, `write`, `cat`, `clear`, `reboot` commands.
- **VGA Graphics**: Switches to Mode 13h (320x200, 256 colors) to render pixel blocks.

## How to Run
1. Compile: `nasm -f bin boot.asm -o boot.bin` and `nasm -f bin kernel.asm -o kernel.bin`
2. Build Image: `node -e "..."` (see docs)
3. Run: `qemu-system-i386 -drive format=raw,file=os.img`

## Screenshots
*(Add your QEMU terminal screenshots here!)*

---
Built with ❤️ by a 13-year-old developer.
