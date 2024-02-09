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

/*******************************************************************************
Description:
    HLS pragmas can be used to optimize the design : improve throughput, reduce
latency and
    device resource utilization of the resulting RTL code
    This is vector addition example to demonstrate how HLS optimizations are
used in kernel.
*******************************************************************************/
//#include <hls_stream.h>
#define BUFFER_SIZE 400000
#define DATA_SIZE 400000
/*
    Vector Addition Kernel Implementation
    Arguments:
        in1   (input)     --> Input Vector1
        in2   (input)     --> Input Vector2
        out   (output)    --> Output Vector
        size  (input)     --> Size of Vector in Integer
   */
extern "C" {
void vadd(const int in1[BUFFER_SIZE], // Read-Only Vector 1
          const int in2[BUFFER_SIZE], // Read-Only Vector 2
          int out[BUFFER_SIZE],       // Output Result
          int size                 // Size in integer
          ) {
// SDAccel kernel must have one and only one s_axilite interface which will be
// used by host application to configure the kernel.
// Here bundle control is defined which is s_axilite interface and associated
// with all the arguments (in1, in2, out and size),
// control interface must also be associated with "return".
// All the global memory access arguments must be associated to one m_axi(AXI
// Master Interface). Here all three arguments(in1, in2, out) are
// associated to bundle gmem which means that a AXI master interface named
// "gmem" will be created in Kernel and all these variables will be
// accessing global memory through this interface.
// Multiple interfaces can also be created based on the requirements. For
// example when multiple memory accessing arguments need access to
// global memory simultaneously, user can create multiple master interfaces and
// can connect to different arguments.
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 //max_read_burst_length = 256
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1 //max_read_burst_length = 256
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2 //max_write_burst_length = 256

  int v1_buffer[DATA_SIZE];   //Local memory to store vector1
  int v2_buffer[DATA_SIZE];   // Local memory to store vector2
  int vout_buffer[DATA_SIZE]; // Local Memory to store result



   for (int j = 0; j < DATA_SIZE ; j += 1) {
       v1_buffer[j  ] = in1[j];
   }

   for (int j = 0; j < DATA_SIZE ; j += 1) {
       v2_buffer[j  ] = in2[j];
   }

   for (int j = 0; j < DATA_SIZE ; j +=1 ) {
       vout_buffer[j] = v1_buffer[j]+v2_buffer[j];
   }

   for (int j = 0; j < DATA_SIZE ; j += 1) {
       out[j] = vout_buffer[j  ];
   }  
}
}
