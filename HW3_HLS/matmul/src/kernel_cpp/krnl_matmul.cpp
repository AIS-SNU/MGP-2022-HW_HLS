/*******************************************************************************
Vendor: Xilinx
Associated Filename: krnl_vadd.cl
Purpose: SDx vector addition example
*******************************************************************************
Copyright (C) 2017 XILINX, Inc.

This file contains confidential and proprietary information of Xilinx, Inc. and
is protected under U.S. and international copyright and other intellectual
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials
distributed herewith. Except as otherwise provided in a valid license issued to
you by Xilinx, and to the maximum extent permitted by applicable law:
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY,
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether
in contract or tort, including negligence, or under any other theory of
liability) for any loss or damage of any kind or nature related to, arising under
or in connection with these materials, including for any direct, or any indirect,
special, incidental, or consequential loss or damage (including loss of data,
profits, goodwill, or any type of loss or damage suffered as a result of any
action brought by a third party) even if such damage or loss was reasonably
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any
application requiring fail-safe performance, such as life-support or safety
devices or systems, Class III medical devices, nuclear facilities, applications
related to the deployment of airbags, or any other applications that could lead
to death, personal injury, or severe property or environmental damage
(individually and collectively, "Critical Applications"). Customer assumes the
sole risk and liability of any use of Xilinx products in Critical Applications,
subject only to applicable laws and regulations governing limitations on product
liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
ALL TIMES.

*******************************************************************************/

//------------------------------------------------------------------------------
//
// kernel:  vadd
//
// Purpose: Demonstrate Vector Add in OpenCL
//


#include "real.h"


void krnl_matmul(
                int** a,
                int** b,
                int** c,
                const int n_elements)
{

#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=a max_read_burst_length = 256
#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem port=b max_read_burst_length = 256
#pragma HLS INTERFACE m_axi offset=SLAVE bundle=gmem1 port=c max_write_burst_length = 256

#pragma HLS INTERFACE s_axilite port=a  bundle=control
#pragma HLS INTERFACE s_axilite port=b  bundle=control
#pragma HLS INTERFACE s_axilite port=c  bundle=control

#pragma HLS INTERFACE s_axilite port=n_elements  bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
       real_t MatA[n_elements][n_elements];
       real_t MatB[n_elements][n_elements];
       real_t MatC[n_elements][n_elements];


       // Read in the data (Matrix A) from DRAM to BRAM
    //MAT_A_ROWS:
    for(int i = 0; i < n_elements; i++) {
        //MAT_A_COLS:
        for(int j = 0; j < n_elements; j++) {
	#pragma HLS pipeline ii = 1 rewind

            MatA[i][j] = a[i][j];
        }
    }

    // Read in the data (Matrix B) from DRAM to BRAM
    //MAT_B_ROWS:
    for(int i = 0; i < n_elements; i++) {
        //MAT_B_COLS:
        for(int j = 0; j < n_elements; j++) {
	#pragma HLS pipeline ii = 1 rewind
            MatB[i][j] = b[i][j];
        }
    }

    // Initialize product matrix C
    //MAT_C_ROWS_INIT:
    for(int i = 0; i < n_elements; i++) {
        //MAT_C_COLS_INIT:
        for(int j = 0; j < n_elements; j++) {
	#pragma HLS pipeline ii = 1 rewind
            MatC[i][j] = 0;
        }
    }

#pragma HLS array_partiotion variable=MatB dim=1 factor=64 cyclic
#pragma HLS array_partiotion variable=MatB dim=2 factor=64 cyclic
#pragma HLS array_partiotion variable=MatA dim=1 factor=64 cyclic
#pragma HLS array_partiotion variable=MatC dim=2 factor=64 cyclic

    real_t temp[n_elements];

    // Perform matrix multiplication
    //OUTER_ROWS:
    for(int i = 0; i < n_elements; i+=64) {
        //OUTER_COLS:
        for(int p = 0; p < n_elements; p++) {
            //INNER_ROW_COLS:
            for(int j = 0; j < n_elements ; j+=64) {
            #pragma HLS pipeline
                for(int ii=0; ii < 64; ii++){
                  for(int jj=0; jj < 64; jj++){
                     MatC[i+ii][j+jj] +=  MatA[i+ii][p] * MatB[p][j+jj];
                  }
                }
            }
        }
    }

    // Write back the data from BRAM to DRAM
    //MAT_C_ROWS:
    for(int i = 0; i < n_elements; i++) {
        //MAT_C_COLS:
        for(int j = 0; j < n_elements; j++) {
	#pragma HLS pipeline ii = 1 rewind
            c[i][j] = MatC[i][j];
        }
    }
}
