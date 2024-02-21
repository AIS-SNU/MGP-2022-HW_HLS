#include <cstring>
#include <iostream>
#include <chrono>
#include <math.h>
#include <omp.h>
#include <vector>

#define MATRIX_LEN 1024*4
#define DATA_SIZE MATRIX_LEN * MATRIX_LEN
#define rows MATRIX_LEN
#define columns MATRIX_LEN
#define inners MATRIX_LEN
#define tileSize 400

using namespace std;

void matmulImplNaive(const int *left, const int *right,
                            int *result) {
#pragma omp parallel for schedule(dynamic) shared(result, left, right) default(none)  collapse(1)  num_threads(48)
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < columns; col++) {
      for (int inner = 0; inner < inners; inner++) {
        result[row * columns + col] +=
            left[row * columns + inner] * right[inner * columns + col];
} } } }

void matmulImplLoopOrder(const int *left, const int *right,
                            int *result) {
#pragma omp parallel num_threads(48) shared(result, left, right) default(none)
#pragma omp for nowait schedule(dynamic) collapse(1)

  for (int row = 0; row < rows; row++) {
    for (int inner = 0; inner < inners; inner++) {
      for (int col = 0; col < columns; col++) {
        result[row * columns + col] +=
            left[row * columns + inner] * right[inner * columns + col];
} } } }

void matmulImplTiling(const int *left, const int *right,
                            int *result){
#pragma omp parallel num_threads(48) shared(result, left, right) default(none) 
#pragma omp for nowait schedule(dynamic) collapse(2)
  for (int innerTile = 0; innerTile < inners; innerTile += tileSize) {
    for (int row = 0; row < rows; row++) {
      int innerTileEnd = std::min(inners, innerTile + tileSize);
      for (int inner = innerTile; inner < innerTileEnd; inner++) {
        for (int column = 0; column < columns; column++) {
          result[row * columns + column] +=
              left[row * inners + inner] * right[inner * columns + column];
} } } } }


void matmulImplRowColParallelInnerTiling(const int*left,const int *right,int *result) {
#pragma omp parallel num_threads(48) shared(result, left, right) default(none)
#pragma omp for nowait schedule(dynamic) collapse(2)
  for (int rowTile = 0; rowTile < rows; rowTile += 512) {
    for (int columnTile = 0; columnTile < columns; columnTile += 512) {
      for (int innerTile = 0; innerTile < inners; innerTile += tileSize) {
	     for (int row = rowTile; row < rowTile + 512; row++) {
          int innerTileEnd = std::min(inners, innerTile + tileSize);
          for (int inner = innerTile; inner < innerTileEnd; inner++) {
            for (int col = columnTile; col < columnTile + 512; col++) {
              result[row * columns + col] += left[row * inners + inner] * right[inner * columns + col];

} } } } } } }

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
cout<<omp_get_max_threads()<<endl;
//un pot
std::cout << "CPU start" << std::endl;
  auto cpu_begin = std::chrono::high_resolution_clock::now();
  matmulImplNaive(&bo0[0],&bo1[0],&bufReference[0]);
  auto cpu_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU end" << std::endl;



//tiling
std::cout << "CPU tile loop start" << std::endl;
auto cpu_tile_begin = std::chrono::high_resolution_clock::now();
matmulImplTiling(&bo0[0],&bo1[0],&bufReference_tile[0]);
auto cpu_tile_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU tile loop end" << std::endl;

//reorder loop
std::cout << "CPU reorder loop start" << std::endl;
  auto cpu_reorder_begin = std::chrono::high_resolution_clock::now();
  matmulImplLoopOrder(&bo0[0],&bo1[0],&bufReference_reorder[0]);
  auto cpu_reorder_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU reorder loop end" << std::endl;

//2d tiling
std::cout << "CPU 2d tile mutithread  loop start" << std::endl;
auto cpu_2dtile_begin = std::chrono::high_resolution_clock::now();
matmulImplRowColParallelInnerTiling(&bo0[0],&bo1[0],&bufReference_2dtile[0]);
auto cpu_2dtile_end = std::chrono::high_resolution_clock::now();
std::cout << "CPU 2d tile mutithread loop end" << std::endl;


std::cout << "MATRIX size "<< MATRIX_LEN <<" x "<< MATRIX_LEN <<  " time results" << std::endl;

std::chrono::duration<double> cpu_duration = cpu_end - cpu_begin;
std::cout << "CPU Time:                            " << cpu_duration.count() << " s" << std::endl;

std::chrono::duration<double> cpu_reorder_duration = cpu_reorder_end - cpu_reorder_begin;
std::cout << "CPU reorder Time:                    " << cpu_reorder_duration.count() << " s" << std::endl;

std::chrono::duration<double> cpu_tile_duration = cpu_tile_end - cpu_tile_begin;
std::cout << "CPU tile Time:                       " << cpu_tile_duration.count() << " s" << std::endl;

std::chrono::duration<double> cpu_2dtile_duration = cpu_2dtile_end - cpu_2dtile_begin;
std::cout << "CPU 2d tile multithread Time:        " << cpu_2dtile_duration.count() << " s" << std::endl;

//for(int i =DATA_SIZE-40 ; i<DATA_SIZE;i++){
//std::cout <<"answer "<< bufReference[i] << " result  " << bufReference_tile[i] << std::endl;
//}


if (std::memcmp( &bufReference[0], &bufReference_reorder[0], DATA_SIZE))
    throw std::runtime_error(" bufReference_reorder does not match reference");
//if (std::memcmp( &bufReference[0], &bufReference_tile[0], DATA_SIZE))
//    throw std::runtime_error("bufReference_tile does not match reference");
if (std::memcmp( &bufReference[0], &bufReference_2dtile[0], DATA_SIZE))
    throw std::runtime_error("bufReference_2dtile does not match reference");

  std::cout << "TEST PASSED\n";

  }
