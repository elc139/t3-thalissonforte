#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <omp.h>

omp_lock_t lock;
int nThreads = 3, chunks = 4; // DEFAULT

class SharedArray {
private:
    char* array;
    int index;
    int size;
   bool usemutex;

public:
   SharedArray(int n, bool use) : size(n), index(0), usemutex(use) {
      array = new char[size];
      std::fill(array, array+size, '-');
   }
   ~SharedArray(){
      delete[] array;
   }
   void addChar(char c, int i) {
      
      
      if(!usemutex) i = index;
      else omp_set_lock(&lock);

      array[i] = c;
      spendSomeTime();
      index++;

      if(usemutex) omp_unset_lock(&lock);


      
   }
   int countOccurrences(char c) {
      return std::count(array, array+size, c);
   }
   std::string toString() {
      return std::string(array, size);
   }
private:
   void spendSomeTime() {
      for (int i = 0; i < 10000; i++) {
         for (int j = 0; j < 100; j++) {
            // These loops shouldn't be removed by the compiler
         }
      }
   }
};

class ArrayFiller {
private:
   static const int nTimes = 20;
   SharedArray* array;
public:
   ArrayFiller(bool usemutex) {
      array = new SharedArray(nThreads * nTimes, usemutex);
   }

   void fillArrayConcurrently(int sched) {
      std::thread t[nThreads];

      for (int i = 0; i < nThreads; ++i){
         t[i] = std::thread(&ArrayFiller::run, this, sched);
      }

      for (int i = 0; i < nThreads; ++i)
         t[i].join();
   }
   void printStats() {
      std::cout << array->toString() << std::endl;
      for (int i = 0; i < nThreads; ++i)
         std::cout << (char) ('A'+i) << "=" 
                   << array->countOccurrences('A'+i) << " ";
      std::cout << std::endl;
   }
   ~ArrayFiller() {
      delete array;
   }
private:
   void run(int sched) {
      int i;

      if(sched == 0){ //STATIC
         #pragma omp parallel for schedule(static) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 1){ //STATIC CHUNK
         #pragma omp parallel for schedule(static, chunks) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 2){ //DYNAMIC
         #pragma omp parallel for schedule(dynamic) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 3){ //DYNAMIC CHUNK
         #pragma omp parallel for schedule(dynamic, chunks) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 4){ //GUIDED
         #pragma omp parallel for schedule(guided) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 5){ //GUIDED CHUNK
         #pragma omp parallel for schedule(guided, chunks) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 6){ //RUNTIME
         #pragma omp parallel for schedule(runtime) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 7){ //AUTO
         #pragma omp parallel for schedule(auto) shared(array) private(i)
         for (i = 0; i < nTimes*nThreads; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }else
      if(sched == 8){ //SEM MUTEX
         #pragma omp parallel for
         for (i = 0; i < nTimes; i++) {
            array->addChar('A' + omp_get_thread_num(), i);
         }
      }
      
   }
};


int main() {

   // INIT NO MUTEX
   omp_init_lock(&lock);
   
   printf("Static\n"); 
   ArrayFiller m0(true);
   m0.fillArrayConcurrently(0);
   m0.printStats();
   printf("\n");

   printf("Static w/ %i chunks\n", chunks); 
   ArrayFiller m1(true);
   m1.fillArrayConcurrently(1);
   m1.printStats();
   printf("\n");

   printf("Dynamic\n"); 
   ArrayFiller m2(true);
   m2.fillArrayConcurrently(2);
   m2.printStats();
   printf("\n");

   printf("Dynamic w/ %i chunks\n", chunks); 
   ArrayFiller m3(true);
   m3.fillArrayConcurrently(3);
   m3.printStats();
   printf("\n");

   printf("Guided\n"); 
   ArrayFiller m4(true);
   m4.fillArrayConcurrently(4);
   m4.printStats();
   printf("\n");

   printf("Guided w/ %i chunks\n", chunks); 
   ArrayFiller m5(true);
   m5.fillArrayConcurrently(5);
   m5.printStats();
   printf("\n");

   printf("Runtime\n"); 
   ArrayFiller m6(true);
   m6.fillArrayConcurrently(6);
   m6.printStats();
   printf("\n");

   printf("Auto\n"); 
   ArrayFiller m7(true);
   m7.fillArrayConcurrently(7);
   m7.printStats();
   printf("\n");

   printf("Sem mutex\n"); 
   ArrayFiller m8(false);
   m8.fillArrayConcurrently(8);
   m8.printStats();
   printf("\n");

}
