# My Super OS (32-bit)

A bare-metal 32-bit operating system built from scratch.

- Boot sector written in pure NASM.
- Loads kernel into memory.
- Enters 32-bit protected mode.
- Sets up 4MB memory paging.
- Configures a 256-entry IDT for hardware interrupts.

## How to build and run

1. Compile boot sector: `nasm -f bin boot.asm -o boot.bin`
2. Compile kernel: `nasm -f bin kernel.asm -o kernel.bin`
3. Assemble image: `node -e "..."`
4. Run: `qemu-system-i386 -drive format=raw,file=os.img`
