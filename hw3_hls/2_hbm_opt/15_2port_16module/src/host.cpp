/**********
Copyright (c) 2020, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/
#include <cstring>
#include <iostream>
#include <chrono>
#include <math.h>
#include <omp.h>

// XRT includes
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"

#define DATA_SIZE 65536/16
#define NUM_KERNEL 4
#define NUM_TIMES 1024
//extern "C" void DUT_Set_ARG(int *dst[4], int *src, unsigned long long num_elements);
//extern "C" void DUT_Get_ARG(int *dst, int *src[4], unsigned long long num_elements);

//extern "C" void vadd_Set_in1(int **, int *, long);
//extern "C" void vadd_Get_in1(int *, int **, long);

//extern "C" void vadd_Set_in2(int **, int *, long);
//extern "C" void vadd_Get_in2(int *, int **, long);

//extern "C" void vadd_Set_out(int **, int *, long);
//extern "C" void vadd_Get_out(int *, int **, long);


int main(int argc, char **argv) {
  std::string xclbin_file_name = argv[1];

  // The host code assumes there is a single device and opening a device by
  // device index 0. If there are multiple devices then this device index needed
  // to be adjusted. The user can get the list of the devices and device indices
  // by xbtuil examine command.
  unsigned int device_index = 0;
  std::cout << "Open the device" << device_index << std::endl;
  auto device = xrt::device(device_index);
  
  std::cout << "Load the xclbin " << xclbin_file_name << std::endl;
  auto uuid = device.load_xclbin(xclbin_file_name);

  size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

  auto krnl = xrt::kernel(device, uuid, "vadd");



  std::cout << "Allocate Buffer in Global Memory\n";
//  auto bo0 = xrt::bo(device, vector_size_bytes, krnl.group_id(0));
//  auto bo1 = xrt::bo(device, vector_size_bytes, krnl.group_id(1));
//  auto bo_out = xrt::bo(device, vector_size_bytes, krnl.group_id(2));
//
//

int *host_bo0=new int[4*DATA_SIZE];
int *host_bo1=new int[4*DATA_SIZE];
int *host_bo_out=new int[4*DATA_SIZE];

//for (size_t i = 0; i < DATA_SIZE; ++i) {
   // host_bo0[i] = i;
   // host_bo1[i] = i;
   // host_bo_out[i] = 0;

//}

xrt::bo bo00;
xrt::bo bo01;
xrt::bo bo02;
xrt::bo bo03;

xrt::bo bo10;
xrt::bo bo11;
xrt::bo bo12;
xrt::bo bo13;

xrt::bo bo_out0;
xrt::bo bo_out1;
xrt::bo bo_out2;
xrt::bo bo_out3;


int* bo00_map;
int* bo01_map;
int* bo02_map;
int* bo03_map;

int* bo10_map;
int* bo11_map;
int* bo12_map;
int* bo13_map;

int* bo_out_map0;
int* bo_out_map1;
int* bo_out_map2;
int* bo_out_map3;


std::cout << "finaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaish bo allocate\n";

//for (int i = 0; i < 4; i++) {
    bo00 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(0));
    bo00_map = bo00.template map<int*>();
    bo10 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(1));
    bo10_map = bo10.template map<int*>();
    
    bo01 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(2));
    bo01_map = bo01.template map<int*>();
    bo11 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(3));
    bo11_map = bo11.template map<int*>();
    
    bo02 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(4));
    bo02_map = bo02.template map<int*>();
    bo12 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(5));
    bo12_map = bo12.template map<int*>();

    bo03 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(6));
    bo03_map = bo03.template map<int*>();
    bo13 = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(7));
    bo13_map = bo13.template map<int*>();


//}

std::cout << "finaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaish bo allocate\n";


//for (int ii = 0; ii < 4; ii++) {
   
    bo_out0= xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(8));
    bo_out_map0 = bo_out0.template map<int*>();
    bo_out1= xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(9));
    bo_out_map1 = bo_out1.template map<int*>();
    bo_out2= xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(10));
    bo_out_map2 = bo_out2.template map<int*>();
    bo_out3= xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(11));
    bo_out_map3 = bo_out3.template map<int*>();

//}

// Map the contents of the buffer object into host memory
//  auto bo0_map = bo0.map<int *>();
//  auto bo1_map = bo1.map<int *>();
//  auto bo_out_map = bo_out.map<int *>();


std::cout << "finish bo allocate\n";
////////////////////////////////////////////////////////////////////
//

//for (int i = 0; i < 4; ++i) {	
//  std::fill(bo0_map[i], bo0_map[i] + DATA_SIZE, 0);
//  std::fill(bo1_map[i], bo1_map[i] + DATA_SIZE, 0);
//}i
//for (int i = 0; i < 4; ++i) {
//  std::fill(bo_out_map[i], bo_out_map[i] + DATA_SIZE, 0);
//}

  // Create the test data
  int bufReference[DATA_SIZE];
  for (int i = 0; i < DATA_SIZE; ++i) {
   // bo0_map[i] = i;
   // bo1_map[i] = i;
    bufReference[i] = 0;
  }
//for (int i = 0; i < 4; ++i) {
    for(int j=0; j<DATA_SIZE ; j++){
      //bo0_map[i][j] = j;
      //bo1_map[i][j] = j;
      bo00_map[j] =j;
      bo10_map[j] =j;
      bo01_map[j] =j;
      bo11_map[j] =j;
      bo02_map[j] =j;
      bo12_map[j] =j;
      bo03_map[j] =j;
      bo13_map[j] =j;

    }

//}
//for(int i = 0; i < 4; ++i) {
    for(int j=0; j<DATA_SIZE ; j++){
        host_bo0[0*DATA_SIZE + j]= bo00_map[j];
	host_bo0[1*DATA_SIZE + j]= bo01_map[j];
	host_bo0[2*DATA_SIZE + j]= bo02_map[j];
	host_bo0[3*DATA_SIZE + j]= bo03_map[j];

    }
//}
//for (int i = 0; i < 4; ++i) {
    for(int j=0; j<DATA_SIZE ; j++){
        host_bo1[0*DATA_SIZE + j]= bo10_map[j];
	host_bo1[1*DATA_SIZE + j]= bo11_map[j];
	host_bo1[2*DATA_SIZE + j]= bo12_map[j];
	host_bo1[3*DATA_SIZE + j]= bo13_map[j];

    }
//}

 std::cout << "finish allocate\n";
////////////////////////////////////////////////////////////////////
 auto cpu_begin = std::chrono::high_resolution_clock::now();

omp_set_num_threads(16);

#pragma omp parallel for
    for (int i = 0; i < DATA_SIZE*4; i ++ ){	
       bufReference[i  ] = host_bo0[i]+host_bo1[i];
    }

  auto cpu_end = std::chrono::high_resolution_clock::now();
  std::cout << "cpu cal finish\n";




  auto fpga_begin = std::chrono::high_resolution_clock::now();

  // Synchronize buffer content with device side
  std::cout << "synchronize input buffer data to device global memory\n";


  //////////////////////////////////////////////////////////////////////////////
  auto host_to_fpga_start = std::chrono::high_resolution_clock::now();

//  for (int i = 0; i < 4; i++) { 
    // sync updated bo contents to board
    bo00.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo10.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo01.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo11.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo02.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo12.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo03.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo13.sync(XCL_BO_SYNC_BO_TO_DEVICE);

 // }
  
  auto host_to_fpga_end = std::chrono::high_resolution_clock::now();
  /////////////////////////////////////////////////////////////////////////////
  std::cout << "synchronize finish\n";
  std::cout << "set argument\n";
   auto fpga_cal_begin = std::chrono::high_resolution_clock::now();
  //for (int i = 0; i < NUM_KERNEL; i++) {
        auto run1 = krnl(bo00,bo10,bo_out0,NUM_TIMES);
        auto run2 = krnl(bo01,bo11,bo_out1,NUM_TIMES);
	auto run3 = krnl(bo02,bo12,bo_out2,NUM_TIMES);
	auto run4 = krnl(bo03,bo13,bo_out3,NUM_TIMES);

         run1.wait();
         run2.wait();
	 run3.wait();
	 run4.wait();

  //}
  //run.wait();

  auto fpga_cal_end = std::chrono::high_resolution_clock::now();

  // Get the output;
  std::cout << "Get the output data from the device" << std::endl;
  
  //////////////////////////////////////////////////////////////////////////////
  auto fpga_to_host_start = std::chrono::high_resolution_clock::now();
//  for (int i = 0; i < 4; i++) {
    bo_out0.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    bo_out1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    bo_out2.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    bo_out3.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
//  }
  auto fpga_to_host_end = std::chrono::high_resolution_clock::now();
  /////////////////////////////////////////////////////////////////////////////


  auto fpga_end = std::chrono::high_resolution_clock::now();

  
  std::chrono::duration<double> fpga_cal_duration = fpga_cal_end - fpga_cal_begin;
  std::cout << "FPGA IN Time:                 " << fpga_cal_duration.count() << " s" << std::endl;

  std::chrono::duration<double> host_to_fpga_duration = host_to_fpga_end -host_to_fpga_start;
  std::cout << "host to fpga PCIe Time:       " << host_to_fpga_duration.count() << " s" << std::endl;

  std::chrono::duration<double> fpga_to_host_duration = fpga_to_host_end -fpga_to_host_start;
  std::cout << "fpga to host PCIe Time:       " << fpga_to_host_duration.count() << " s" << std::endl;
  
  std::chrono::duration<double> fpga_duration = fpga_end - fpga_begin;
  std::cout << "FPGA Time:                    " << fpga_duration.count() << " s" << std::endl;

  std::chrono::duration<double> cpu_duration = cpu_end - cpu_begin;
  std::cout << "CPU Time:                     " << cpu_duration.count() << " s" << std::endl;

  std::cout << "FPGA Speedup:                 " << cpu_duration.count() / fpga_duration.count() << " x" << std::endl;



    auto compare_begin = std::chrono::high_resolution_clock::now();

//for (int i = 0; i < 4; ++i) {
    for(int j=0; j<DATA_SIZE ; j++){
        host_bo_out[0*DATA_SIZE+j]= bo_out_map0[j];
    }
//}

for (int i = DATA_SIZE-100; i < DATA_SIZE; ++i) {
    std::cout << "module:  " << host_bo_out[i] << ", refer:  " << bufReference[i] << std::endl;
}

   // vadd_Get_out(host_bo_out, bo_out_map ,DATA_SIZE);
    // Validate our results
  if (std::memcmp(host_bo_out, bufReference, DATA_SIZE))
    throw std::runtime_error("Value read back does not match reference");
    auto compare_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> compare_duration = compare_end - compare_begin;
  std::cout << "Compare Time:                 " << compare_duration.count() << " s" << std::endl;



  std::cout << "TEST PASSED\n";

  return 0;
}
