#ifndef CPU_X86_MSR_H
#define CPU_X86_MSR_H

#if defined(__ROMCC__)

typedef __builtin_msr_t msr_t;

static msr_t rdmsr(unsigned long index)
{
	return __builtin_rdmsr(index);
}

static void wrmsr(unsigned long index, msr_t msr)
{
	__builtin_wrmsr(index, msr.lo, msr.hi);
}

#else

typedef struct msr_struct
{
	unsigned lo;
	unsigned hi;
} msr_t;

typedef struct msrinit_struct
{
	unsigned index;
	msr_t msr;
} msrinit_t;

#if IS_ENABLED(CONFIG_SOC_SETS_MSRS)
msr_t soc_msr_read(unsigned index);
void soc_msr_write(unsigned index, msr_t msr);

/* Handle MSR references in the other source code */
static inline __attribute__((always_inline)) msr_t rdmsr(unsigned index)
{
	return soc_msr_read(index);
}

static inline __attribute__((always_inline)) void wrmsr(unsigned index, msr_t msr)
{
	soc_msr_write(index, msr);
}
#else /* CONFIG_SOC_SETS_MSRS */

/* The following functions require the always_inline due to AMD
 * function STOP_CAR_AND_CPU that disables cache as
 * RAM, the cache as RAM stack can no longer be used. Called
 * functions must be inlined to avoid stack usage. Also, the
 * compiler must keep local variables register based and not
 * allocated them from the stack. With gcc 4.5.0, some functions
 * declared as inline are not being inlined. This patch forces
 * these functions to always be inlined by adding the qualifier
 * __attribute__((always_inline)) to their declaration.
 */
static inline __attribute__((always_inline)) msr_t rdmsr(unsigned index)
{
	msr_t result;
	__asm__ __volatile__ (
		"rdmsr"
		: "=a" (result.lo), "=d" (result.hi)
		: "c" (index)
		);
	return result;
}

static inline __attribute__((always_inline)) void wrmsr(unsigned index, msr_t msr)
{
	__asm__ __volatile__ (
		"wrmsr"
		: /* No outputs */
		: "c" (index), "a" (msr.lo), "d" (msr.hi)
		);
}

#endif /* CONFIG_SOC_SETS_MSRS */
#endif /* __ROMCC__ */

#endif /* CPU_X86_MSR_H */
