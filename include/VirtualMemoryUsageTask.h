#ifndef VIRTUAL_MEMORY_USAGE_TASK_H 
#define VIRTUAL_MEMORY_USAGE_TASK_H 

Class VirtualMemoryUsageTask : public Task {
  private:
    static const std::string id = "cd012a52-e59e-446e-a476-1d5b95af87af";
    static const std::string m_description("Task to collect data about total virtual memory and virtual memory being used.");
  public:
    VirtualMemoryUsageTask():Task(id, m_description) {}  
};

#endif //VIRTUAL_MEMORY_USAGE_TASK_H
