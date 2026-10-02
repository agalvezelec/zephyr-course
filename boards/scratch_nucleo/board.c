#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

static int board_scratch_nucleo_init(void) {
    printk("Board Initialized\n");
    return 0;
}

SYS_INIT(board_scratch_nucleo_init, PRE_KERNEL_1, 99);
