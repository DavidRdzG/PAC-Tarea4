#pragma once
#include <string>
namespace AntiVM {
    bool detect_bios_vendor(std::string &out_info) noexcept;
}