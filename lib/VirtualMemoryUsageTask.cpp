#include <iostream>
#include <map>
#include "sys/types.h"
#include "sys/sysinfo.h"

#include "VirtualMemoryUsageTask.h"

std::map<int, double> VirtualMemoryUsageTask::operation() const {
  std::map<int, double> result;
  struct sysinfo memInfo;
  //getting system info
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

const std::string VirtualMemoryUsageTask::id = "cd012a52-e59e-446e-a476-1d5b95af87af";

const std::string VirtualMemoryUsageTask::m_description = "Task to collects data about total virtual memory and virtual memory being used.";
    
const int VirtualMemoryUsageTask::m_numberof_metrices = 2;
