#ifndef _CPU_AMD_CAR_H
#define _CPU_AMD_CAR_H

#include <arch/cpu.h>

void asmlinkage mainboard_romstage_entry(unsigned long bist);

void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx);
void done_cache_as_ram_main(void);
void post_cache_as_ram(void);

void cache_as_ram_switch_stack(void *stacktop);
void cache_as_ram_new_stack(void);

#endif
