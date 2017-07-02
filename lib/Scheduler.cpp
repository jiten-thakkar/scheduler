#ifndef SCHEDULER_H
#undef SCHEDULER_H

#include <iostream>
#include <thread>

#include "Scheduler.h"
#include "Schedule.h"

#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "KompexSQLiteStreamRedirection.h"
#include "KompexSQLiteBlob.h"

void Scheduler::init() {
  m_taskDatabase = new Kompex::SQLiteDatabase(m_taskDBName, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
  taskStmt = new Kompex::SQLiteStatement(m_taskDatabase);
  taskStmt->SqlStatement(taskCreateStatement);
  m_dataDatabase = new Kompex::SQLiteDatabase(m_dataDBName, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
  dataStmt = new Kompex::SQLiteStatement(m_dataDatabase);
  dataStmt->SqlStatement(dataCreateStatement);
}

bool Scheduler::createTask(Schedule* sch) {
  //return false if task has already been created
  if (m_scheduledTasks.find(sch->getTask()->getId()) != m_scheduledTasks.end())
    return false; 
  std::thread fireSchedule(doTheDeeds, sch);
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
  m_scheduledTask.erase(m_scheduledTasks.find(id));
  return true;
}

void Scheduler::doTheDeeds(Schedule* sch) {
  std::vector<double> min(sch->getTask()->getNumberofMatrices(), 0);
  std::vector<double> avg(sch->getTask()->getNumberofMatrices(), 0);
  std::vector<double> max(sch->getTask()->getNumberofMatrices(), 0);
  dataStmt->Sql(getLatestDataWithTaskid);
  dataStmt->BindString(0, sch->task->getId());
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
      int metricId = d->first;
      double data = d->second;
      if (data < min[metricId])
        min[metricId] = data;
      if (data > max[metricId])
        max[metricId] = data;
      avg[metricId] = (data + avg[metricId])/2;
      dataStmt->SqlStatement(insertIntoData);
      dataStmt->BindString(DATA_TASKID_COLUMN, sch->getTask()->getId());
      dataStmt->BindInt(DATA_METRICID_COLUMN, metricId);
      dataStmt->BindDouble(DATA_VAL_COLUMN, val);
      dataStmt->BindDouble(DATA_MIN_COLUMN, min);
      dataStmt->BindDouble(DATA_AVG_COLUMN, avg);
      dataStmt->BindDouble(DATA_MAX_COLUMN, max);
      dataStmt->ExecuteAndFree();
    }
    std::this_thread::sleep_for (std::chrono::seconds(sch->getFrequency()));
  } while (sch->isRecurring());
}

#endif // SCHEDULER_H
