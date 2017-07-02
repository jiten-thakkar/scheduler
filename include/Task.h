#ifndef TASK_H
#define TASK_H

#include <vector>

Class Task {
  private:
    static const std::string m_id;
  public:
    Task(std::string id):m_id(id) {}

    std::string getTaskId() {
      return m_id;
    }

    virtual std::map<int, double> operation();
};

#endif //TASK_H
