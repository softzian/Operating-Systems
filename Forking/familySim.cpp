// header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> // required for waitpid
#include <fstream>
#include <iostream>

using namespace std;

// main
int main(int agrc, char *argv[])
{
 // initialize variables
 char p1name, p2name, childName, testName;
 char count = '0', numChildren;
 int i = 0, tabCount = 0, start, storagePtr = 3;
 bool married = false;
 pid_t pid[100];
 ofstream fout;
 ifstream fin;

 // open file
 fin.open(argv[1]);
 fout.open("output.txt");

 // obtain parent and children information
 fin >> p1name >> p2name >> numChildren;
 fout << p1name << "-" << p2name << endl;

 if(fin.good())
 {
  // initialize pid[0] to 1 for parent to begin loop
  // if there are children to create
  if(numChildren != '0')
   pid[i] = 1;

/* ---------------------- START PARENT PROCESS -------------------- */

  // loop until all children of parent are created
  while(pid[i] != 0 && count < numChildren)
  {
   // fork a new child
   pid[i] = fork();
   count++;

   // wait for children to finish
   waitpid(pid[i], NULL, 0);

   // test if error
   if(pid[i] < 0)
   {
    cout << "ERROR";
    exit(1);
   }
   // test if parent process
   else if(pid[i] != 0)
   {
    // increment i for parent to create a child
    // and then store its pid at next part of array
    i++;

    // ensures that parent enters next loop iteration
    pid[i] = 1;
   }
/*---------------------END PARENT PROCESS-------------------*/
/*---------------------BEGIN CHILD PROCESS------------------*/
   else
   {
    // loop to retrieve correct child ID
    while(count > '0')
    {
     fin >> childName;
     count--;
    }

    // loop to see if married
    while(fin.good() && !married)
    {
     fin >> testName;

     // test name appears twice, they are married
     if(testName == childName)
      married = true;
    }

    if(married)
    {
     // make child first parent and obtain 2nd parent + numChildren
     p1name = childName;
     fin >> p2name >> numChildren;

     // output correct amount of tabs
     for(int j=-1; j<tabCount; j++)
      fout << "\t";

     // write new couple to file
     fout << p1name << "-" << p2name << endl;

     // reset variables correctly as this child process 
     // will enter the parent process statements
     i = 0;
     count = '0';
     tabCount++;
     married = false;
     if(numChildren != '0')
      pid[i] = 1;

    }
    // if not married then single
    else
    {
     // write correct amount of tabs
     for(int j=-1; j<tabCount; j++)
      fout << "\t";

     // write single child to file
     fout << childName << endl;

     // no children or partner, child process done
     exit(0);
    }
   }

/*---------------------END CHILD PROCESS---------------------*/

  } // while count < numChildren end
 } // if fin.good() end

 // close files
 fin.close();
 fout.close();

 // return success
 return 0;

} // main end

