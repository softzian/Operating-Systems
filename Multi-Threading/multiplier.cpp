// header files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h> // for threads
#include <fstream>
#include <iostream>

using namespace std;

// structs
struct pos
{
 int i; /* row */
 int j; /* col */
};

// global variables
int **matrixA;
int **matrixB;
int **matrixC;
int M, N, K; // row/column values for Matrix A, B or C

// function prototypes
void* getVal(void* vdata);

// main
int main(int agrc, char *argv[])
{
 // initialize variables
 pthread_t *t;
 ofstream fout;
 ifstream matrix1, matrix2;
 int i, j, k;

 // open input matrix files
 matrix1.open(argv[1]);
 matrix2.open(argv[2]);

 // open product matrix file
 fout.open("product.txt");

 // obtain number of rows and columns for matrices
 matrix1 >> M;
 matrix1 >> K;
 matrix2 >> K;
 matrix2 >> N;

 /*-----------------------BEGIN MEMORY ALLOCATION-----------------*/
 matrixA = new int*[M];
 matrixB = new int*[K];
 matrixC = new int*[M];

 for(i=0; i<M; i++)
 {
  matrixA[i] = new int[K];
  matrixC[i] = new int[N];
 }

 for(i=0; i<K; i++)
  matrixB[i] = new int[N];

 for(i=0; i<M; i++)
  for(j=0; j<K; j++)
  {
   matrix1 >> matrixA[i][j];
  }

 for(i=0; i<K; i++)
  for(j=0; j<N; j++)
  {
   matrix2 >> matrixB[i][j];
  }

 // allocate memory that corresponds to number of threads
 // needed to find every element of product matrix C
 t = new pthread_t[M*N];

 /*------------------------END MEMORY ALLOCATION--------------------*/

 /*------------------------BEGIN MULTI-THREADING--------------------*/
 // create thread for each element of matrixC needed to be calculated
 k=0;
 for(i=0; i<M; i++)
  for(j=0; j<N; j++, k++)
  {
   // create a data struct to store current i, j values in loop
   // so each thread knows what C(i,j) it's responsible for calculating
   struct pos* data = new pos;
   data->i = i;
   data->j = j;

   // pass in thread name and function/data it is to use
   pthread_create(&t[k], NULL, &getVal, data);
  }

 // wait for all threads to complete and obtain values calculated
 for(i=0; i<M*N; i++)
  pthread_join(t[i], NULL);

 /*------------------------END MULTI-THREADING------------------*/

 /*------------------------BEGIN OUTPUT DATA----------------------*/

 // output data calculated by threads to screen and file
 cout << endl << "Resulting Matrix Product" << endl << endl;
 fout << M << endl << N;
 cout << M << endl << N;
 for(i=0; i<M; i++)
 {
  fout << endl;
  cout << endl;
  for(j=0; j<N; j++)
  {
   fout << matrixC[i][j] << " ";
   cout << matrixC[i][j] << " ";
  }
 }
 cout << endl << endl << "Copy of data written to product.txt" << endl << endl;

 // close files
 matrix1.close();
 matrix2.close();
 fout.close();

 /*-------------------------END OUTPUT DATA---------------------------*/

 /*------------------------BEGIN MEMORY DEALLOCATION------------------*/
 // deallocate all memory
 for(i=0; i<M; i++)
 {
  delete [] matrixA[i];
  delete [] matrixC[i];
 }

 for(i=0; i<K; i++)
  delete [] matrixB[i];

 delete [] matrixA;
 delete [] matrixB;
 delete [] matrixC;
 delete [] t;

 // return success
 return 1;

 /*---------------------END PROGRAM/MEMORY DEALLOCATION-------------*/

} // main end

// function implementations

void* getVal(void* vdata)
{
 // need to create reference to struct pointer to allow usage
 pos data = *((struct pos*) vdata);

 // use formula to calculate product matrix value at C(i,j)
 for(int i=0; i<K; i++)
  matrixC[data.i][data.j] += matrixA[data.i][i] * matrixB[i][data.j];
}
