/**
 * Notorious Netfilter
 * CS 241 - Spring 2019
 */
#include <linux/ip.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/proc_fs.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/udp.h>

#include "filter.h"

// Good luck!
MODULE_LICENSE("GPL");

int init_module() {
    printk("hello world\n");
    return 0;
}

void cleanup_module() {

}
