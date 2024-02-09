#define BUFFER_SIZE 256
#define DATA_SIZE 65536

extern "C" {
void vadd(const int in1_0[DATA_SIZE/4],// Read-Only Vector 1
          const int in1_1[DATA_SIZE/4],
	  const int in1_2[DATA_SIZE/4],
	  const int in1_3[DATA_SIZE/4],

	  const int in2_0[DATA_SIZE/4],// Read-Only Vector 2
	  const int in2_1[DATA_SIZE/4],
	  const int in2_2[DATA_SIZE/4],
	  const int in2_3[DATA_SIZE/4],
          
	  int out_0[DATA_SIZE/4], // Output Result
	  int out_1[DATA_SIZE/4],
	  int out_2[DATA_SIZE/4],
	  int out_3[DATA_SIZE/4],
          int size                 // Size in integer
          ) {

#pragma HLS INTERFACE m_axi port = in1_0 offset = slave bundle = gmem0  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = in1_1 offset = slave bundle = gmem1  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = in1_2 offset = slave bundle = gmem2  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = in1_3 offset = slave bundle = gmem3  depth=DATA_SIZE/4

#pragma HLS INTERFACE m_axi port = in2_0 offset = slave bundle = gmem4  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = in2_1 offset = slave bundle = gmem5  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = in2_2 offset = slave bundle = gmem6  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = in2_3 offset = slave bundle = gmem7  depth=DATA_SIZE/4

#pragma HLS INTERFACE m_axi port = out_0 offset = slave bundle = gmem8  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = out_1 offset = slave bundle = gmem9  depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = out_2 offset = slave bundle = gmem10 depth=DATA_SIZE/4
#pragma HLS INTERFACE m_axi port = out_3 offset = slave bundle = gmem11 depth=DATA_SIZE/4

//#pragma HLS ARRAY_PARTITION cyclic variable=in1 factor=4
//#pragma HLS ARRAY_PARTITION cyclic variable=in2 factor=4
//#pragma HLS ARRAY_PARTITION cyclic variable=out factor=4

  int v1_buffer[DATA_SIZE];   //Local memory to store vector1
  int v2_buffer[DATA_SIZE];   // Local memory to store vector2
  int vout_buffer[DATA_SIZE]; // Local Memory to store result

#pragma HLS array_partition variable=v1_buffer factor=4 cyclic
#pragma HLS array_partition variable=v2_buffer factor=4 cyclic
#pragma HLS array_partition variable=vout_buffer factor=4 cyclic

   for (int j = 0; j < DATA_SIZE/4 ; j += 1) { 
       v1_buffer[j*4  ] = in1_0[j];
       v1_buffer[j*4+1] = in1_1[j];
       v1_buffer[j*4+2] = in1_2[j];
       v1_buffer[j*4+3] = in1_3[j];
   }
   for (int j = 0; j < DATA_SIZE/4 ; j += 1) {
       v2_buffer[j*4  ] = in2_0[j];
       v2_buffer[j*4+1] = in2_1[j];
       v2_buffer[j*4+2] = in2_2[j];
       v2_buffer[j*4+3] = in2_3[j];
   }

   for (int j = 0; j < DATA_SIZE ; j +=1 ) {
       vout_buffer[j] = v1_buffer[j]+v2_buffer[j];         
   }

   for (int j = 0; j < DATA_SIZE/4 ; j += 1) {
       out_0[j] = vout_buffer[j*4  ];
       out_1[j] = vout_buffer[j*4+1];
       out_2[j] = vout_buffer[j*4+2];
       out_3[j] = vout_buffer[j*4+3];
   }

}
}
