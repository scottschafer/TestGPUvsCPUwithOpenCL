#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <time.h>

// set up your include path for EasyCL
#include "EasyCL.h"
#include "CLKernel.h"
#include "CLKernel_structs.h"


/***
 * Run a test algorithm both on the GPU and on the CPU, see which is faster. Requires https://github.com/hughperkins/EasyCL.
 */

// include the C++ ified gpuOrganism kernel code
#include "gpuOrganism.h"

using namespace std;
using namespace easycl;


// Include the gpuOrganism.cl code as a string constant;
#define STRINGIFY(TOSTRINGIFY) #TOSTRINGIFY

const char * gpuOrganismKernel =
#include "gpuOrganism/gpuOrganism.cl"
;

void setupTestData(std::vector<gpuOrganism> &list, int numOrganisms) {
  list.clear();
  for (int x = 0; x < numOrganisms; x++) {
    gpuOrganism organism;
    organism.numCells = 200;
    
    for (int y = 0; y < organism.numCells; y++) {
      organism.cells[y].pos.x = rand() % 100;
      organism.cells[y].pos.y = rand() % 100;
      organism.cells[y].velocity.x = rand() % 100;
      organism.cells[y].velocity.y = rand() % 100;
      
      organism.cells[y].expectedPos.x = organism.cells[y].pos.x + organism.cells[y].velocity.x;
      organism.cells[y].expectedPos.y = organism.cells[y].pos.y + organism.cells[y].velocity.y;
    }
    list.push_back(organism);
  }
}

void verifyTestData(std::vector<gpuOrganism> &list) {
  
  for (std::vector<gpuOrganism>::iterator i = list.begin(); i != list.end(); i++) {
    gpuOrganism organism = *i;
    
    for (int y = 0; y < organism.numCells; y++) {
      if (organism.cells[y].expectedPos.x != organism.cells[y].pos.x ||
          organism.cells[y].expectedPos.y != organism.cells[y].pos.y) {
        throw "ERROR";
      }
    }
  }
}

int main() {
  if(!EasyCL::isOpenCLAvailable()) {
    cout << "opencl library not found" << endl;
    exit(-1);
  }
  cout << "found opencl library" << endl;
  
  EasyCL *cl = EasyCL::createForFirstGpuOtherwiseCpu();
  CLKernel *kernel = cl->buildKernelFromString(gpuOrganismKernel, "processPhysics", "");
  
  // Test data
  int numOrganisms = 10000;
  std::vector<gpuOrganism> list;
  clock_t GPU_TIME, CPU_TIME, t;
  
  
  // RUN GPU TEST
  setupTestData(list, numOrganisms);
  cout << endl << "Beginning test on GPU" << endl;
  t = clock();
  int global_worksize = numOrganisms;
  int local_worksize = 100;
  kernel->inout(list.size(), list.data());
  kernel->run_1d(global_worksize, local_worksize);
  cl->finish();
  t = clock() - t;
  GPU_TIME = t;
  cout << "GPU took " << t << " clicks (" << (((float)t)/CLOCKS_PER_SEC) << " seconds)." << endl;
  verifyTestData(list); // Verify that it worked

  // RUN CPU TEST, in pure C++
  setupTestData(list, numOrganisms);
  cout << endl << "Beginning test on C++" << endl;
  t = clock();
  for (vector<gpuOrganism>::iterator i = list.begin(); i != list.end(); i++) {
    processPhysics(&*i);
  }
  t = clock() - t;
  CPU_TIME = t;
  
  cout << "CPU took " << t << " clicks (" << (((float)t)/CLOCKS_PER_SEC) << " seconds)." << endl;
  verifyTestData(list);
  
  cout << endl << endl << "GPU speedup = " << (float(CPU_TIME) / float(GPU_TIME)) << endl << endl;

}