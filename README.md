# scheduler
A simple Scheduler service which can be used to run any task at given frequency and the scheduler would store data returned by the task in sqlite database called scheduler.db

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
