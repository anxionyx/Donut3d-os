# Donut3d-os
Install cross-toolchain: i686-elf-gcc / ld or adapt to your toolchain. (As a quick alternative, you can compile with native gcc -m32 if available.)
Run make to produce donutos.iso.
Boot donutos.iso in QEMU: qemu-system-i386 -cdrom donutos.iso or in VirtualBox.
