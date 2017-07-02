#ifndef TASK_H
#define TASK_H

#include <map>

class Task {
  private:
    const std::string m_id;
    const std::string m_description;
    const int m_numberof_matrices;
  public:
    Task(std::string id, std::string description, int numberof_matrices):m_id(id), m_description(description),
       m_numberof_matrices(numberof_matrices) {}

    std::string getId() const {
      return m_id;
    }

    std::string getDescription() const {
      return m_description;
    }

    int getNumberofMatrices() const {
      return m_numberof_matrices;
    }

    virtual std::map<int, double> operation() const = 0;
};

#endif //TASK_H
