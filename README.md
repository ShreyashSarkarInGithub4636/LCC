THis is lcc.
Lightweight C Compiler.
It is an alternitive to GNU Compiler COllection (gcc)

To compile it, run:
musl-gcc --static -o lcc lcc.c
Very easy to compile.
it requires you to have musl.

How to install musl:

For fedora:
sudo dnf install musl

For Arch Linux:
sudo pacman -S musl
or
yay -S musl

For Ubuntu:
sudo apt install musl

For Debian:
sudo dpkg install musl

For busybox:
