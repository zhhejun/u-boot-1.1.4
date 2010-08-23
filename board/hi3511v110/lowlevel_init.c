#include <config.h>
#include <common.h>
#include <linux-adapter.h>
#include <asm/arch/platform.h>


void cpu_init_sysctrl(void);
void cpu_init_smi(void);
void cpu_init_ddrc(void);

void lowlevel_init(void)
{
	cpu_init_sysctrl();
	cpu_init_smi();
	cpu_init_ddrc();
}

