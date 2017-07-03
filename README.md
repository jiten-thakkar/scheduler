# scheduler
A simple Scheduler service which can be used to run any task at given frequency and the scheduler would store data returned by the task in sqlite database called `scheduler.db`

Task [VirtualMemoryTask](lib/VirtualMemoryUsageTask.cpp) collects virtual memory used and total virtual memory when run and task [TCPTimeTask](lib/TCPTimeTask.cpp) collects conneciton time to `www.google.com` at port `80`. The code in [example](src/main.cpp) runs the task using the scheduler every 5 seconds and stores it in `data` table in `scheduler.db`.

steps for building the scheduler:

```
install sqlite3 library
clone the repository
cd scheduler
git submodule init
git submodule update
cd dependency/
cd ../..
mkdir build
cd build
cmake ..
make
./example
```
