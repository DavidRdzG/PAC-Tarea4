#include "MacDetect.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <ws2tcpip.h>
  #include <iphlpapi.h>
  #pragma comment(lib, "iphlpapi.lib")
#else
  #include <ifaddrs.h>
  #include <net/if.h>
  #include <sys/ioctl.h>
  #include <unistd.h>
  #include <sys/socket.h>
  #include <netpacket/packet.h>
  #include <net/ethernet.h>
#endif
namespace AntiVM {
static const std::vector<std::string> vm_ouis = {
    "08:00:27","00:05:69","00:0c:29","00:50:56","00:03:ff","00:15:5d"
};
static std::string mac_to_str(const unsigned char *mac, size_t len) {
    std::ostringstream oss;
    for (size_t i=0;i<len;++i){ if(i)oss<<":"; oss<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)mac[i]; }
    std::string s=oss.str(); std::transform(s.begin(),s.end(),s.begin(),::tolower); return s;
}
bool detect_mac_oui(std::string &out_info) noexcept {
    out_info.clear();
#ifdef _WIN32
    ULONG flags = 0;
    ULONG family = AF_UNSPEC;
    IP_ADAPTER_ADDRESSES *adapter_addresses=nullptr; ULONG out_len=0;
    if(GetAdaptersAddresses(family,flags,nullptr,adapter_addresses,&out_len)==ERROR_BUFFER_OVERFLOW)
        adapter_addresses=(IP_ADAPTER_ADDRESSES*)malloc(out_len);
    if(!adapter_addresses) return false;
    if(GetAdaptersAddresses(family,flags,nullptr,adapter_addresses,&out_len)!=NO_ERROR){ free(adapter_addresses); return false; }
    IP_ADAPTER_ADDRESSES *a=adapter_addresses;
    while(a){ if(a->PhysicalAddressLength>=6){ std::string mac=mac_to_str(a->PhysicalAddress,a->PhysicalAddressLength);
        for(auto &oui:vm_ouis){ if(mac.substr(0,8)==oui){ out_info=mac; free(adapter_addresses); return true; } } }
        a=a->Next; }
    free(adapter_addresses); return false;
#else
    struct ifaddrs *ifaddr=nullptr; if(getifaddrs(&ifaddr)==-1)return false;
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    for(struct ifaddrs *ifa=ifaddr;ifa!=nullptr;ifa=ifa->ifa_next){ if(!ifa->ifa_name)continue;
        struct ifreq ifr; memset(&ifr,0,sizeof(ifr)); strncpy(ifr.ifr_name,ifa->ifa_name,IFNAMSIZ-1);
        if(ioctl(sock,SIOCGIFHWADDR,&ifr)==0){ unsigned char *mac=(unsigned char*)ifr.ifr_hwaddr.sa_data;
            std::string macs=mac_to_str(mac,6); for(auto &oui:vm_ouis){ if(macs.substr(0,8)==oui){ out_info=macs; close(sock); freeifaddrs(ifaddr); return true; } } } }
    close(sock); freeifaddrs(ifaddr); return false;
#endif
}}