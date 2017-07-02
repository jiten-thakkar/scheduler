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
  double min = avg = max = 0;
  dataStmt->Sql(getLatestDataWithTaskid);
  dataStmt->BindString(0, sch->task->getId());
  assert(dataStmt->GetDataCount() <= 1 && "Query should have returned at max 1 raw of data");
  if (dataStmt->GetDataCount() > 0) {
    min = dataStmt->GetColumnDouble(DATA_MIN_COLUMN);
    avg = dataStmt->GetColumnDouble(DATA_AVG_COLUMN);
    max = dataStmt->GetColumnDouble(DATA_MAX_COLUMN);
  }
  dataStmt->FreeQuery();
  do {
    vector<double> data = sch->getTask()->operation();
    for (auto& d : data) {
      if (d < min)
        min = d;
      if (d > max)
        max = d;
      avg = (d + avg)/2;
      dataStmt->SqlStatement(insertIntoData);
      dataStmt->BindString(DATA_TASKID_COLUMN, sch->getTask()->getId());
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
