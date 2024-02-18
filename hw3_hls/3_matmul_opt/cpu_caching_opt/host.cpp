#include <cstring>
#include <iostream>
#include <chrono>
#include <math.h>
#include <omp.h>
#include <vector>

#define MATRIX_LEN 1024
#define DATA_SIZE MATRIX_LEN * MATRIX_LEN
using namespace std;

int main(int argc, char **argv) {


  // Create the test data
  vector<int> bo0(DATA_SIZE);
  vector<int> bo1(DATA_SIZE);
  vector<int> bufReference(DATA_SIZE);
  vector<int> bufReference_reorder(DATA_SIZE);
  vector<int> bufReference_opt(DATA_SIZE);
  vector<int> bufReference_tile(DATA_SIZE);
  vector<int> bufReference_2dtile(DATA_SIZE);


  for (int i = 0; i < DATA_SIZE; ++i) {
    bo0[i] = i;
    bo1[i] = i;
    bufReference[i] = 0;
    bufReference_reorder[i]=0;
    bufReference_opt[i]=0;
	bufReference_tile[i]=0;
	bufReference_2dtile[i]=0;
  }

//un pot
std::cout << "CPU start" << std::endl;
  auto cpu_begin = std::chrono::high_resolution_clock::now();
#pragma omp parallel for collapse(2) num_threads(64)
    for (int i = 0; i < MATRIX_LEN; i++){
     for (int j = 0; j < MATRIX_LEN; j++){
      for (int k = 0; k < MATRIX_LEN; k++){
        bufReference[i * MATRIX_LEN + j] += bo0[i * MATRIX_LEN + k] * bo1[k * MATRIX_LEN + j];
      }
     }
    }
  auto cpu_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU end" << std::endl;

//reorder loop
std::cout << "CPU reorder loop start" << std::endl;
  auto cpu_reorder_begin = std::chrono::high_resolution_clock::now();
#pragma omp parallel for collapse(2) num_threads(64)
    for (int i = 0; i < MATRIX_LEN; i++){
     for (int k = 0; k < MATRIX_LEN; k++){
	  int temp = bo0[i * MATRIX_LEN + k];
      for (int j = 0; j < MATRIX_LEN; j++){
        bufReference_reorder[i * MATRIX_LEN + j] += temp * bo1[k * MATRIX_LEN + j];
      }
     }
    }
  auto cpu_reorder_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU reorder loop end" << std::endl;

//tiling
std::cout << "CPU tile loop start" << std::endl;
const int tileSize=256;
auto cpu_tile_begin = std::chrono::high_resolution_clock::now();
#pragma omp parallel for num_threads(4)
   for (int innerTile = 0; innerTile < MATRIX_LEN; innerTile += tileSize) {
#pragma omp parallel for num_threads(16)   
    for (int i = 0; i < MATRIX_LEN; i++){
	 int innerTileEnd = std::min(MATRIX_LEN, innerTile + tileSize);
     for (int k = innerTile; k < innerTileEnd; k++){
      int temp = bo0[i * MATRIX_LEN + k];
      for (int j = 0; j < MATRIX_LEN; j++){
        bufReference_tile[i * MATRIX_LEN + j] += temp * bo1[k * MATRIX_LEN + j];
      }
     }
    }
   }
  auto cpu_tile_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU tile loop end" << std::endl;


//2d tiling
std::cout << "CPU 2d tile loop start" << std::endl;
//int tileSize=256;
auto cpu_2dtile_begin = std::chrono::high_resolution_clock::now();

#pragma omp parallel for collapse(2) num_threads(64)
 for (int rowTile = 0; rowTile < MATRIX_LEN; rowTile += tileSize) {
//#pragma omp parallel for num_threads(8)
  for (int columnTile = 0; columnTile < MATRIX_LEN; columnTile += tileSize) {
//#pragma omp parallel for num_threads(8)
   for (int innerTile = 0; innerTile < MATRIX_LEN; innerTile += tileSize) {
    for (int i = rowTile; i < rowTile + tileSize; i++){
     int innerTileEnd = std::min(MATRIX_LEN, innerTile + tileSize);
     for (int k = innerTile; k < innerTileEnd; k++){
      int temp = bo0[i * MATRIX_LEN + k];
      for (int j = columnTile; j < columnTile + tileSize; j++){
        bufReference_2dtile[i * MATRIX_LEN + j] += temp * bo1[k * MATRIX_LEN + j];
      }
     }
    }
   }
  }
 }
  auto cpu_2dtile_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU 2d tile loop end" << std::endl;



//opt
std::cout << "CPU OPT start" << std::endl;

auto cpu_opt_begin = std::chrono::high_resolution_clock::now();
vector<vector<int>> bo0_buffer(tileSize, vector<int>(tileSize));
vector<vector<int>> bo1_buffer(tileSize, vector<int>(tileSize));
#pragma omp parallel for collapse(2) num_threads(64)
 for (int rowTile = 0; rowTile < MATRIX_LEN; rowTile += tileSize) {
  for (int columnTile = 0; columnTile < MATRIX_LEN; columnTile += tileSize) {
   for (int innerTile = 0; innerTile < MATRIX_LEN; innerTile += tileSize) {
	///
	for(int x=0; x<tileSize; x++){
	 for(int y=0; y<tileSize; y++){
	   bo0_buffer[x][y] = bo0[(x + rowTile) * MATRIX_LEN + innerTile + y];
	   bo1_buffer[x][y] = bo1[(innerTile + x) * MATRIX_LEN + columnTile + y];
	 }
	}
    for (int i = 0; i < tileSize; i++){
     int innerTileEnd = std::min(MATRIX_LEN, innerTile + tileSize);
     for (int k = 0; k < tileSize; k++){
      int temp = bo0_buffer[i][k];
      for (int j = 0; j < tileSize; j++){
        bufReference_opt[(i+rowTile) * MATRIX_LEN + j+columnTile] += temp * bo1_buffer[k][j];
      }
     }
    }
	///
   }
  }
 }

  auto cpu_opt_end = std::chrono::high_resolution_clock::now();

std::cout << "CPU OPT end" << std::endl;



//finish
std::cout << "MATRIX 2048*2048 time results" << std::endl;

std::chrono::duration<double> cpu_duration = cpu_end - cpu_begin;
  std::cout << "CPU Time:                          " << cpu_duration.count() << " s" << std::endl;

std::chrono::duration<double> cpu_reorder_duration = cpu_reorder_end - cpu_reorder_begin;
std::cout << "CPU reorder Time:                    " << cpu_reorder_duration.count() << " s" << std::endl;

std::chrono::duration<double> cpu_tile_duration = cpu_tile_end - cpu_tile_begin;
std::cout << "CPU tile Time:                       " << cpu_tile_duration.count() << " s" << std::endl;

std::chrono::duration<double> cpu_2dtile_duration = cpu_2dtile_end - cpu_2dtile_begin;
std::cout << "CPU 2d tile Time:                    " << cpu_2dtile_duration.count() << " s" << std::endl;

std::chrono::duration<double> cpu_opt_duration = cpu_opt_end - cpu_opt_begin;
std::cout << "CPU OPT Time:                    " << cpu_opt_duration.count() << " s" << std::endl;

std::cout << "OPT Speedup:                 " << cpu_duration.count() / cpu_opt_duration.count() << " x" << std::endl;

  if (std::memcmp( &bufReference[0], &bufReference_2dtile[0], DATA_SIZE))
    throw std::runtime_error("Value read back does not match reference");

  std::cout << "TEST PASSED\n";

  }
