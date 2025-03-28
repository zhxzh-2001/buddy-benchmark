
//===- Utils.cpp ----------------------------------------------------------===//
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//===----------------------------------------------------------------------===//
//
// This file implements Batch MatMul helper functions.
//
//===----------------------------------------------------------------------===//
#ifndef BATCH_MATMUL_RVV_UTILS_HPP
#define BATCH_MATMUL_RVV_UTILS_HPP
#include "Utils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

namespace batch_matmul_int {

// Allocates a 1D array with dimensions `rows * cols` and fills it with random
// values between 0 and 99.
template <typename DATA_TYPE> DATA_TYPE *allocArray(int rows, int cols) {
  // Initialize the random number generator.
  std::srand(static_cast<unsigned int>(std::time(0)));
  // Allocate memory for the array
  DATA_TYPE *array = new DATA_TYPE[rows * cols];
  // Fill the array with random numbers between 0 and 99
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      array[i * cols + j] = static_cast<DATA_TYPE>(std::rand() % 100);
    }
  }
  return array;
}

// Verifies two arrays for equality within a specified tolerance.
template <typename DATA_TYPE>
void verify(DATA_TYPE *A, DATA_TYPE *B, int batch, int size,
            const std::string &name) {
  const std::string PASS = "\033[32mPASS\033[0m";
  const std::string FAIL = "\033[31mFAIL\033[0m";
  const double epsilon = 1e-6; // Tolerance for floating point comparison

  std::cout << name << " ";
  if (!A || !B) {
    std::cout << FAIL << " (Null pointer detected)" << std::endl;
    return;
  }

  bool isPass = true;
  for (int i = 0; i < batch; ++i) {
    for (int j = 0; j < size; ++j) {
      int k = i * size + j;
      if (std::fabs(A[k] - B[k]) > epsilon) {
        std::cout << FAIL << std::endl;
        std::cout << "Batch=" << i << " Index=" << j << ":\tA[k]=" << A[k]
                  << " B[k]=" << B[k] << std::endl;
        isPass = false;
        break;
      }
    }
    if (!isPass) {
      break;
    }
  }
  if (isPass) {
    std::cout << PASS << std::endl;
  }
}

} // namespace batch_matmul_int
#endif
