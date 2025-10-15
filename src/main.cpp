#include <iostream>

#include <iomanip>

#include <string>

#include "CpuidDetect.h"

#include "BiosDetect.h"

#include "MacDetect.h"

using namespace std;
static string yesNoNA(bool val, bool supported) {
  if (!supported) return "N/A";
  return val ? "Detecta" : "No Detecta";
}
int main(int argc, char ** argv) {
  bool run_cpuid = true, run_bios = true, run_mac = true;
  if (argc >= 3 && string(argv[1]) == "--technique") {
    int n = atoi(argv[2]);
    run_cpuid = run_bios = run_mac = false;
    if (n == 1) run_cpuid = true;
    if (n == 2) run_bios = true;
    if (n == 3) run_mac = true;
  }
  cout << "Anti-VM Detector (C++17)\n";
  bool is_windows = false;
  #if defined(_WIN32) || defined(__MINGW32__) || defined(__MINGW64__)
  is_windows = true;
  #endif
  cout << "\nResumen:\n\n" << left << setw(30) << "Técnica" << setw(18) << "Windows" << setw(18) << "Linux" << "Info\n" << string(80, '-') << "\n";
  string info;
  bool cpuid_val = false, cpuid_supported = false;
  if (run_cpuid) {
    cpuid_val = AntiVM::detect_cpuid_hypervisor(info);
    cpuid_supported = true;
  } else info = "Skipped";
  cout << left << setw(30) << "CPUID Hypervisor Bit" << setw(18) << (is_windows ? yesNoNA(cpuid_val, cpuid_supported) : "N/A") << setw(18) << (!is_windows ? yesNoNA(cpuid_val, cpuid_supported) : "N/A") << info << "\n";
  bool bios_val = false, bios_supported = false;
  info.clear();
  if (run_bios) {
    bios_val = AntiVM::detect_bios_vendor(info);
    bios_supported = true;
  } else info = "Skipped";
  cout << left << setw(30) << "BIOS Vendor String" << setw(18) << (is_windows ? yesNoNA(bios_val, bios_supported) : "N/A") << setw(18) << (!is_windows ? yesNoNA(bios_val, bios_supported) : "N/A") << info << "\n";
  bool mac_val = false, mac_supported = false;
  info.clear();
  if (run_mac) {
    mac_val = AntiVM::detect_mac_oui(info);
    mac_supported = true;
  } else info = "Skipped";
  cout << left << setw(30) << "MAC OUI" << setw(18) << (is_windows ? yesNoNA(mac_val, mac_supported) : "N/A") << setw(18) << (!is_windows ? yesNoNA(mac_val, mac_supported) : "N/A") << info << "\n";
  cout << "\nNotas:\n- Ejecuta en Host/VMs y guarda la salida.\n- Usa --technique 1|2|3 para probar solo una.\n";
  return 0;
}