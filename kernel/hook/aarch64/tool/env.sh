sysctl -w kernel.printk="8 8 8 8"
sysctl -w kernel.panic_on_warn=1
sysctl -w kernel.panic_on_oops=1
sysctl -w kernel.panic=1
sysctl -w kernel.softlockup_panic=1
dmesg -E
