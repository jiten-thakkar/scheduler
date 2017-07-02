#include <iostream>
#include <map>
#include "sys/types.h"
#include "sys/sysinfo.h"

#include "VirtualMemoryUsageTask.h"

std::map<int, double> VirtualMemoryUsageTask::operation() {
  std::map<int, double> result;
  struct sysinfo memInfo;
  sysinfo (&memInfo);
  double totalVirtualMem = memInfo.totalram;
  totalVirtualMem += memInfo.totalswap;
  totalVirtualMem *= memInfo.mem_unit;
  result[0] = totalVirtualMem;

  double virtualMemUsed = memInfo.totalram - memInfo.freeram;
  virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
  virtualMemUsed *= memInfo.mem_unit;
  result[1] = virtualMemUsed;
  
  return result;
}
