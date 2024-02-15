#include <cstring>
#include <iostream>
#include <chrono>
#include <math.h>
#include <omp.h>

#define DATA_SIZE 65536
#define MATRIX_LEN 256

int main(int argc, char **argv) {


  // Create the test data
  int bo0[DATA_SIZE];
  int bo1[DATA_SIZE];
  int bufReference[DATA_SIZE];
  int bufReference_opt[DATA_SIZE];
  for (int i = 0; i < DATA_SIZE; ++i) {
    bo0[i] = i;
    bo1[i] = i;
    bufReference[i] = 0;
    bufReference_opt[i]=0;
  }

int bo0_buffer[DATA_SIZE];
int bo1_buffer[DATA_SIZE];

std::cout << "CPU start" << std::endl;
//#pragma omp parallel for num_threads(16)
  auto cpu_begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < MATRIX_LEN; i++){
     for (int k = 0; k < MATRIX_LEN; k++){
      for (int j = 0; j < MATRIX_LEN; j++){
        bufReference[i * MATRIX_LEN + j] += bo0[i * MATRIX_LEN + k] * bo1[k * MATRIX_LEN + j];
      }
     }
    }
  auto cpu_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU end" << std::endl;

std::cout << "CPU OPT start" << std::endl;

  auto cpu_opt_begin = std::chrono::high_resolution_clock::now();
//
//template <int rows, int columns, int inners, int tileSize = 4>
#pragma omp parallel for num_threads(16)
  for (int rowTile = 0; rowTile < MATRIX_LEN; rowTile += 64) {
    for (int columnTile = 0; columnTile < MATRIX_LEN; columnTile += 64) {
      for (int innerTile = 0; innerTile < MATRIX_LEN; innerTile += 16) {
        for (int row = rowTile; row < rowTile + 64; row++) {
          int innerTileEnd = std::min(MATRIX_LEN, innerTile + 16);
          for (int inner = innerTile; inner < innerTileEnd; inner++) {
            for (int col = columnTile; col < columnTile + 64; col++) {
              bufReference_opt[row * MATRIX_LEN + col] += bo0[row * MATRIX_LEN + inner] * bo1[inner * MATRIX_LEN + col];
} } } } } } 
  auto cpu_opt_end = std::chrono::high_resolution_clock::now();


std::cout << "CPU OPT end" << std::endl;



  std::chrono::duration<double> cpu_opt_duration = cpu_opt_end - cpu_opt_begin;
  std::cout << "CPU OPT Time:                    " << cpu_opt_duration.count() << " s" << std::endl;

  std::chrono::duration<double> cpu_duration = cpu_end - cpu_begin;
  std::cout << "CPU Time:                     " << cpu_duration.count() << " s" << std::endl;

  std::cout << "OPT Speedup:                 " << cpu_duration.count() / cpu_opt_duration.count() << " x" << std::endl;

  if (std::memcmp(bufReference_opt, bufReference, DATA_SIZE))
    throw std::runtime_error("Value read back does not match reference");

  std::cout << "TEST PASSED\n";

  }
