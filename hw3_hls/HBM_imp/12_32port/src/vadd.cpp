#define BUFFER_SIZE 256
#define DATA_SIZE 65536

extern "C" {
void vadd(const int in1_0[DATA_SIZE/16],// Read-Only Vector 1
          const int in1_1[DATA_SIZE/16],
	  const int in1_2[DATA_SIZE/16],
	  const int in1_3[DATA_SIZE/16],
	  const int in1_4[DATA_SIZE/16],
	  const int in1_5[DATA_SIZE/16],
	  const int in1_6[DATA_SIZE/16],
	  const int in1_7[DATA_SIZE/16],
	  const int in1_8[DATA_SIZE/16],
	  const int in1_9[DATA_SIZE/16],
	  const int in1_10[DATA_SIZE/16],
	  const int in1_11[DATA_SIZE/16],
	  const int in1_12[DATA_SIZE/16],
	  const int in1_13[DATA_SIZE/16],
	  const int in1_14[DATA_SIZE/16],
	  const int in1_15[DATA_SIZE/16],


	  const int in2_0[DATA_SIZE/16],// Read-Only Vector 2
	  const int in2_1[DATA_SIZE/16],
	  const int in2_2[DATA_SIZE/16],
	  const int in2_3[DATA_SIZE/16],
	  const int in2_4[DATA_SIZE/16],
	  const int in2_5[DATA_SIZE/16],
	  const int in2_6[DATA_SIZE/16],
	  const int in2_7[DATA_SIZE/16],
	  const int in2_8[DATA_SIZE/16],
	  const int in2_9[DATA_SIZE/16],
	  const int in2_10[DATA_SIZE/16],
	  const int in2_11[DATA_SIZE/16],
	  const int in2_12[DATA_SIZE/16],
	  const int in2_13[DATA_SIZE/16],
	  const int in2_14[DATA_SIZE/16],
	  const int in2_15[DATA_SIZE/16],
          
	  int out_0[DATA_SIZE/32], // Output Result
	  int out_1[DATA_SIZE/32],
	  int out_2[DATA_SIZE/32],
	  int out_3[DATA_SIZE/32],
	  int out_4[DATA_SIZE/32],
	  int out_5[DATA_SIZE/32],
	  int out_6[DATA_SIZE/32],
	  int out_7[DATA_SIZE/32],
	  int out_8[DATA_SIZE/32],
	  int out_9[DATA_SIZE/32],
	  int out_10[DATA_SIZE/32],
	  int out_11[DATA_SIZE/32],
	  int out_12[DATA_SIZE/32],
	  int out_13[DATA_SIZE/32],
	  int out_14[DATA_SIZE/32],
	  int out_15[DATA_SIZE/32],
	  int out_16[DATA_SIZE/32],
	  int out_17[DATA_SIZE/32],
	  int out_18[DATA_SIZE/32],
	  int out_19[DATA_SIZE/32],
	  int out_20[DATA_SIZE/32],
	  int out_21[DATA_SIZE/32],
	  int out_22[DATA_SIZE/32],
	  int out_23[DATA_SIZE/32],
	  int out_24[DATA_SIZE/32],
	  int out_25[DATA_SIZE/32],
	  int out_26[DATA_SIZE/32],
	  int out_27[DATA_SIZE/32],
	  int out_28[DATA_SIZE/32],
	  int out_29[DATA_SIZE/32],
	  int out_30[DATA_SIZE/32],
	  int out_31[DATA_SIZE/32]
          ) {

#pragma HLS INTERFACE m_axi port = in1_0 offset = slave bundle = gmem0  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_1 offset = slave bundle = gmem1  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_2 offset = slave bundle = gmem2  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_3 offset = slave bundle = gmem3  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_4 offset = slave bundle = gmem4  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_5 offset = slave bundle = gmem5  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_6 offset = slave bundle = gmem6  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_7 offset = slave bundle = gmem7  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_8 offset = slave bundle = gmem8  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_9 offset = slave bundle = gmem9  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_10 offset = slave bundle = gmem10  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_11 offset = slave bundle = gmem11  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_12 offset = slave bundle = gmem12  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_13 offset = slave bundle = gmem13  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_14 offset = slave bundle = gmem14  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in1_15 offset = slave bundle = gmem15  depth=DATA_SIZE/16 num_read_outstanding=32

#pragma HLS INTERFACE m_axi port = in2_0 offset = slave bundle = gmem16  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_1 offset = slave bundle = gmem17  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_2 offset = slave bundle = gmem18  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_3 offset = slave bundle = gmem19  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_4 offset = slave bundle = gmem20  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_5 offset = slave bundle = gmem21  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_6 offset = slave bundle = gmem22  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_7 offset = slave bundle = gmem23  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_8 offset = slave bundle = gmem24  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_9 offset = slave bundle = gmem25  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_10 offset = slave bundle = gmem26  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_11 offset = slave bundle = gmem27  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_12 offset = slave bundle = gmem28  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_13 offset = slave bundle = gmem29  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_14 offset = slave bundle = gmem30  depth=DATA_SIZE/16 num_read_outstanding=32
#pragma HLS INTERFACE m_axi port = in2_15 offset = slave bundle = gmem31  depth=DATA_SIZE/16 num_read_outstanding=32

#pragma HLS INTERFACE m_axi port = out_0 offset = slave bundle = gmem0  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_1 offset = slave bundle = gmem1  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_2 offset = slave bundle = gmem2  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_3 offset = slave bundle = gmem3  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_4 offset = slave bundle = gmem4  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_5 offset = slave bundle = gmem5  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_6 offset = slave bundle = gmem6  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_7 offset = slave bundle = gmem7  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_8 offset = slave bundle = gmem8  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_9 offset = slave bundle = gmem9  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_10 offset = slave bundle = gmem10  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_11 offset = slave bundle = gmem11  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_12 offset = slave bundle = gmem12  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_13 offset = slave bundle = gmem13  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_14 offset = slave bundle = gmem14 depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_15 offset = slave bundle = gmem15  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_16 offset = slave bundle = gmem16  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_17 offset = slave bundle = gmem17  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_18 offset = slave bundle = gmem18  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_19 offset = slave bundle = gmem19  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_20 offset = slave bundle = gmem20  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_21 offset = slave bundle = gmem21  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_22 offset = slave bundle = gmem22  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_23 offset = slave bundle = gmem23  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_24 offset = slave bundle = gmem24  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_25 offset = slave bundle = gmem25  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_26 offset = slave bundle = gmem26  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_27 offset = slave bundle = gmem27  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_28 offset = slave bundle = gmem28  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_29 offset = slave bundle = gmem29  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_30 offset = slave bundle = gmem30  depth=DATA_SIZE/32 num_write_outstanding=32
#pragma HLS INTERFACE m_axi port = out_31 offset = slave bundle = gmem31  depth=DATA_SIZE/32 num_write_outstanding=32
		  
		  
		  
		  
		  //#pragma HLS ARRAY_PARTITION cyclic variable=in1 factor=4
//#pragma HLS ARRAY_PARTITION cyclic variable=in2 factor=4
//#pragma HLS ARRAY_PARTITION cyclic variable=out factor=4

  int v1_buffer[DATA_SIZE] __attribute__((buffer_location("bram")));  //Local memory to store vector1
  int v2_buffer[DATA_SIZE] __attribute__((buffer_location("bram")));  // Local memory to store vector2
//  int vout_buffer[DATA_SIZE] __attribute__((buffer_location("bram"))); // Local Memory to store result

#pragma HLS array_partition variable=v1_buffer factor=32 cyclic
#pragma HLS array_partition variable=v2_buffer factor=32 cyclic
//#pragma HLS array_partition variable=vout_buffer factor=32 cyclic

   for (int j = 0; j < DATA_SIZE/16 ; j += 1) { 
       v1_buffer[j*16  ] = in1_0[j];
       v1_buffer[j*16+1] = in1_1[j];
       v1_buffer[j*16+2] = in1_2[j];
       v1_buffer[j*16+3] = in1_3[j];
       v1_buffer[j*16+4] = in1_4[j];
       v1_buffer[j*16+5] = in1_5[j];
       v1_buffer[j*16+6] = in1_6[j];
       v1_buffer[j*16+7] = in1_7[j];
       v1_buffer[j*16+8] = in1_8[j];
       v1_buffer[j*16+9] = in1_9[j];
       v1_buffer[j*16+10] = in1_10[j];
       v1_buffer[j*16+11] = in1_11[j];
       v1_buffer[j*16+12] = in1_12[j];
       v1_buffer[j*16+13] = in1_13[j];
       v1_buffer[j*16+14] = in1_14[j];
       v1_buffer[j*16+15] = in1_15[j];

   }
   for (int j = 0; j < DATA_SIZE/16 ; j += 1) {
       v2_buffer[j*16  ] = in2_0[j];
       v2_buffer[j*16+1] = in2_1[j];
       v2_buffer[j*16+2] = in2_2[j];
       v2_buffer[j*16+3] = in2_3[j];
       v2_buffer[j*16+4] = in2_4[j];
       v2_buffer[j*16+5] = in2_5[j];
       v2_buffer[j*16+6] = in2_6[j];
       v2_buffer[j*16+7] = in2_7[j];
       v2_buffer[j*16+8] = in2_8[j];
       v2_buffer[j*16+9] = in2_9[j];
       v2_buffer[j*16+10] = in2_10[j];
       v2_buffer[j*16+11] = in2_11[j];
       v2_buffer[j*16+12] = in2_12[j];
       v2_buffer[j*16+13] = in2_13[j];
       v2_buffer[j*16+14] = in2_14[j];
       v2_buffer[j*16+15] = in2_15[j];

   }


   for (int j = 0; j < DATA_SIZE/32 ; j += 1) {
       out_0[j] = v1_buffer[j*32  ];
       out_1[j] = v1_buffer[j*32+1];
       out_2[j] = v2_buffer[j*32+2];
       out_3[j] = v2_buffer[j*32+3];
       out_4[j] = v1_buffer[j*32+4];
       out_5[j] = v1_buffer[j*32+5];
       out_6[j] = v2_buffer[j*32+6];
       out_7[j] = v2_buffer[j*32+7];
       out_8[j] = v1_buffer[j*32+8];
       out_9[j] = v1_buffer[j*32+9];
       out_10[j] = v2_buffer[j*32+10];
       out_11[j] = v2_buffer[j*32+11];
       out_12[j] = v1_buffer[j*32+12];
       out_13[j] = v1_buffer[j*32+13];
       out_14[j] = v2_buffer[j*32+14];
       out_15[j] = v2_buffer[j*32+15];
       out_16[j] = v1_buffer[j*32+16];
       out_17[j] = v1_buffer[j*32+17];
       out_18[j] = v2_buffer[j*32+18];
       out_19[j] = v2_buffer[j*32+19];
       out_20[j] = v1_buffer[j*32+20];
       out_21[j] = v1_buffer[j*32+21];
       out_22[j] = v2_buffer[j*32+22];
       out_23[j] = v2_buffer[j*32+23];
       out_24[j] = v1_buffer[j*32+24];
       out_25[j] = v1_buffer[j*32+25];
       out_26[j] = v2_buffer[j*32+26];
       out_27[j] = v2_buffer[j*32+27];
       out_28[j] = v1_buffer[j*32+28];
       out_29[j] = v1_buffer[j*32+29];
       out_30[j] = v2_buffer[j*32+30];
       out_31[j] = v2_buffer[j*32+31];


   }

}
}
