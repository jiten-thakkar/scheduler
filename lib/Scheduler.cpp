#ifndef SCHEDULER_H
#undef SCHEDULER_H

#include <iostream>
#include <thread>

#include "Scheduler.h"
#include "Schedule.h"

bool Scheduler::createTask(Schedule* sch) {
  std::thread fireSchedule(doTheDeeds, sch);
  sch->setId(++taskCounter);
  scheduledTasks[sch->getId()] = sch; 
  return true;
}

bool Scheduler::modifyTaskFrequency(long id, long newFrequency) {
  if (scheduledTasks.find(id) == scheduledTasks.end())
    return false;
  scheduledTasks[id]->setFrequency(newFrequency); 
  return true;
}

bool Scheduler::cancelTask(long id) {
  if (scheduledTasks.find(id) == scheduledTasks.end())
    return false;
  scheduledTasks[id]->setRecurring(false);
  return true;
}

void Scheduler::doTheDeeds(Schedule* sch) {
  vector<double> data = sch->getTask()->operation();
  // To do: put the data in database
  if(sch->isRecurring()) {
    std::this_thread::sleep_for (std::chrono::seconds(sch->getFrequency()));
  }
}
#endif // SCHEDULER_H
