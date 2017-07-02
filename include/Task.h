#ifndef TASK_H
#define TASK_H

#include <map>

Class Task {
  private:
    const std::string m_id;
    const std::string m_description;
  public:
    Task(std::string id, std::string description):m_id(id), m_description(description) {}

    std::string getTaskId() {
      return m_id;
    }

    std::string getDescription() {
      return m_description;
    }

    virtual std::map<int, double> operation();
};

#endif //TASK_H
