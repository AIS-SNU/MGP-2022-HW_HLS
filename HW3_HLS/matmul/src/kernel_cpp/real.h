///////////////////////////////////////////////////////////////////////////////
// Course:      ECE8893 - Parallel Programming for FPGAs
// Filename:    real.h
// Description: Header file for real matrix multiplication
//
// Note:        DO NOT MODIFY THIS CODE!
///////////////////////////////////////////////////////////////////////////////

#ifndef __REAL_H__
#define __REAL_H__

#include <stdio.h>
#include <stdlib.h>

#include <ap_int.h>

typedef ap_int<16> real_t;

void krnl_matmul(
                int* a,
                int* b,
                int* c,
                const int n_elements
		);

#endif
