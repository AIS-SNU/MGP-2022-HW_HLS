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
      const int* in1_0,// Read-Only Vector 1
      const int* in1_1,
      const int* in1_2,
      const int* in1_3,
      const int* in1_4,
      const int* in1_5,
      const int* in1_6,
      const int* in1_7,

      const int* in2_0,// Read-Only Vector 2
      const int* in2_1,
      const int* in2_2,
      const int* in2_3,
      const int* in2_4,
      const int* in2_5,
      const int* in2_6,
      const int* in2_7,

      int* out_0, // Output Result
      int* out_1,
      int* out_2,
      int* out_3,
      int* out_4,
      int* out_5,
      int* out_6,
      int* out_7,
      int* out_8,
      int* out_9,
	  int* out_10,
      int* out_11,
      int* out_12,
      int* out_13,
      int* out_14,
      int* out_15,
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
#pragma HLS INTERFACE m_axi port = in1_0 offset = slave bundle = gmem0  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_1 offset = slave bundle = gmem1  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_2 offset = slave bundle = gmem2  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_3 offset = slave bundle = gmem3  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_4 offset = slave bundle = gmem4  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_5 offset = slave bundle = gmem5  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_6 offset = slave bundle = gmem6  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_7 offset = slave bundle = gmem7  depth=DATA_SIZE/16 num_read_outstanding=32


#pragma HLS INTERFACE m_axi port = in2_0 offset = slave bundle = gmem8   depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_1 offset = slave bundle = gmem9   depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_2 offset = slave bundle = gmem10  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_3 offset = slave bundle = gmem11  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_4 offset = slave bundle = gmem12  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_5 offset = slave bundle = gmem13  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_6 offset = slave bundle = gmem14  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_7 offset = slave bundle = gmem15  depth=DATA_SIZE/16 num_read_outstanding=32

			  

#pragma HLS INTERFACE m_axi port = out_0 offset = slave bundle = gmem00  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_1 offset = slave bundle = gmem10  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_2 offset = slave bundle = gmem20  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_3 offset = slave bundle = gmem30  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_4 offset = slave bundle = gmem40  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_5 offset = slave bundle = gmem50  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_6 offset = slave bundle = gmem60  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_7 offset = slave bundle = gmem70  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_8 offset = slave bundle = gmem80  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_9 offset = slave bundle = gmem90  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_10 offset = slave bundle = gmem100  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_11 offset = slave bundle = gmem110  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_12 offset = slave bundle = gmem120  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_13 offset = slave bundle = gmem130  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_14 offset = slave bundle = gmem140  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_15 offset = slave bundle = gmem150  depth=DATA_SIZE/32 num_write_outstanding=32
			  

   int v1_buffer[8][DATA_SIZE];   //Local memory to store v1
   int v2_buffer[8][DATA_SIZE];   // Local memory to store v2
   int vout_buffer[16][DATA_SIZE]; // Local Memory to store result

   #pragma HLS array_partition variable=v1_buffer factor=16 type=cyclic dim=2
   #pragma HLS array_partition variable=v1_buffer factor=8 type=cyclic dim=1

   #pragma HLS array_partition variable=v2_buffer factor=16 type=cyclic dim=2
   #pragma HLS array_partition variable=v2_buffer factor=8 type=cyclic dim=1

   #pragma HLS array_partition variable=vout_buffer factor=16 type=cyclic dim=2
   #pragma HLS array_partition variable=v1_buffer factor=16 type=cyclic dim=1

   int n_size = size;
   name1: for (int i =0; i < n_size; i += DATA_SIZE*VEC_SIZE){
	    name2: for (int j = 0; j < DATA_SIZE ; j += VEC_SIZE) {
	       #pragma HLS PIPELINE II=1
		for (int k =0; k < VEC_SIZE; k++)
		{
			#pragma HLS UNROLL
		    v1_buffer[0][j + k] = in1_0[i + j + k];
			v1_buffer[1][j + k] = in1_1[i + j + k];
			v1_buffer[2][j + k] = in1_2[i + j + k];
			v1_buffer[3][j + k] = in1_3[i + j + k];
			v1_buffer[4][j + k] = in1_4[i + j + k];
			v1_buffer[5][j + k] = in1_5[i + j + k];
			v1_buffer[6][j + k] = in1_6[i + j + k];
			v1_buffer[7][j + k] = in1_7[i + j + k];
		}
	   }

		name3: for (int j = 0; j < DATA_SIZE ; j += VEC_SIZE) {
			#pragma HLS PIPELINE II=1
         for (int k =0; k < VEC_SIZE; k++)
         {
             #pragma HLS UNROLL
            v2_buffer[0][j + k] = in2_0[i + j + k];
            v2_buffer[1][j + k] = in2_1[i + j + k];
            v2_buffer[2][j + k] = in2_2[i + j + k];
            v2_buffer[3][j + k] = in2_3[i + j + k];
            v2_buffer[4][j + k] = in2_4[i + j + k];
            v2_buffer[5][j + k] = in2_5[i + j + k];
            v2_buffer[6][j + k] = in2_6[i + j + k];
            v2_buffer[7][j + k] = in2_7[i + j + k];

         }
        }

        name4: for (int j = 0; j < DATA_SIZE ; j += VEC_SIZE) {
            #pragma HLS PIPELINE II=1
         for (int k =0; k < VEC_SIZE; k++)
         {
             #pragma HLS UNROLL
             vout_buffer[0][j + k] = v1_buffer[0][j + k] + v2_buffer[0][j + k];
			 vout_buffer[1][j + k] = v1_buffer[1][j + k] + v2_buffer[1][j + k];
			 vout_buffer[2][j + k] = v1_buffer[2][j + k] + v2_buffer[2][j + k];
			 vout_buffer[3][j + k] = v1_buffer[3][j + k] + v2_buffer[3][j + k];
			 vout_buffer[4][j + k] = v1_buffer[4][j + k] + v2_buffer[4][j + k];
			 vout_buffer[5][j + k] = v1_buffer[5][j + k] + v2_buffer[5][j + k];
			 vout_buffer[6][j + k] = v1_buffer[6][j + k] + v2_buffer[6][j + k];
			 vout_buffer[7][j + k] = v1_buffer[7][j + k] + v2_buffer[7][j + k];
			 vout_buffer[8][j + k] = v1_buffer[0][j + k] - v2_buffer[0][j + k];
			 vout_buffer[9][j + k] = v1_buffer[1][j + k] - v2_buffer[1][j + k];
			 vout_buffer[10][j + k] = v1_buffer[2][j + k] - v2_buffer[2][j + k];
			 vout_buffer[11][j + k] = v1_buffer[3][j + k] - v2_buffer[3][j + k];
			 vout_buffer[12][j + k] = v1_buffer[4][j + k] - v2_buffer[4][j + k];
			 vout_buffer[13][j + k] = v1_buffer[5][j + k] - v2_buffer[5][j + k];
			 vout_buffer[14][j + k] = v1_buffer[6][j + k] - v2_buffer[6][j + k];
			 vout_buffer[15][j + k] = v1_buffer[7][j + k] - v2_buffer[7][j + k];
         }
        }

	    name6: for (int j = 0; j < DATA_SIZE ; j += VEC_SIZE) {
			#pragma HLS PIPELINE II=1
			for(int k=0; k< VEC_SIZE; k++)
			{
				#pragma HLS UNROLL
		       out_0[i+j+k] = vout_buffer[0][j+k];
		       out_1[i+j+k] = vout_buffer[1][j+k];
			   out_2[i+j+k] = vout_buffer[2][j+k];
		       out_3[i+j+k] = vout_buffer[3][j+k];
		       out_4[i+j+k] = vout_buffer[4][j+k];
		       out_5[i+j+k] = vout_buffer[5][j+k];
		       out_6[i+j+k] = vout_buffer[6][j+k];
		       out_7[i+j+k] = vout_buffer[7][j+k];
		       out_8[i+j+k] = vout_buffer[8][j+k];
			   out_9[i+j+k] = vout_buffer[9][j+k];
			   out_10[i+j+k] = vout_buffer[10][j+k];
			   out_11[i+j+k] = vout_buffer[11][j+k];
		       out_12[i+j+k] = vout_buffer[12][j+k];
			   out_13[i+j+k] = vout_buffer[13][j+k];
			   out_14[i+j+k] = vout_buffer[14][j+k];
			   out_15[i+j+k] = vout_buffer[15][j+k];

			}
		}  
	   }
 }
}
