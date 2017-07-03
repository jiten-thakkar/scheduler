#ifndef TCP_TIME_TASK_H 
#define TCP_TIME_TASK_H 

#include "Task.h"

class TCPTimeTask: public Task {
  private:
    static const std::string id;
    static const std::string m_description;
    static const int m_numberof_metrices;

    std::map<int, double> operation() const;
  public:
    TCPTimeTask():Task(id, m_description, m_numberof_metrices) {}  
};



#endif //TCP_TIME__TASK_H
