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
#include <iostream>

#define TILE_SIZE 256
#define multi_thread 2
extern "C" {
void vadd(const int *in1,  // Read-Only Vector 1
          const int *in2,  // Read-Only Vector 2
          int *out,        // Output Result
		  int size		   // matrix len
          ) {

#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2

//setting constant
int BUFFER_SIZE = size;
int rows = BUFFER_SIZE;
int inners = BUFFER_SIZE;
int columns = BUFFER_SIZE;

int vout_buffer[multi_thread*TILE_SIZE][multi_thread*TILE_SIZE];
#pragma HLS array_partition variable=vout_buffer dim=2 factor=16 type= cyclic
#pragma HLS array_partition variable=vout_buffer dim=1 factor=16 type= cyclic
//burst write
//init vout_buffer
   
int left[multi_thread*TILE_SIZE][TILE_SIZE];
int right[TILE_SIZE][TILE_SIZE*multi_thread];
#pragma HLS array_partition variable=left dim=1 factor=16 type= cyclic
#pragma HLS array_partition variable=left dim=2 factor=16 type= cyclic
#pragma HLS array_partition variable=right dim=2 factor=16 type= cyclic


  for (int rowTile = 0; rowTile < rows; rowTile += multi_thread*TILE_SIZE) {
    for (int columnTile = 0; columnTile < columns; columnTile += multi_thread*TILE_SIZE) {

/////////////////////////////////reset vout
	 for (int i = 0; i < multi_thread* TILE_SIZE ; i ++) {
      for (int j = 0; j < multi_thread* TILE_SIZE ; j+=16) {
      #pragma HLS pipeline II=1
        for (int k = 0; k < 16 ; k ++) {
        #pragma HLS unroll
            vout_buffer[i][j+k] = 0;
        }
      }
     }

/////////////////////////////////one tile start/////////////////////////////////////////////////////
      for (int innerTile = 0; innerTile < inners; innerTile += TILE_SIZE) {
//  	#pragma HLS pipeline II=1
		int innerTileEnd = std::min(inners, innerTile + TILE_SIZE);
        int rowTileEnd = std::min(rows, rowTile + multi_thread*TILE_SIZE);
		int columnTileEnd = std::min(columns, columnTile + multi_thread*TILE_SIZE);
        
		//data import
		for (int row = rowTile; row < rowTileEnd; row++) {
          for (int inner = innerTile; inner <  innerTileEnd; inner +=16) {
		  	for (int k = 0; k < 16 ; k ++) {
               #pragma HLS unroll
               left[row-rowTile][inner-innerTile + k]= in1[row * inners + inner+k];
            }
		  }
		}
		for (int inner = innerTile; inner <  innerTileEnd; inner++) { 
          for (int col = columnTile; col < columnTileEnd; col += 16) {
		    for (int k = 0; k < 16 ; k ++) {
              #pragma HLS unroll
              right[inner-innerTile][col-columnTile + k]= in2[inner * columns + col + k];
		    }
          }
        }
        

        //calculate
        for (int row = rowTile; row < rowTileEnd; row += 16 ) {
          for (int inner = innerTile; inner < innerTileEnd; inner++) {
   			#pragma HLS pipeline II = 1
            for (int col = columnTile; col < columnTileEnd; col += 16) {
			  #pragma HLS unroll
			  for(int ii=0; ii < 16; ii++){
			  #pragma HLS unroll
                for(int jj=0; jj < 16; jj++){
                  vout_buffer[row-rowTile+ii][col-columnTile+jj] +=  left[row-rowTile+ii][inner-innerTile] * right[inner-innerTile][col-columnTile+jj];
                  
                }
			   }

            }
          }
        }



      }
/////////////////////////////////one tile finish/////////////////////////////////////////////////////
	for(int j = 0; j< multi_thread*TILE_SIZE ; j ++ ){
        for (int i = 0; i< multi_thread*TILE_SIZE ; i += 16 ) {
		#pragma HLS pipeline II=1
			for (int k = 0; k < 16 ; k ++) {
			#pragma HLS unroll
               out[(rowTile+j) * columns + columnTile + i + k] = vout_buffer[j][i+k] ;
			}
        }
    }
/////////////////////////////////send vout


    }
  }
}
}
