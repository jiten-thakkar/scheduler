#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Task.h"

Class Scheduler {
  public:
    bool createTask(Schedule*);
    bool modifyTaskFrequency(long, int);
    bool cancelTask(long);
  private:
    std::map<long, Schedule*> scheduledTasks;
    long taskCounter=0;

    void doTheDeeds(Schedule*);
};

#endif // SCHEDULER_H
