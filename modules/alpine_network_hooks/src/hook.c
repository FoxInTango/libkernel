#include "hook.h"
#include "../../core/echo.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
//unsigned int alpine_ipv4_in(unsigned int hooknum, struct sk_buff* skb, const struct net_device* in, const struct net_device* out, int(*okfn)(struct sk_buff*))

unsigned int alpine_ipv4_in(void*, struct sk_buff*, const struct nf_hook_state*)
{
    echo("alpine ipv4 hooking.\n");
    return NF_ACCEPT;
};

static struct nf_hook_ops alpine_network_hooks[] = {
    {
        .hook     = alpine_ipv4_in,
        .pf       = NFPROTO_IPV4,
        .hooknum  = NF_INET_PRE_ROUTING,
        .priority = NF_IP_PRI_CONNTRACK,
    },
    {
        .hook     = alpine_ipv4_in,
        .pf       = NFPROTO_IPV4,
        .hooknum  = NF_INET_PRE_ROUTING,
        .priority = NF_IP_PRI_CONNTRACK,
    },
    {
        .hook     = alpine_ipv4_in,
        .pf       = NFPROTO_IPV4,
        .hooknum  = NF_INET_PRE_ROUTING,
        .priority = NF_IP_PRI_CONNTRACK,
    },
    {
        .hook     = alpine_ipv4_in,
        .pf       = NFPROTO_IPV4,
        .hooknum  = NF_INET_PRE_ROUTING,
        .priority = NF_IP_PRI_CONNTRACK,
    },
    {
        .hook     = alpine_ipv4_in,
        .pf       = NFPROTO_IPV4,
        .hooknum  = NF_INET_PRE_ROUTING,
        .priority = NF_IP_PRI_CONNTRACK,
    },
    {
        .hook     = alpine_ipv4_in,
        .pf       = NFPROTO_IPV4,
        .hooknum  = NF_INET_PRE_ROUTING,
        .priority = NF_IP_PRI_CONNTRACK,
    },
}

int alpine_network_hook_startup(void){
    return ( 
        nf_register_net_hook(alpine_network_hooks[0]) &&
        nf_register_net_hook(alpine_network_hooks[1]) &&
        nf_register_net_hook(alpine_network_hooks[2]) &&
        nf_register_net_hook(alpine_network_hooks[3]) &&
        nf_register_net_hook(alpine_network_hooks[4]) &&
        nf_register_net_hook(alpine_network_hooks[5])
        ) ? 1:0;
}
void alpine_network_hook_shutdown(void){
    nf_unregister_net_hook(alpine_network_hooks[0]);
    nf_unregister_net_hook(alpine_network_hooks[1]);
    nf_unregister_net_hook(alpine_network_hooks[2]);
    nf_unregister_net_hook(alpine_network_hooks[3]);
    nf_unregister_net_hook(alpine_network_hooks[4]);
    nf_unregister_net_hook(alpine_network_hooks[5]);
}

// 钩子返回值 https://blog.csdn.net/weixin_41400449/article/details/106764232