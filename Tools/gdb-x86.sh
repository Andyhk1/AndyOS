#!/bin/bash
cd "$(dirname "$0")"

GNOME_TERMINAL_SCREEN=""
mkfifo /tmp/guest
gnome-terminal -- qemu-system-i386 -m 1G -cpu qemu32,+ssse3,+sse4.1,+sse4.2, -cdrom ../AndyOS.iso -drive file="$HOME/VirtualBox VMs/AndyOS/disk.img" -s -S -serial pipe:/tmp/guest -device AC97

sleep 0.1

args=
for arg in "$@";
do
  args="$args '$arg'"
done

eval exec gdb $args
