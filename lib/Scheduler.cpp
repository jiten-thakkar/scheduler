#ifndef SCHEDULER_H
#undef SCHEDULER_H

#include <iostream>
#include <thread>
#include <map>
#include <vector>

#include "Scheduler.h"

#define log std::cout

void Scheduler::init() {
  m_schedulerDatabase = new Kompex::SQLiteDatabase(m_schedulerDBName, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
  taskStmt = new Kompex::SQLiteStatement(m_schedulerDatabase);
  taskStmt->SqlStatement(taskCreateStatement);
  dataStmt = new Kompex::SQLiteStatement(m_schedulerDatabase);
  dataStmt->SqlStatement(dataCreateStatement);
}

bool Scheduler::createTask(Schedule* sch) {
  //return false if task has already been created
  if (m_scheduledTasks.find(sch->getTask()->getId()) != m_scheduledTasks.end())
    return false; 
  taskStmt->Sql(getTaskWhereId);
  taskStmt->BindString(1, sch->getTask()->getId());
  taskStmt->Execute();
  if (taskStmt->GetDataCount() == 0) {
    taskStmt->FreeQuery();
    taskStmt->Sql(insertIntoTask);
    taskStmt->BindString(1, sch->getTask()->getId());
    taskStmt->BindString(2, sch->getTask()->getDescription());
    taskStmt->ExecuteAndFree();
  } else {
    taskStmt->FreeQuery();
  }
  std::thread fireSchedule(&Scheduler::doTheDeeds, this, sch);
  fireSchedule.detach();
  sch->setId(++m_taskCounter);
  m_scheduledTasks[sch->getTask()->getId()] = sch;
  return true;
}

bool Scheduler::modifyTaskFrequency(std::string id, long newFrequency) {
  if (m_scheduledTasks.find(id) == m_scheduledTasks.end())
    return false;
  m_scheduledTasks[id]->setFrequency(newFrequency); 
  return true;
}

bool Scheduler::cancelTask(std::string id) {
  if (m_scheduledTasks.find(id) == m_scheduledTasks.end())
    return false;
  //setting recurrance to false so that in the next execution
  //the task thread will exit
  m_scheduledTasks[id]->setRecurring(false);
  m_scheduledTasks.erase(m_scheduledTasks.find(id));
  return true;
}

void Scheduler::doTheDeeds(Schedule* sch) {
  std::vector<double> min(sch->getTask()->getNumberofMatrices(), 0);
  std::vector<double> avg(sch->getTask()->getNumberofMatrices(), 0);
  std::vector<double> max(sch->getTask()->getNumberofMatrices(), 0);
  dataStmt->Sql(getLatestDataWithTaskid);
  dataStmt->BindString(1, sch->getTask()->getId());
  if (dataStmt->GetDataCount() > 0) {
    while(dataStmt->FetchRow()) {
      int metricId = dataStmt->GetColumnInt(DATA_METRICID_COLUMN);
      min[metricId] = dataStmt->GetColumnDouble(DATA_MIN_COLUMN);
      avg[metricId] = dataStmt->GetColumnDouble(DATA_AVG_COLUMN);
      max[metricId] = dataStmt->GetColumnDouble(DATA_MAX_COLUMN);
    }
  }
  dataStmt->FreeQuery();
  do {
    std::map<int, double> data = sch->getTask()->operation();
    for (auto& d : data) {
      int metricId = d.first;
      double data = d.second;
      if (data < min[metricId] || min[metricId] == 0)
        min[metricId] = data;
      if (data > max[metricId])
        max[metricId] = data;
      avg[metricId] = (data + avg[metricId])/2;
      dataStmt->Sql(insertIntoData);
      dataStmt->BindString(DATA_TASKID_COLUMN, sch->getTask()->getId());
      dataStmt->BindInt(DATA_METRICID_COLUMN, metricId);
      dataStmt->BindDouble(DATA_VAL_COLUMN, data);
      dataStmt->BindDouble(DATA_MIN_COLUMN, min[metricId]);
      dataStmt->BindDouble(DATA_AVG_COLUMN, avg[metricId]);
      dataStmt->BindDouble(DATA_MAX_COLUMN, max[metricId]);
      dataStmt->ExecuteAndFree();
    }
    std::this_thread::sleep_for(std::chrono::seconds(sch->getFrequency()));
  } while (sch->isRecurring());
}

#endif // SCHEDULER_H
