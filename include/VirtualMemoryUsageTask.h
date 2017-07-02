#ifndef VIRTUAL_MEMORY_USAGE_TASK_H 
#define VIRTUAL_MEMORY_USAGE_TASK_H 

#include "Task.h"

class VirtualMemoryUsageTask: public Task {
  private:
    static const std::string id;
    static const std::string m_description;
    static const int m_numberof_metrices;

    std::map<int, double> operation() const;
  public:
    VirtualMemoryUsageTask():Task(id, m_description, m_numberof_metrices) {}  
};



#endif //VIRTUAL_MEMORY_USAGE_TASK_H
