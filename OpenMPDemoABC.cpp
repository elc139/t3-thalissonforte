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

   void fillArrayConcurrently() {
      std::thread t[nThreads];

      for (int i = 0; i < nThreads; ++i){
         t[i] = std::thread(&ArrayFiller::run, this, 'A'+i);
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
   void run(char c) {
      int i;
      #pragma omp parallel for schedule(auto) private(i)
      for (i = 0; i < nTimes*nThreads; i++) {
         array->addChar('A' + omp_get_thread_num(), i);
      }
   }
};


int main() {

   // INIT NO MUTEX
   omp_init_lock(&lock);
   printf("\n");
   ArrayFiller m1(true);
   m1.fillArrayConcurrently();
   m1.printStats();
   printf("\n");
}
