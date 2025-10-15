#include "BiosDetect.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#ifdef _WIN32
  #include <windows.h>
#else
  #include <fstream>
#endif
namespace AntiVM {
static const std::vector<std::string> vm_signatures = {
    "vbox", "virtualbox", "vmware", "vmware, inc.", "qemu", "kvm", "microsoft"
};
static std::string to_lower(const std::string &s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), [](unsigned char c){ return std::tolower(c); });
    return r;
}
bool detect_bios_vendor(std::string &out_info) noexcept {
    out_info.clear();
#ifdef _WIN32
    DWORD needed = ::GetSystemFirmwareTable(0x52534D42, 0, nullptr, 0); // 'RSMB'
    if (needed == 0) return false;
    std::string buf(needed, '\0');
    DWORD read = ::GetSystemFirmwareTable(0x52534D42, 0, &buf[0], (DWORD)buf.size());
    if (read == 0) return false;
    std::string lower = to_lower(buf);
    for (auto &sig : vm_signatures)
        if (lower.find(sig) != std::string::npos) { out_info = sig; return true; }
    return false;
#else
    const char* paths[] = {"/sys/class/dmi/id/bios_vendor","/sys/class/dmi/id/product_name","/sys/class/dmi/id/sys_vendor"};
    for (auto p : paths) {
        std::ifstream ifs(p);
        if (!ifs) continue;
        std::string content;
        std::getline(ifs, content);
        std::string low = to_lower(content);
        for (auto &sig : vm_signatures)
            if (low.find(sig) != std::string::npos) { out_info = content; return true; }
    }
    return false;
#endif
}}