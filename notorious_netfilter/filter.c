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

static struct nf_hook_ops netfilter_ops;

static unsigned int main_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);
    unsigned int src_ip = (unsigned int)ip_header->saddr;
}

int init_module() {
    printk("hello world\n");

    netfilter_ops.hook = main_hook;
    netfilter_ops.pf = PF_INET;
    netfilter_ops.hooknum = 0;
    netfilter_ops.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net, &netfilter_ops);

    return 0;
}

void cleanup_module() {
    nf_unregister_net_hook(&init_net, &netfilter_ops);
}
