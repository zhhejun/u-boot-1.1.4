
#include <common.h>
#include <linux-adapter.h>
#include <asm/arch/gpio.h>

#ifdef CONFIG_HW_WATCHDOG

#ifdef CONFIG_HISILICON_WDT_LED 
	#define hi_led_init() HISILICON_GPIO_SET_BITDIR(1, \
			HISILICON_WDT_LED_GPIO_GROUP, \
			HISILICON_WDT_LED_GPIO_BITS)
	#define hi_led_on() HISILICON_GPIO_WRBIT(HISILICON_WDT_LED_ON_VAL, \
			HISILICON_WDT_LED_GPIO_GROUP, \
			HISILICON_WDT_LED_GPIO_BITS)
	#define hi_led_off() HISILICON_GPIO_WRBIT(!HISILICON_WDT_LED_ON_VAL, \
			HISILICON_WDT_LED_GPIO_GROUP, \
			HISILICON_WDT_LED_GPIO_BITS)
#else
	#define hi_led_init() 
	#define hi_led_on() 
	#define hi_led_off() 
#endif

#if CONFIG_HISILICON_WDT_EXTERN > 0

    #define hi_wdt_ext_init()\
	        writel(readl(IO_ADDRESS(HISILICON_SCTL_BASE) + REG_SC_PERCTRL1) & (~(1 << 19)), (IO_ADDRESS(HISILICON_SCTL_BASE) + REG_SC_PERCTRL1));\
            HISILICON_GPIO_SET_BITDIR(1, HISILICON_WDT_EXT_GPIO_GROUP, HISILTCON_WDT_EXT_GPIO_BITS)
    #define hi_wdt_ext_set()\
            HISILICON_GPIO_WRBIT(1, HISILICON_WDT_EXT_GPIO_GROUP, HISILTCON_WDT_EXT_GPIO_BITS)
    #define hi_wdt_ext_clear()\
            HISILICON_GPIO_WRBIT(0, HISILICON_WDT_EXT_GPIO_GROUP, HISILTCON_WDT_EXT_GPIO_BITS)
    #define hi_wdt_ext_toggle()\
            if ((HISILICON_GPIO_RDBIT(HISILICON_WDT_EXT_GPIO_GROUP, HISILTCON_WDT_EXT_GPIO_BITS) >> HISILTCON_WDT_EXT_GPIO_BITS) & 0x01) {\
                hi_wdt_ext_clear();\
            } else {\
                hi_wdt_ext_set();\
            }
#else
    #define hi_wdt_ext_init()
    #define hi_wdt_ext_toggle()
#endif

#ifdef CONFIG_HISILICON_WDT
#define hiwdt_readl(x)          readl(IO_ADDRESS(HIWDT_REG(x)))
#define hiwdt_writel(v,x)       writel(v, IO_ADDRESS(HIWDT_REG(x)))

static struct __hidog_clk {
	unsigned long rate;
}hidog_clk = { CFG_DEFAULT_CLK_BUS/2 };
#endif

static int hidog_status = 0;

#ifdef CONFIG_HISILICON_WDT
static void hidog_set_timeout(unsigned int nr)
{
	unsigned long cnt = (~0x0)/hidog_clk.rate; 
//	unsigned long flags; 

	spin_lock_irqsave(&hidog_lock, flags); 

	if( nr==0 || nr>cnt)
		cnt=~0x0; 
	else
		cnt = nr*hidog_clk.rate;
	/* unlock watchdog registers */ 
	hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK); 
	hiwdt_writel(cnt, HIWDT_LOAD); 
	hiwdt_writel(cnt, HIWDT_VALUE); 
	/* lock watchdog registers */ 
	hiwdt_writel(0, HIWDT_LOCK); 
	spin_unlock_irqrestore(&hidog_lock, flags); 
};
#endif

static void hidog_start(void)
{
#ifdef CONFIG_HISILICON_WDT
//	unsigned long flags;
	unsigned long t;

	spin_lock_irqsave(&hidog_lock, flags);
	/* unlock watchdog registers */
	hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);
	hiwdt_writel(0x00, HIWDT_CTRL);
	hiwdt_writel(0x00, HIWDT_INTCLR);
	hiwdt_writel(0x03, HIWDT_CTRL);
	/* lock watchdog registers */
	hiwdt_writel(0, HIWDT_LOCK);
	/* enable watchdog clock */
	t = readl(IO_ADDRESS(HISILICON_SCTL_BASE));
	writel(t | 0x00800000, IO_ADDRESS(HISILICON_SCTL_BASE));
	spin_unlock_irqrestore(&hidog_lock, flags);

//	options = WDIOS_ENABLECARD;
#endif
	hi_led_on();
	hidog_status = 1;
}

static void hidog_stop(void)
{
#ifdef CONFIG_HISILICON_WDT
//	unsigned long flags;
	unsigned long t;

	spin_lock_irqsave(&hidog_lock, flags);
	/* disable watchdog clock */
	t = readl(IO_ADDRESS(HISILICON_SCTL_BASE));
	writel(t & ~0x00800000, IO_ADDRESS(HISILICON_SCTL_BASE));

	/* unlock watchdog registers */
	hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK);

	/* stop watchdog timer */
	hiwdt_writel(0x00, HIWDT_CTRL);
	hiwdt_writel(0x00, HIWDT_INTCLR);

	/* lock watchdog registers */
	hiwdt_writel(0, HIWDT_LOCK);

	spin_unlock_irqrestore(&hidog_lock, flags);

	hidog_set_timeout(0);

//	options = WDIOS_DISABLECARD;
#endif

	hi_led_off();
	hidog_status = 0;
}

#ifdef CONFIG_HISILICON_WDT
static void hidog_feed(void)
{
//	unsigned long flags; 

	spin_lock_irqsave(&hidog_lock, flags); 
	/* unlock watchdog registers */ 
	hiwdt_writel(HIWDT_UNLOCK_VAL, HIWDT_LOCK); 
	/* clear watchdog */ 
	hiwdt_writel(0x00, HIWDT_INTCLR); 
	/* lock watchdog registers */ 
	hiwdt_writel(0, HIWDT_LOCK); 
	spin_unlock_irqrestore(&hidog_lock, flags); 
};
#else
#define hidog_feed()
#endif

void hw_watchdog_reset(void)
{
#ifdef CONFIG_HISILICON_WDT_LED 
	static int led_status = 0;
	static volatile int in_hw_watchdog_reset = 0;
	static unsigned long last_timer_masked = 0;
#endif
	hi_wdt_ext_toggle();
	if(hidog_status == 0)
		return;

	hidog_feed();

#ifdef CONFIG_HISILICON_WDT_LED 
	if(0 == in_hw_watchdog_reset++) {
		if(get_timer_masked() >= last_timer_masked) {
			last_timer_masked = get_timer_masked() + HISILICON_WDT_LED_PERIOD;
			if(led_status++ & 0x1) {
				hi_led_on();
			} else
				hi_led_off();
		}
		if(get_timer_masked() < (last_timer_masked - HISILICON_WDT_LED_PERIOD))
			last_timer_masked = get_timer_masked() + HISILICON_WDT_LED_PERIOD;
	}

	in_hw_watchdog_reset--;
#endif
}

void reconfig_hidog(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	const char *s = getenv("watchdog");
	int timeout;
#ifdef CONFIG_HISILICON_WDT
	unsigned long cnt;
	hidog_clk.rate = gd->bus_clk/2;
	cnt = (~0x0)/hidog_clk.rate; 
#endif

	hi_wdt_ext_init();
	hi_led_init();
	hi_led_on();

	if(s) {
		if(strcmp(s,"off") == 0) {
			hidog_stop();
			printf("Watchdog disabled\n");
			return;
		}
		timeout = simple_strtoul(s, NULL, 0);
	} else {
#if !defined(CONFIG_HISILICON_WDT) || (CONFIG_HISILICON_WDT == 0)
		hidog_stop();
		printf("Watchdog disabled by default!\n");
		return;
#else
		timeout = CONFIG_WDT_TIMEOUT;
#endif
	}

#ifdef CONFIG_HISILICON_WDT
	if(timeout<10 || timeout > cnt) {
		printf("Watchdog timeout(t=%d) not in range 10<=watchdog<=%d, set as default %d\n", timeout, cnt, CONFIG_WDT_TIMEOUT);
		timeout = CONFIG_WDT_TIMEOUT;
	}
	hidog_set_timeout(timeout);
#endif

	hidog_start();
	hidog_feed();
}

#endif

