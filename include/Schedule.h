#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <iostream>
#include <chrono>

class Schedule {
  public:
    Schedule(Task* task, bool recurring, long frequency):m_task(task), 
      m_recurring(recurring), m_frequency(frequency) {}

    const long getId() {
      return m_id;
    }

    void setId(long id) {
      m_id = id;
    }

    const Task* getTask() {
      return m_task;
    }

    bool isRecurring() {
      return m_recurring;
    }

    void setRecurring(bool recurring) {
      m_recurring = recurring;
    }

    long getFrequency() {
      return m_frequency;
    }

    void setFrequency(long frequency) {
      m_frequency = frequency;
    }

  private:
    long m_id;
    Task* m_task;
    bool m_recurring;
    long m_frequency;
};

#endif // SCHEDULE_H
