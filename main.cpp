#include <iostream>
#include "Tensor.h"
using namespace std;
int main() {
   Tensor A = Tensor::arange(-5, 5).view({2, 5});
   ReLU relu;
   Tensor B = A.apply(relu);
}