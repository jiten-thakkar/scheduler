#include <iostream>
#include <thread>

#include "Scheduler.h"
#include "VirtualMemoryUsageTask.h"
#include "TCPTimeTask.h"

#define log std::cout

int main() {
  try {
    Scheduler* scheduler = new Scheduler();
    scheduler->init();
    log << "scheduler initiated\n";
    VirtualMemoryUsageTask* memoryTask = new VirtualMemoryUsageTask();
    Schedule* memorySchedule = new Schedule(memoryTask, true, 5);
    if(!scheduler->createTask(memorySchedule)) {
      std::cout << "Memory task already scheduled.\n";
    } else {
      std::cout << "Memory task scheduled.\n";
    }

    TCPTimeTask* tcpTask = new TCPTimeTask();
    Schedule* tcpSchedule = new Schedule(tcpTask, true, 5);
    if(!scheduler->createTask(tcpSchedule)) {
      std::cout << "TCP time task already scheduled.\n";
    } else {
      std::cout << "TCP time task scheduled.\n";
    }

    
    std::cout << "sleeping this main thread for 15 seconds.\n";
    std::this_thread::sleep_for(std::chrono::seconds(15));


    if(!scheduler->modifyTaskFrequency(memoryTask->getId(), 10)) {
      std::cout << "Couldn't reschedule the memory task.\n";
    } else {
      std::cout << "Rescheduled memory task to 10 second frequency.\n";
    }

    if(!scheduler->modifyTaskFrequency(tcpTask->getId(), 10)) {
      std::cout << "Couldn't reschedule the TCP task.\n";
    } else {
      std::cout << "Rescheduled TCP task to 10 second frequency.\n";
    }

    std::cout << "sleeping this main thread for 35 seconds.\n";
    std::this_thread::sleep_for(std::chrono::seconds(35));


    if(!scheduler->cancelTask(memoryTask->getId())) {
      std::cout << "Couldn't cancel the memory task.\n";
    } else {
      std::cout << "Cancelled the memory task.\n";
    }
    if(!scheduler->cancelTask(tcpTask->getId())) {
      std::cout << "Couldn't cancel the TCP task.\n";
    } else {
      std::cout << "Cancelled the TCP task.\n";
    }


    free(memorySchedule);
    free(memoryTask);
    free(tcpTask);
    free(scheduler); 
  } catch (Kompex::SQLiteException &exception) {
    std::cerr << "\nException Occured" << std::endl;
    exception.Show();
    std::cerr << "SQLite result code: " << exception.GetSqliteResultCode() << std::endl;
  }

  return 0;
}
