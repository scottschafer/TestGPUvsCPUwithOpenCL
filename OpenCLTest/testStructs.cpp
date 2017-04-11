/*
//
//  testStructs.cpp
//  OpenCLTest
//
//  Created by Scott Schafer on 4/9/17.
//  Copyright © 2017 Scott Schafer. All rights reserved.
//


#define kernel
#define global
#define get_global_id(X) X

//#include "testStructs.cl"

typedef struct MyStruct {
  int anint;
  float afloat;
  int threeints[3];
} MyStruct;

kernel void testStructs(global int *ints, global float *floats, global MyStruct *structs) {
  if(get_global_id(0) != 0) {
    return;
  }
  floats[0] = structs[0].afloat;
  ints[0] = structs[0].anint;
  structs[0].threeints[0] = structs[0].threeints[1] + structs[0].threeints[2];
}

*/