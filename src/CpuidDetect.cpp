#include "CpuidDetect.h"
#include <cstring>
#ifdef _WIN32
  #include <intrin.h>
#else
  #include <cpuid.h>
#endif
namespace AntiVM {
bool detect_cpuid_hypervisor(std::string &out_info) noexcept {
    out_info.clear();
    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;
#ifdef _WIN32
    int regs[4] = {0};
    __cpuid(regs, 1);
    eax = (unsigned)regs[0];
    ebx = (unsigned)regs[1];
    ecx = (unsigned)regs[2];
    edx = (unsigned)regs[3];
#else
    if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
        out_info = "cpuid not supported";
        return false;
    }
#endif
    bool hypervisor = (ecx & (1u << 31)) != 0;
    if (hypervisor) out_info = "Hypervisor bit (CPUID.1:ECX[31]) = 1";
    else out_info = "Hypervisor bit (CPUID.1:ECX[31]) = 0";
    return hypervisor;
}}