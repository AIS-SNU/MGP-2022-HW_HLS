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

#define DATA_SIZE 32*16

#define VEC_SIZE 16

/*
    Vector Addition Kernel Implementation
    Arguments:
        in1   (input)     --> Input Vector1
        in2   (input)     --> Input Vector2
        out   (output)    --> Output Vector
        size  (input)     --> Size of Vector in Integer
   */
extern "C" {
void vadd(
	  const int* in1, // Read-Only Vector 1
          const int* in2, // Read-Only Vector 2
          int *out,       // Output Result
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
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 max_read_burst_length = 256
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1 max_read_burst_length = 256
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2 max_write_burst_length = 256

#pragma HLS interface s_axilite port=return

   int v1_buffer[DATA_SIZE];   //Local memory to store v1
   int v2_buffer[DATA_SIZE];   // Local memory to store v2
//   int vout_buffer[DATA_SIZE]; // Local Memory to store result

   #pragma HLS array_partition variable=v1_buffer factor=16 type=cyclic
   #pragma HLS array_partition variable=v2_buffer factor=16 type=cyclic
//   #pragma HLS array_partition variable=vout_buffer factor=16 type=cyclic

//   #pragma HLS array_partition variable=v1_buffer complete
//   #pragma HLS array_partition variable=v1_buffer complete

   int n_size = size;
   name1: for (int i =0; i < n_size; i += DATA_SIZE){
//		#pragma HLS PIPELINE II=1	  
	    name2: for (int j = 0; j < DATA_SIZE ; j += VEC_SIZE) {
	       #pragma HLS PIPELINE II=1
		for (int k =0; k < VEC_SIZE; k++)
		{
			#pragma HLS UNROLL
		    v1_buffer[j + k] = in1[i + j + k];
			v2_buffer[j + k] = in2[i + j + k];
			out[i+j+k] =  v1_buffer[j + k] + v2_buffer[j + k];
		}
	   }
//        name3: for (int j = 0; j < DATA_SIZE ; j += VEC_SIZE) {
//            #pragma HLS PIPELINE II=1
//         for (int k =0; k < VEC_SIZE; k++)
//         {
//             #pragma HLS UNROLL
//             vout_buffer[j + k] = v1_buffer[j + k] + v2_buffer[j + k];
//         }
//        }

//	    name4: for (int j = 0; j < DATA_SIZE ; j += VEC_SIZE) {
//			#pragma HLS PIPELINE II=1
//			for(int k=0; k< VEC_SIZE; k++)
//			{
//				#pragma HLS UNROLL
//				//out[i+j+k] = vout_buffer[j+k ];
//				out[i+j+k] =  v1_buffer[j + k] + v2_buffer[j + k];
//			}
//		}  
	   }
 }
}
