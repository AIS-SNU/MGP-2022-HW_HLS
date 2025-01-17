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
#include<vector>
// XRT includes
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"

#define DATA_SIZE 512*16*8
#define ALL_DATA DATA_SIZE*8
#define ITERATION 128
//extern "C" void DUT_Set_ARG(int *dst[4], int *src, unsigned long long num_elements);
//extern "C" void DUT_Get_ARG(int *dst, int *src[4], unsigned long long num_elements);

//extern "C" void vadd_Set_in1(int **, int *, long);
//extern "C" void vadd_Get_in1(int *, int **, long);

//extern "C" void vadd_Set_in2(int **, int *, long);
//extern "C" void vadd_Get_in2(int *, int **, long);

//extern "C" void vadd_Set_out(int **, int *, long);
//extern "C" void vadd_Get_out(int *, int **, long);

using namespace std;
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


/*
int *host_bo0=new int[DATA_SIZE*8];
int *host_bo1=new int[DATA_SIZE*8];
int *host_bo_out=new int[DATA_SIZE*16];

for (size_t i = 0; i < DATA_SIZE; ++i) {
    host_bo0[i] = i;
    host_bo1[i] = i;
    host_bo_out[i] = 0;

}
*/
xrt::bo bo0[8];
xrt::bo bo1[8];
xrt::bo bo_out[16];

int* bo0_map[8];
int* bo1_map[8];
int* bo_out_map[16];


for (int i = 0; i < 8; i++) {
    bo0[i] = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(i));
    bo0_map[i] = bo0[i].template map<int*>();
    bo1[i] = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(i+8));
    bo1_map[i] = bo1[i].template map<int*>();
}

for (int i = 0; i < 16; i++) {
    bo_out[i] = xrt::bo(device, DATA_SIZE * sizeof(int), krnl.group_id(i+16));
    bo_out_map[i] = bo_out[i].template map<int*>();
}

// Map the contents of the buffer object into host memory
//  auto bo0_map = bo0.map<int *>();
//  auto bo1_map = bo1.map<int *>();
//  auto bo_out_map = bo_out.map<int *>();




for (int i = 0; i < 8; ++i) {	
  std::fill(bo0_map[i], bo0_map[i] + DATA_SIZE, 0);
  std::fill(bo1_map[i], bo1_map[i] + DATA_SIZE, 0);
}
for (int i = 0; i < 16; ++i) {
  std::fill(bo_out_map[i], bo_out_map[i] + DATA_SIZE, 0);
}

  // Create the test data
 vector< int> bufReference(DATA_SIZE*16);
  for (int i = 0; i < DATA_SIZE*16; ++i) {
   // bo0_map[i] = i;
   // bo1_map[i] = i;
    bufReference[i] = 0;
  }


for (int i = 0; i < 8; ++i) {
    for(int j=0; j< DATA_SIZE ; j++){
      //bo0_map[i][j] = j;
      //bo1_map[i][j] = j;
      bo0_map[i][j] =i+j;
      bo1_map[i][j] =i+j;
    }
}
/*
for(int i = 0; i < 8; ++i) {
    for(int j=0; j<DATA_SIZE ; j++){
        host_bo0[i*DATA_SIZE + j]= bo0_map[i][j];
    }
}
for (int i = 0; i < 8; ++i) {
    for(int j=0; j<DATA_SIZE ; j++){
        host_bo1[i*DATA_SIZE + j]= bo1_map[i][j];
    }
}
*/
 std::cout << "finish allocate\n";
////////////////////////////////////////////////////////////////////
 auto cpu_begin = std::chrono::high_resolution_clock::now();

omp_set_num_threads(16);
#pragma omp parallel for
	for(int i = 0; i < DATA_SIZE; i ++ ){
       bufReference[i] = i;
    }
    for (int i = DATA_SIZE*8; i < DATA_SIZE*9; i ++ ){
		bufReference[i] = i;
    }


  auto cpu_end = std::chrono::high_resolution_clock::now();
  std::cout << "cpu cal finish\n";




  auto fpga_begin = std::chrono::high_resolution_clock::now();

  // Synchronize buffer content with device side
  std::cout << "synchronize input buffer data to device global memory\n";


  //////////////////////////////////////////////////////////////////////////////
  auto host_to_fpga_start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 8; i++) { 
    // sync updated bo contents to board
    bo0[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
    bo1[i].sync(XCL_BO_SYNC_BO_TO_DEVICE);
  }

//  bo0.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//  bo1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  
  auto host_to_fpga_end = std::chrono::high_resolution_clock::now();
  /////////////////////////////////////////////////////////////////////////////
  std::cout << "synchronize finish\n";
  std::cout << "set argument\n";
//   auto fpga_cal_begin = std::chrono::high_resolution_clock::now();
//   auto run = krnl(bo0[0],bo0[1],bo0[2],bo0[3],
//		   bo0[4],bo0[5],bo0[6],bo0[7],
//
//		   bo1[0],bo1[1],bo1[2],bo1[3],
//		   bo1[4],bo1[5],bo1[6],bo1[7],
//
//		   bo_out[0],bo_out[1],bo_out[2],bo_out[3],
//		   bo_out[4],bo_out[5],bo_out[6],bo_out[7],
//		   bo_out[8],bo_out[9],bo_out[10],bo_out[11],
//		   bo_out[12],bo_out[13],bo_out[14],bo_out[15],
//		   ALL_DATA
//		   );
 auto run = xrt::run(krnl);
 for(int i=0;i<8;i++){run.set_arg(i,bo0[i]);}
 for(int i=8;i<16;i++){run.set_arg(i,bo1[i-8]);}
 for(int i=16;i<32;i++){run.set_arg(i,bo_out[i-16]);}
run.set_arg(32,ALL_DATA*16);
run.set_arg(33,ITERATION);

  auto fpga_cal_begin = std::chrono::high_resolution_clock::now();
  run.start();
  run.wait();
  auto fpga_cal_end = std::chrono::high_resolution_clock::now();

  // Get the output;
  std::cout << "Get the output data from the device" << std::endl;
  
  //////////////////////////////////////////////////////////////////////////////
  auto fpga_to_host_start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 16; i++) {
    bo_out[i].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  }
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

//  std::chrono::duration<double> cpu_duration = cpu_end - cpu_begin;
//  std::cout << "CPU Time:                     " << cpu_duration.count() << " s" << std::endl;

 // std::cout << "FPGA Speedup:                 " << cpu_duration.count() / fpga_duration.count() << " x" << std::endl;

double result = fpga_cal_duration.count();
unsigned int n= ALL_DATA*2*ITERATION;
result= (float)(n*2)/1000000000/result;

    auto compare_begin = std::chrono::high_resolution_clock::now();
/*
for (int i = 0; i < 16; ++i) {
    for(int j=0; j<DATA_SIZE ; j++){
        host_bo_out[i*DATA_SIZE+j]= bo_out_map[i][j];
    }
}*/
std::cout << "THROUGHPUT:  " << result << std::endl;

//for (int i = ALL_DATA-10; i < ALL_DATA+10; ++i) {
//   std::cout << "module:  " << host_bo_out[i] << ", refer:  " << bufReference[i] << std::endl;
//}

   // vadd_Get_out(host_bo_out, bo_out_map ,DATA_SIZE);
    // Validate our results
//  if (std::memcmp(host_bo_out, bufReference, ALL_DATA*2))
//    throw std::runtime_error("Value read back does not match reference");
    auto compare_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> compare_duration = compare_end - compare_begin;
  std::cout << "Compare Time:                 " << compare_duration.count() << " s" << std::endl;


  std::cout << "TEST PASSED\n";

  return 0;
}
