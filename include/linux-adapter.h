
#ifndef __U_BOOT_NET_OSHAL_H
#define __U_BOOT_NET_OSHAL_H

#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <linux/string.h>
#include <asm/errno.h>
#include <asm/sizes.h>
#include <asm/io.h>

#define __U_BOOT_OSHAL_NET__

#define KHZ (1000)
#define MHZ (KHZ*1000)

#define IO_ADDRESS(x) (x)

#define ETH_ALEN 6

struct net_device_stats
{
	unsigned int reserved;
};

struct net_device {
	unsigned char dev_addr[ETH_ALEN];

	void *priv_data;
};

inline static struct net_device *alloc_etherdev(int size)
{
	struct net_device *p;

	size += sizeof(struct net_device);

	p = (struct net_device *)malloc(size);
	memset(p, 0, size);

	p->priv_data = (void*)((char*)p + sizeof(struct net_device));

	return p;
}
#define free_netdev(p) free(p)
#define netdev_priv(dev) (dev)->priv_data
#define register_netdev(dev) (dev)==NULL
#define netif_carrier_off(dev)
#define netif_carrier_on(dev)
#define netif_start_queue(dev)
#define netif_stop_queue(dev)

/* spin lock */
typedef unsigned int spinlock_t;
#define spin_lock_irqsave(a,b) do{ }while(0)
#define spin_unlock_irqrestore(a,b) do{ }while(0)
#define spin_lock_init(p) do{ }while(0)
#define local_irq_save(f) do{ (f)=0; }while(0)
#define local_irq_restore(f) 

/* format */
#define printk printf
#define KERN_INFO ""
static inline int sscanf(const char *s, const char *f, unsigned long *r)
{
	const char *endp = s;

	*(r) = simple_strtoul(s,(char**)&endp,0);

	return endp - s;
}

#endif /* __U_BOOT_NET_OSHAL_H */

