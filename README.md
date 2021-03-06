# scheduler
A simple Scheduler service which can be used to run any task at given frequency and the scheduler would store data returned by the task in sqlite database called `scheduler.db`

Task [VirtualMemoryTask](lib/VirtualMemoryUsageTask.cpp) collects virtual memory used and total virtual memory when run and task [TCPTimeTask](lib/TCPTimeTask.cpp) collects conneciton time to `www.google.com` at port `80`. The code in [example](src/main.cpp) runs the task using the scheduler every 5 seconds and stores it in `data` table in `scheduler.db`.

steps for building the scheduler on Ubuntu, requires cmake 3.1.0+:
1) install sqlite3 library (sudo apt-get install sqlite3 libsqlite3-dev in ubuntu)
2) install pthread (sudo apt-get install libpthread-stubs0-dev in ubuntu)
2) clone the repository
3) follow these commands

```
cd scheduler
git submodule init
git submodule update
cd dependency/kompex-sqlite-wrapper/
./configure && make
cd ../..
mkdir build
cd build
cmake ..
make
./example
```
