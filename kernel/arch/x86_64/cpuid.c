#include <kernel/arch/x86_64/cpuid.h>
#include <common/print.h>

#define BASIC_LEAF 0x00000000
#define FEATURE_LEAF 0x00000001
#define EXTENDED_LEAF 0x80000000
#define LONG 0x80000001

#define BIT_IS_SET(reg, bit) (((reg) & (1u << (bit))) != 0)

static inline kbool_t cpuid_supported(void) {
  uint32_t eflags_before = 0;
  uint32_t eflags_after = 0;

  __asm__ volatile(
      "pushfl\n\t"
      "popl %0\n\t"
      "movl %0, %1\n\t"
      "xorl $0x00200000, %1\n\t"
      "pushl %1\n\t"
      "popfl\n\t"
      "pushfl\n\t"
      "popl %1\n\t"
      "pushl %0\n\t"
      "popfl\n\t"
      : "=&r"(eflags_before), "=&r"(eflags_after)
      :
      : "cc");

  return ((eflags_before ^ eflags_after) & 0x00200000) != 0;
}

static inline void cpuid_get(uint32_t leaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
  uint32_t a = 0;
  uint32_t b = 0;
  uint32_t c = 0;
  uint32_t d = 0;

  __asm__ volatile("cpuid"
                   : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                   : "a"(leaf), "c"(0));

  if (eax != NULL) {
    *eax = a;
  }
  if (ebx != NULL) {
    *ebx = b;
  }
  if (ecx != NULL) {
    *ecx = c;
  }
  if (edx != NULL) {
    *edx = d;
  }
}

static inline uint32_t cpuid_get_max(uint32_t base_leaf) {
  uint32_t max_leaf = 0;

  cpuid_get(base_leaf, &max_leaf, NULL, NULL, NULL);
  return max_leaf;
}

cpuid_info cpuid_probe(void) {
    cpuid_info info = {
        false,
        0,
        0,
        false,
        false,
        false,
    };
    if (!cpuid_supported()) {
        print(AS_CPUID, "CPUID NOT SUPPORTED. RETURNING TO CALLER", l_red);
        return info;
    } else {
        print(AS_CPUID, "CPUID SUPPORTED", l_green);
        info.cpuid_exists = true;
    }
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;
    kbool_t has_msr = false, has_pae = false, has_long = false;
    uint32_t ext_mleaf = cpuid_get_max(EXTENDED_LEAF);
    info.max_eleaf = ext_mleaf;
    uint32_t basic_mleaf = cpuid_get_max(BASIC_LEAF);
    info.max_bleaf = basic_mleaf; 
    if (basic_mleaf < FEATURE_LEAF) {
        warn("BASIC CPUID LEAF SMALLER THAN 1");
    } else {
        cpuid_get(FEATURE_LEAF, &eax, &ebx, &ecx, &edx);
    }
    has_msr = BIT_IS_SET(edx, 5);
    info.has_msr = has_msr;
    has_pae = BIT_IS_SET(edx, 6);
    info.has_pae = has_pae;
    if (ext_mleaf < LONG) {
        warn("CAN'T QUERY LONG-MODE CAPABILITY");
    } else {
    cpuid_get(LONG, &eax, &ebx, &ecx, &edx);
    has_long = BIT_IS_SET(edx, 29);
    info.has_long_mode = has_long;
    }
    if (!has_msr) {warn("NO MODEL-SPECIFIC REGISTER - MSR");}
    else {print(AS_CPUID, "MODEL-SPECIFIC REGISTER EXISTS - MSR AVAILABLE", l_green);}
    if (!has_pae) {warn("NO PHYSICAL ADDRESS EXTENSION - PAE");}
    else {print(AS_CPUID, "PHYSICAL ADDRESS EXTENSION EXISTS - PAE AVAILABLE", l_green);}
    if (!has_long) {warn("NO LONG MODE");}
    else {print(AS_CPUID, "LONG MODE AVAILABLE", l_green);}
    print(AS_CPUID, "END OF CPUID CHECK. RETURNING TO CALLER", l_green);
    return info;
}   
