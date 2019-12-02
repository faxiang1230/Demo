/* include/urcu/config.h.  Generated from config.h.in by configure.  */
/* urcu/config.h.in. Manually generated for control over the contained defs. */

/* Defined when on a system that has memory fence instructions. */
/* #undef CONFIG_RCU_HAVE_FENCE */

/* Defined when on a system with futex support. */
#define CONFIG_RCU_HAVE_FUTEX 1

/* Enable SMP support. With SMP support enabled, uniprocessors are also
   supported. With SMP support disabled, UP systems work fine, but the
   behavior of SMP systems is undefined. */
#define CONFIG_RCU_SMP 1

/* Compatibility mode for i386 which lacks cmpxchg instruction. */
/* #undef CONFIG_RCU_COMPAT_ARCH */

/* Use the dmb instruction is available for use on ARM. */
/* #undef CONFIG_RCU_ARM_HAVE_DMB */

/* TLS provided by the compiler. */
#define CONFIG_RCU_TLS __thread

/* clock_gettime() is detected. */
#define CONFIG_RCU_HAVE_CLOCK_GETTIME 1

/* Require the operating system to support the membarrier system call for
   default and bulletproof flavors. */
/* #undef CONFIG_RCU_FORCE_SYS_MEMBARRIER */

/* Enable internal debugging self-checks.
   Introduce performance penalty. */
/* #undef CONFIG_RCU_DEBUG */
