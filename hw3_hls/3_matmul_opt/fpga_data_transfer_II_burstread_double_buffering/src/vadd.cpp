#include <iostream>

#define TILE_SIZE 64
#define multi_thread 1


#define VDATA_SIZE 16
typedef struct v_datatype { unsigned int data[VDATA_SIZE]; } v_dt;

extern "C" void resetToZero(int input[TILE_SIZE][TILE_SIZE]);
extern "C" void readOneTile(int input[TILE_SIZE][TILE_SIZE] ,const v_dt *in,int inners, const int rowTile, const int innerTile);
extern "C" void exportOneTile( int vout_buffer[TILE_SIZE][TILE_SIZE],  v_dt *out,int columns ,const int rowTile, const int columnTile);
extern "C" void calculate(int vout_buffer[TILE_SIZE][TILE_SIZE],const int left[TILE_SIZE][TILE_SIZE],const int right[TILE_SIZE][TILE_SIZE]);


extern "C" {
void vadd(const v_dt *in1,  // Read-Only Vector 1
          const v_dt *in2,  // Read-Only Vector 2
          v_dt *out,        // Output Result
		  int size		   // matrix len
          ) {

#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0 max_widen_bitwidth=512 
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1 max_widen_bitwidth=512
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2 max_widen_bitwidth=512


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
    for (int columnTile = 0; columnTile < columns; columnTile += TILE_SIZE){ 
//one tile start
	  resetToZero(vout_buffer);
      for (int innerTile = 0; innerTile < inners; innerTile += TILE_SIZE) {
        //data import
		readOneTile( left , in1,inners, rowTile, innerTile);
		readOneTile( right , in2,columns ,innerTile, columnTile);
        //calculate
		calculate( vout_buffer, left, right);
      }
		//export
	  exportOneTile( vout_buffer, out ,columns, rowTile, columnTile);
/////////////////////////////////one tile finish
    }
  }
}
}


extern "C" {
void resetToZero(int input[TILE_SIZE][TILE_SIZE]){

     for (int i = 0; i < TILE_SIZE ; i ++) {
      for (int j = 0; j < TILE_SIZE ; j+=16) {
      #pragma HLS pipeline II=1
        for (int k = 0; k < 16 ; k ++) {
        #pragma HLS unroll
          input[i][j+k] = 0;
        }
      }
     }
}}
extern "C" {
void readOneTile(int input[TILE_SIZE][TILE_SIZE] ,const v_dt *in,int inners, const int rowTile, const int innerTile){
        for (int row = 0; row < TILE_SIZE; row++) {
          for (int inner = 0; inner < TILE_SIZE; inner += 16) {
          #pragma HLS pipeline II=1
            int in1_ = ((row + rowTile) * inners +innerTile+ inner)/16;
            for (int k = 0; k < 16 ; k ++) {
               #pragma HLS unroll
               input[row][inner + k]= in[in1_].data[k];
            }
          }
        }
}}
extern "C" {
void exportOneTile( int vout_buffer[TILE_SIZE][TILE_SIZE],  v_dt *out,int columns ,const int rowTile, const int columnTile){
    for(int j = 0; j< TILE_SIZE ; j ++ ){
        for (int i = 0; i< TILE_SIZE ; i += 16 ) {
        #pragma HLS pipeline II=1
            int out_ = (rowTile+j) * columns + columnTile+i;
            for (int k = 0; k < 16 ; k ++) {
            #pragma HLS unroll
              out[out_/16].data[ k] = vout_buffer[j][i+k] ;
            }
        }
    }
}}
extern "C" {
void calculate(int vout_buffer[TILE_SIZE][TILE_SIZE],const int left[TILE_SIZE][TILE_SIZE],const int right[TILE_SIZE][TILE_SIZE]){
        for (int row = 0; row < TILE_SIZE; row += 16 ) {
          for (int inner = 0; inner < TILE_SIZE; inner++) {
            #pragma HLS pipeline II = 1
            for (int col = 0; col < TILE_SIZE; col += 16) {
              #pragma HLS unroll
              for(int ii=0; ii < 16; ii++){
              #pragma HLS unroll
                for(int jj=0; jj < 16; jj++){
                vout_buffer[row+ii][col+jj] +=  left[row+ii][inner] * right[inner][col+jj];

                }
               }

            }
          }
        }

}}

