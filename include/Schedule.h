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

    const task* getTask() {
      return m_task;
    }

    bool isRecurring() {
      return m_recurring;
    }

    void setRecurring(bool recurring) {
      m_recurring = recurring;
    }

    //returns next execution time in seconds since epoch
    long getNextExecutionTime() {
      using namespace std::chrono;
      system_clock::time_point tp = system_clock::now();
      seconds dtn = tp.time_since_epoch();
      return dtn.count() + m_frequency;
    }

    long getFrequency() {
      return m_frequency;
    }

    void setFrequency(long frequency) {
      m_frequency = frequency;
    }

    const Task* getTask() {
      return m_task;
    }
  private:
    const long m_id;
    Task* m_task;
    bool m_recurring;
    long m_frequency;
};

#endif // SCHEDULE_H
