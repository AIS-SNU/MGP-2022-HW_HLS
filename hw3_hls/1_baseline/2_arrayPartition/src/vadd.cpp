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

#define BUFFER_SIZE 512

/*
    Vector Addition Kernel Implementation
    Arguments:
        in1   (input)     --> Input Vector1
        in2   (input)     --> Input Vector2
        out   (output)    --> Output Vector
        size  (input)     --> Size of Vector in Integer
   */
extern "C" {
void vadd(const int *in1, // Read-Only Vector 1
          const int *in2, // Read-Only Vector 2
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
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 depth=1024*1024*16/(512/8) num_read_outstanding=32 max_read_burst_length=16
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1 depth=1024*1024*16/(512/8) num_read_outstanding=32 max_read_burst_length=16
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2 depth=1024*1024*16/(512/8) num_write_outstanding=32 max_write_burst_length=16
//#pragma HLS ARRAY_PARTITION cyclic variable=in1 factor=8
//#pragma HLS ARRAY_PARTITION cyclic variable=in2 factor=8
//#pragma HLS ARRAY_PARTITION cyclic variable=out factor=8

  int v1_buffer[BUFFER_SIZE][BUFFER_SIZE];   //Local memory to store vector1
  int v2_buffer[BUFFER_SIZE][BUFFER_SIZE];   // Local memory to store vector2
  int vout_buffer[BUFFER_SIZE][BUFFER_SIZE]; // Local Memory to store result
//  // Per iteration of this loop perform BUFFER_SIZE vector addition
//  for (int i = 0; i < size; i += BUFFER_SIZE) {
//    int chunk_size = BUFFER_SIZE;
//    // boundary checks
//    if ((i + BUFFER_SIZE) > size)
//      chunk_size = size - i;

    // Transferring data in bursts hides the memory access latency as well as
    // improves bandwidth utilization and efficiency of the memory controller.
    // It is recommended to infer burst transfers from successive requests of
    // data from consecutive address locations.
    // A local memory vl_local is used for buffering the data from a single
    // burst. The entire input vector is read in multiple bursts.
    // The choice of LOCAL_MEM_SIZE depends on the specific applications and
    // available on-chip memory on target FPGA.
    // burst read of v1 and v2 vector from global memory
 
   for (int j = 0; j < BUFFER_SIZE ; j ++) {
	for (int i = 0; i< BUFFER_SIZE ; i +=8 ) {
//	    #pragma HLS pipeline ii=1
	    for (int ii = 0; ii< 8 ; ii ++ ) {
               v1_buffer[j][ii+i] = in1[j*BUFFER_SIZE + i+ii];
	    }
	}
    }
      for (int j = 0; j < BUFFER_SIZE ; j ++) {
        for (int i = 0; i< BUFFER_SIZE ; i +=8 ) {
//          #pragma HLS pipeline ii=1
            for (int ii = 0; ii< 8 ; ii ++ ) {
               v2_buffer[j][ii+i] = in2[j*BUFFER_SIZE + i+ii];
            }
        }
    }

//    for (int j = 0; j < BUFFER_SIZE ; j ++) {
//        for (int i = 0; i< BUFFER_SIZE ; i ++ ) {
//           v2_buffer[j][i] = in2[j*BUFFER_SIZE + i];
//        }
//    }

    for (int j = 0; j < BUFFER_SIZE ; j ++) {
        for (int i = 0; i< BUFFER_SIZE ; i ++ ) {
           vout_buffer[j][i] = 0;
        }
    }

#pragma HLS array_partition variable=v2_buffer dim=2 factor=16 cyclic
#pragma HLS array_partition variable=v2_buffer dim=1 factor=16 cyclic
#pragma HLS array_partition variable=v1_buffer dim=1 factor=16 cyclic
#pragma HLS array_partition variable=vout_buffer dim=1 factor=16 cyclic
#pragma HLS array_partition variable=vout_buffer dim=2 factor=16 cyclic

int vout_buffer_local[16][16]; 
 


  // PIPELINE pragma reduces the initiation interval for loop by allowing the
  // concurrent executions of operations
  vadd:
        // Perform matrix multiplication
    //OUTER_ROWS:
    for(int i = 0; i < BUFFER_SIZE; i+=16) {
        //OUTER_COLS:
        for(int p = 0; p < BUFFER_SIZE; p++) {
            //INNER_ROW_COLS:
            for(int j = 0; j < BUFFER_SIZE ; j+=16) {
            #pragma HLS pipeline ii = 1 rewind
                for(int ii=0; ii < 16; ii++){
                  for(int jj=0; jj < 16; jj++){
                     vout_buffer_local[ii][jj] =  v1_buffer[i+ii][p] * v2_buffer[p][j+jj];
                  }
                }
            
	   // #pragma HLS unroll
	        for(int ii=0; ii < 16; ii++){
                    for(int jj=0; jj < 16; jj++){
                        vout_buffer[i+ii][j+jj] += vout_buffer_local[ii][jj];
                    }
                }
	    }

        }
    }
    

    // burst write the result
//        for(int i = 0; i < BUFFER_SIZE; i++) {
        //MAT_C_COLS:
//        for(int j = 0; j < BUFFER_SIZE; j++) {
//        #pragma HLS pipeline ii = 1 rewind
//            out[i*BUFFER_SIZE+j] = vout_buffer[i][j];
//        }
//    }
      for (int j = 0; j < BUFFER_SIZE ; j ++) {
        for (int i = 0; i< BUFFER_SIZE ; i +=8 ) {
//          #pragma HLS pipeline ii=1
            for (int ii = 0; ii< 8 ; ii ++ ) {
               out[j*BUFFER_SIZE + i+ii] = vout_buffer[j][ii+i] ;
            }
        }
    }


  
}
}