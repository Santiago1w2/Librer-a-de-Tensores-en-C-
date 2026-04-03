#include <iostream>
#include "Tensor.h"
using namespace std;
int main() {
    Tensor A = Tensor::zeros({2, 3});
    Tensor B = Tensor::ones({3, 3});
    Tensor C = Tensor::random({2, 2}, 0.0, 1.0);
    Tensor D = Tensor::arange(0, 6);
    return 0;
}