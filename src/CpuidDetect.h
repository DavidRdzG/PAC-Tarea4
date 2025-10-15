#pragma once
#include <string>
namespace AntiVM {
    bool detect_cpuid_hypervisor(std::string &out_info) noexcept;
}