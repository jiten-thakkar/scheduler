#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <map>

#include "Task.h"
#include "Schedule.h"

#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "KompexSQLiteStreamRedirection.h"
#include "KompexSQLiteBlob.h"

#define NAME(x) #x
#define SCHEDULER_DB_NAME NAME(scheduler.db)
#define DATA_TASKID_COLUMN 1 
#define DATA_METRICID_COLUMN 2 
#define DATA_VAL_COLUMN 3
#define DATA_MIN_COLUMN 4
#define DATA_AVG_COLUMN 5
#define DATA_MAX_COLUMN 6

class Scheduler {
  public:
    void init();
    bool createTask(Schedule*);
    bool modifyTaskFrequency(std::string, long);
    bool cancelTask(std::string);
  private:
    std::map<std::string, Schedule*> m_scheduledTasks;
    long m_taskCounter=0;
    const std::string m_schedulerDBName = SCHEDULER_DB_NAME;
    Kompex::SQLiteDatabase* m_schedulerDatabase;
    Kompex::SQLiteStatement* taskStmt;
    Kompex::SQLiteStatement* dataStmt;
    const std::string taskCreateStatement = "CREATE TABLE IF NOT EXISTS task (id VARCHAR(255) \
      primary key, description VARCHAR(255))";
    const std::string dataCreateStatement = "CREATE TABLE IF NOT EXISTS data (id integer primary \
      key autoincrement, time timestamp default (strftime('%s', 'now')), taskid VARCHAR(20), \
       metricid INTEGER NOT NULL, val DOUBLE NOT NULL, min DOUBLE NOT NULL, \
      avg DOUBLE NOT NULL, max DOUBLE NOT NULL, FOREIGN KEY(taskid) REFERENCES task(id))";
    const std::string insertIntoData = "INSERT INTO data (taskid, metricid, val, min, avg, max) \
        VALUES (?, ?, ?, ?, ?, ?)";
    const std::string insertIntoTask = "INSERT INTO task (id, description) VALUES (?, ?)";
    const std::string getTaskWhereId = "SELECT * FROM task WHERE id=@taskid";
    const std::string getDataWhereTaskid = "SELECT * FROM data WHERE taskid=@id";
    const std::string getLatestDataWithTaskid = "Select * FROM data WHERE taskid=@id ORDER BY time \
        DESC LIMIT 1";

    void doTheDeeds(Schedule*);
};

#endif // SCHEDULER_H
