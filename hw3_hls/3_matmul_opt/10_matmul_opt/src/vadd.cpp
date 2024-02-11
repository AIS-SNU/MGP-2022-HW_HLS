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
//typedef int<512> int16;
#include <iostream>

#define BUFFER_SIZE 128
#define TILE_SIZE 64
#define rows BUFFER_SIZE
#define inners BUFFER_SIZE
#define columns BUFFER_SIZE

/*
    Vector Addition Kernel Implementation
    Arguments:
        in1   (input)     --> Input Vector1
        in2   (input)     --> Input Vector2
        out   (output)    --> Output Vector
        size  (input)     --> Size of Vector in Integer
*/


/*
void matmulImplRowColParallelInnerTiling( int *in1,int *in2,int *result) {
//#pragma HLS array_partition variable=result factor=64 cyclic
int left[TILE_SIZE][TILE_SIZE];   //Local memory to store vector1
int right[TILE_SIZE][TILE_SIZE];   // Local memory to store vector2
#pragma HLS array_partition variable=left dim=2 factor=64 cyclic
#pragma HLS array_partition variable=right dim=1 factor=64 cyclic
int rows =  BUFFER_SIZE;
int columns =  BUFFER_SIZE;
int inners =  BUFFER_SIZE;
int tileSize = 64;


  for (int rowTile = 0; rowTile < rows; rowTile += TILE_SIZE) {
    for (int columnTile = 0; columnTile < columns; columnTile += TILE_SIZE) {
      for (int innerTile = 0; innerTile < inners; innerTile += tileSize) {
	//data import
        for (int row = rowTile; row < rowTile + TILE_SIZE; row++) {
          int innerTileEnd = std::min(inners, innerTile + tileSize);
          for (int inner = innerTile; inner < innerTileEnd; inner++) {
            for (int col = columnTile; col < columnTile + TILE_SIZE; col++) {
		left[row-rowTile][inner-innerTile]= in1[row * inners + inner];
		right[inner-innerTile][col-columnTile]= in2[inner * columns + col];	
	    }
	  }
       	}
	//calculate
	for (int row = rowTile; row < rowTile + TILE_SIZE; row++) {
          int innerTileEnd = std::min(inners, innerTile + tileSize);
          for (int inner = innerTile; inner < innerTileEnd; inner++) {
            for (int col = columnTile; col < columnTile + TILE_SIZE; col++) {
		    #pragma HLS unroll
                 result[row * columns + col] += left[row-rowTile][inner-innerTile] * right[inner-innerTile][col-columnTile];
      	    }
          }
        }

      }
    }
  } 
}




*/





extern "C" {
void vadd(const int in1[BUFFER_SIZE*BUFFER_SIZE], // Read-Only Vector 1
          const int in2[BUFFER_SIZE*BUFFER_SIZE], // Read-Only Vector 2
          int out[BUFFER_SIZE*BUFFER_SIZE]       // Output Result
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
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 depth=4096
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1 depth=4096
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2 depth=4096

#pragma HLS interface s_axilite port=return


  int vout_buffer[BUFFER_SIZE*BUFFER_SIZE]; // Local Memory to store result
#pragma HLS array_partition variable=vout_buffer factor=64 cyclic

for (int i = 0; i < BUFFER_SIZE ; i ++) {
  for (int j = 0; j < BUFFER_SIZE ; j ++) {

      vout_buffer[i*BUFFER_SIZE+j] = 0;
  }
}
   
//matmulImplRowColParallelInnerTiling(in1,in2,vout_buffer);
//#pragma HLS array_partition variable=result factor=64 cyclic
int left[TILE_SIZE][TILE_SIZE];   //Local memory to store vector1
int right[TILE_SIZE][TILE_SIZE];   // Local memory to store vector2
#pragma HLS array_partition variable=left dim=2 factor=64 cyclic
#pragma HLS array_partition variable=right dim=1 factor=64 cyclic
//int rows =  BUFFER_SIZE;
//int columns =  BUFFER_SIZE;
//int inners =  BUFFER_SIZE;
int tileSize = 64;


  for (int rowTile = 0; rowTile < rows; rowTile += TILE_SIZE) {
    for (int columnTile = 0; columnTile < columns; columnTile += TILE_SIZE) {
      for (int innerTile = 0; innerTile < inners; innerTile += tileSize) {
        //data import
        for (int row = rowTile; row < rowTile + TILE_SIZE; row++) {
          int innerTileEnd = std::min(inners, innerTile + tileSize);
          for (int inner = innerTile; inner < innerTileEnd; inner++) {
            for (int col = columnTile; col < columnTile + TILE_SIZE; col++) {
                left[row-rowTile][inner-innerTile]= in1[row * inners + inner];
                right[inner-innerTile][col-columnTile]= in2[inner * columns + col];
            }
          }
        }
        //calculate
        for (int row = rowTile; row < rowTile + TILE_SIZE; row++) {
          int innerTileEnd = std::min(inners, innerTile + tileSize);
          for (int inner = innerTile; inner < innerTileEnd; inner++) {
            for (int col = columnTile; col < columnTile + TILE_SIZE; col++) {
                    #pragma HLS unroll
                 vout_buffer[row * columns + col] += left[row-rowTile][inner-innerTile] * right[inner-innerTile][col-columnTile];
            }
          }
        }

      }
    }
  }


    for(int j = 0; j< BUFFER_SIZE ; j ++ ){
        for (int i = 0; i< BUFFER_SIZE ; i ++ ) {
               out[j*BUFFER_SIZE+i] = vout_buffer[j*BUFFER_SIZE+i] ;
        }
    }


  
}
}
