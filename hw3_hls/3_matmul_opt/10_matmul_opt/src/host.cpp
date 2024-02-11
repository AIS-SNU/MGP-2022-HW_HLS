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

#define DATA_SIZE 16384
#define MATRIX_LEN 128
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
  auto bo0 = xrt::bo(device, vector_size_bytes, krnl.group_id(0));
  auto bo1 = xrt::bo(device, vector_size_bytes, krnl.group_id(1));
  auto bo_out = xrt::bo(device, vector_size_bytes, krnl.group_id(2));
// Map the contents of the buffer object into host memory
  auto bo0_map = bo0.map<int *>();
  auto bo1_map = bo1.map<int *>();
  auto bo_out_map = bo_out.map<int *>();
  std::fill(bo0_map, bo0_map + DATA_SIZE, 0);
  std::fill(bo1_map, bo1_map + DATA_SIZE, 0);
  std::fill(bo_out_map, bo_out_map + DATA_SIZE, 0);

  // Create the test data
  int bufReference[DATA_SIZE];
  for (int i = 0; i < DATA_SIZE; ++i) {
    bo0_map[i] = i;
    bo1_map[i] = i;
    bufReference[i] = 0;
  }

  auto cpu_begin = std::chrono::high_resolution_clock::now();

omp_set_num_threads(16);  
#pragma omp parallel for 
    for (int i = 0; i < MATRIX_LEN; i++){
     for (int k = 0; k < MATRIX_LEN; k++){
      for (int j = 0; j < MATRIX_LEN; j++){
        bufReference[i * MATRIX_LEN + j] += bo0_map[i * MATRIX_LEN + k] * bo1_map[k * MATRIX_LEN + j];
      }
     }
    }
  auto cpu_end = std::chrono::high_resolution_clock::now();

  auto fpga_begin = std::chrono::high_resolution_clock::now();

  // Synchronize buffer content with device side
  std::cout << "synchronize input buffer data to device global memory\n";


  //////////////////////////////////////////////////////////////////////////////
  auto host_to_fpga_start = std::chrono::high_resolution_clock::now();


  bo0.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  bo1.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  
  auto host_to_fpga_end = std::chrono::high_resolution_clock::now();
  /////////////////////////////////////////////////////////////////////////////



  auto fpga_cal_begin = std::chrono::high_resolution_clock::now();
  auto run = krnl(bo0, bo1, bo_out);
  run.wait();
  auto fpga_cal_end = std::chrono::high_resolution_clock::now();

  // Get the output;
  std::cout << "Get the output data from the device" << std::endl;
  
  //////////////////////////////////////////////////////////////////////////////
  auto fpga_to_host_start = std::chrono::high_resolution_clock::now();

  bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

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
//////////////
//

  for (int i = 0; i < 40; ++i){
std::cout << "module:  " << bo_out_map[i] << ", refer:  " << bufReference[i] << std::endl;
  }


//
////////////


    auto compare_begin = std::chrono::high_resolution_clock::now();

    // Validate our results
  if (std::memcmp(bo_out_map, bufReference, DATA_SIZE))
    throw std::runtime_error("Value read back does not match reference");
    auto compare_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> compare_duration = compare_end - compare_begin;
  std::cout << "Compare Time:                 " << compare_duration.count() << " s" << std::endl;



  std::cout << "TEST PASSED\n";

  return 0;
}
