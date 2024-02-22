#include <iostream>

#define TILE_SIZE 256
#define multi_thread 1


#define VDATA_SIZE 16
typedef struct v_datatype { unsigned int data[VDATA_SIZE]; } v_dt;

extern "C" {
void vadd(const v_dt *in1,  // Read-Only Vector 1
          const v_dt *in2,  // Read-Only Vector 2
          v_dt *out,        // Output Result
		  int size		   // matrix len
          ) {

#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 max_widen_bitwidth=512 max_read_burst_length = 16 
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1 max_widen_bitwidth=512 max_read_burst_length = 16
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2 max_widen_bitwidth=512 max_write_burst_length = 16


//setting constant
int BUFFER_SIZE = size;
int rows = BUFFER_SIZE;
int inners = BUFFER_SIZE;
int columns = BUFFER_SIZE;
int vSize = TILE_SIZE / VDATA_SIZE;


int vout_buffer[TILE_SIZE][TILE_SIZE];
#pragma HLS array_partition variable=vout_buffer dim=1 factor=16 cyclic
#pragma HLS array_partition variable=vout_buffer dim=2 factor=16 cyclic

//burst write
//init vout_buffer
int left[TILE_SIZE][TILE_SIZE];
int right[TILE_SIZE][TILE_SIZE];
//#pragma HLS array_partition variable=left dim=1 factor=16 cyclic
//#pragma HLS array_partition variable=right dim=1 factor=16 cyclic
#pragma HLS array_partition variable=left dim=2 factor=16 cyclic
#pragma HLS array_partition variable=right dim=2 factor=16 cyclic

  for (int rowTile = 0; rowTile < rows; rowTile += TILE_SIZE) {
    for (int columnTile = 0; columnTile < columns; columnTile += TILE_SIZE) {

/////////////////////////////////reset vout
	 for (int i = 0; i < TILE_SIZE ; i ++) {
      for (int j = 0; j < TILE_SIZE ; j+=16) {
      #pragma HLS pipeline II=1
        for (int k = 0; k < 16 ; k ++) {
        #pragma HLS unroll
          vout_buffer[i][j+k] = 0;
        }
      }
     }

/////////////////////////////////one tile start/////////////////////////////////////////////////////
      for (int innerTile = 0; innerTile < inners; innerTile += TILE_SIZE) {
//  	  #pragma HLS pipeline II=1
        //data import
		for (int row = rowTile; row < rowTile + TILE_SIZE; row++) {
          for (int inner = innerTile; inner <  innerTile + TILE_SIZE; inner += 16) {
			int in1_ = (row * inners + inner)/16;
		  	for (int k = 0; k < 16 ; k ++) {
               #pragma HLS unroll
               left[row-rowTile][inner-innerTile + k]= in1[in1_].data[k];
            }
		  }
		}
		for (int inner = innerTile; inner <  innerTile + TILE_SIZE; inner++) { 
          for (int col = columnTile; col < columnTile + TILE_SIZE; col += 16) {
			int in2_ = (inner * columns + col)/16;
		    for (int k = 0; k < 16 ; k ++) {
              #pragma HLS unroll
              right[inner-innerTile][col-columnTile + k]= in2[in2_].data[k];
		    }
          }
        }
        

        //calculate
        for (int row = rowTile; row < rowTile + TILE_SIZE; row++) {
          for (int inner = innerTile; inner < innerTile + TILE_SIZE; inner++) {
            for (int col = columnTile; col < columnTile + TILE_SIZE; col++) {
              #pragma HLS unroll
              vout_buffer[row-rowTile][col-columnTile] += left[row-rowTile][inner-innerTile] * right[inner-innerTile][col-columnTile];
            }
          }
        }

      }
/////////////////////////////////one tile finish/////////////////////////////////////////////////////
	for(int j = 0; j< TILE_SIZE ; j ++ ){
        for (int i = 0; i< TILE_SIZE ; i += 16 ) {
		#pragma HLS pipeline II=1
			int out_ = (rowTile+j) * columns + columnTile+i;
			for (int k = 0; k < 16 ; k ++) {
			#pragma HLS unroll
              out[out_/16].data[ k] = vout_buffer[j][i+k] ;
			  //out[out_ + k] = left[j][i+k] ;
			}
        }
    }
/////////////////////////////////send vout


    }
  }
}
}
