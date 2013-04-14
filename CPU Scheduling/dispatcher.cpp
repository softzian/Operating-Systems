// header files
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <iostream>
#include <iomanip> // for setPrecision
#include <fstream>

using namespace std;

// global variables
int tick = 1;
int runTime = 0;

// structs
struct process
{
 int arrival;
 int priority;
 int procTime;
 int timeRemaining;
 int startTime;
 int endTime;
 bool started;
};

// main
int main(int argc, char *argv[])
{
 // initialize variables
 int tempArrival, tempPriority, tempProcTime, numProcesses = 0, totalProcTime = 0;
 float turnaroundTime = 0, normTurnaroundTime = 0, waitTime = 0;
 char *fname, comma;
 bool fileEnd = false, programEnd = false, notDone = false;
 ifstream fin;
 process currProcess, newProcess, tempProcess;
 queueType<process> realTime(100), finishedQueue(100);
 queueType<process> userPriority1(100), userPriority2(100), userPriority3(100);

 // open file
 fname = new char[100];
 fname = argv[1];
 fin.open(fname);

 // check how many processes are in file
 while(fin.good())
 {
  fin >> tempArrival >> comma >> tempPriority >> comma >> tempProcTime;
  if(fin.good())
   numProcesses++;
 }

 // reset file for execution
 fin.close();
 fin.open(fname);

 // begin algorithm
 while(programEnd != true)
 {
  // cycle through entire file comparing to current tick value
  // to see if there are any new jobs to be enqueued
  while(fin.good())
  {
   // obtain process data
   fin >> tempArrival >> comma >> tempPriority >> comma >> tempProcTime;
   // file end duplication protection
   if(!fin.good())
    fileEnd = true;

/*----------------------------QUEUE PLACEMENT--------------------------*/

   // if a process has arrived at the current time, place it in correct queue
   if(runTime == tempArrival && fileEnd == false)
   {
    if(tempPriority == 0)
    {
     // create a process struct for enqueueing
     newProcess.arrival = tempArrival;
     newProcess.priority = tempPriority;
     newProcess.procTime = tempProcTime;
     newProcess.started = false;
     realTime.enqueue(newProcess);
    }
    // 1 priority means it belongs to userPriority 1 queue
    else if(tempPriority == 1)
    {
     // create a process struct for enqueueing
     newProcess.arrival = tempArrival;
     newProcess.priority = tempPriority;
     newProcess.procTime = tempProcTime;
     newProcess.started = false;
     userPriority1.enqueue(newProcess);
    }
    // 2 priority means it belongs to userPriority 2 queue
    else if(tempPriority == 2)
    {
     // create a process struct for enqueueing
     newProcess.arrival = tempArrival;
     newProcess.priority = tempPriority;
     newProcess.procTime = tempProcTime;
     newProcess.started = false;
     userPriority2.enqueue(newProcess);
    }
    // otherwise it has a 3rd level priority
    else
    {
     // create a process struct for enqueueing
     newProcess.arrival = tempArrival;
     newProcess.priority = tempPriority;
     newProcess.procTime = tempProcTime;
     newProcess.started = false;
     userPriority3.enqueue(newProcess);
    }
   } // if(runTime == tempArrival && fileEnd == false) end
   // if the last process wasn't done and no new processes arrive
   // it's ok to enqueue
  }// while fin.good() end

  if(notDone == true)
  {
   if(currProcess.priority == 0)
   {
    realTime.enqueue(currProcess);
    notDone = false;
   }
   else if(currProcess.priority == 1)
   {
    currProcess.priority++;
    userPriority2.enqueue(currProcess);
    notDone = false;
   }
   else if(currProcess.priority == 2)
   {
    currProcess.priority++;
    userPriority3.enqueue(currProcess);
    notDone = false;
   }
   else
   {
    userPriority3.enqueue(currProcess);
    notDone = false;
   }
  }

/*-------------------------RUN CORRECT PROCESS------------------*/

  // first check if there are any real time processes that are ready
  if(!realTime.isEmpty())
  {
   // tick the time of the next process
   realTime.dequeue(currProcess);

   if(currProcess.started == false)
   {
    currProcess.started = true;
    currProcess.startTime = runTime;
    currProcess.timeRemaining = currProcess.procTime;
   }

   currProcess.timeRemaining -= tick;

   // if the process is not done executing, enqueue it again
   if(currProcess.timeRemaining != 0)
    notDone = true;
   else
   {
    numProcesses--;
    currProcess.endTime = runTime+1;
    finishedQueue.enqueue(currProcess);
   }
  }
  // otherwise real time queue is empty, check the 3 user level queues
  else
  {
   // if level 1 queue is not empty, get next process
   if(!userPriority1.isEmpty())
   {
    // tick the time of the next process
    userPriority1.dequeue(currProcess);

    if(currProcess.started == false)
    {
     currProcess.started = true;
     currProcess.startTime = runTime;
     currProcess.timeRemaining = currProcess.procTime;
    }

    currProcess.timeRemaining -= tick;

    // if the process is not done, demote to a lower level
    if(currProcess.timeRemaining != 0)
     notDone = true;
    else
    {
     numProcesses--;
     currProcess.endTime = runTime+1;
     finishedQueue.enqueue(currProcess);
    }
   }
   // otherwise if level 2 queue is not empty, get next process
   else if(!userPriority2.isEmpty())
   {
    // tick the time of the next process
    userPriority2.dequeue(currProcess);

    if(currProcess.started == false)
    {
     currProcess.started = true;
     currProcess.startTime = runTime;
     currProcess.timeRemaining = currProcess.procTime;
    }

    currProcess.timeRemaining -= tick;

    // if the process is not done, demote to a lower level
    if(currProcess.timeRemaining != 0)
     notDone = true;
    else
    {
     numProcesses--;
     currProcess.endTime = runTime+1;
     finishedQueue.enqueue(currProcess);
    }
   }
   // otherwise if level 1 and 2 queues are empty and level 3 is not, get next process
   else if(!userPriority3.isEmpty())
   {
    // tick the time of the next process
    userPriority3.dequeue(currProcess);

    if(currProcess.started == false)
    {
     currProcess.started = true;
     currProcess.startTime = runTime;
     currProcess.timeRemaining = currProcess.procTime;
    }

    currProcess.timeRemaining -= tick;

    // enqueue process again if it is still not done
    if(currProcess.timeRemaining != 0)
     notDone = true;
    else
    {
     numProcesses--;
     currProcess.endTime = runTime+1;
     finishedQueue.enqueue(currProcess);
    }
   }
  }

  // re-initialize file for loop again if all processes have not finished
  if(numProcesses != 0)
  {
   runTime++;

   fin.close();
   fin.open(fname);
   fileEnd = false;
  }
  // otherwise all processes have finished and program is done
  else
   programEnd = true;
 } // while(programEnd != true) end

/*----------------------------DATA CALCULATIONS------------------------*/

while(!finishedQueue.isEmpty())
{
 finishedQueue.dequeue(tempProcess);
 turnaroundTime += tempProcess.endTime - tempProcess.arrival;
 normTurnaroundTime += (tempProcess.endTime - tempProcess.arrival) / tempProcess.procTime;
 waitTime += tempProcess.endTime - tempProcess.arrival - tempProcess.procTime;
 numProcesses++;
}

 turnaroundTime /= numProcesses;
 normTurnaroundTime /= numProcesses;
 waitTime /= numProcesses;

 // output data
 cout << fixed;
 cout << endl << "Average turnaround time = " << setprecision(2) << turnaroundTime << endl;
 cout << "Average normalized turnaround Time = " << normTurnaroundTime << endl;
 cout << "Average waiting time = " << waitTime << endl << endl;

 // close file
 fin.close();

 return 0;
} // main end
