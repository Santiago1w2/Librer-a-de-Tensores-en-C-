#include <iostream>
#include "Tensor.h"
using namespace std;
int main() {
   /* Tensor A = Tensor::arange(-5, 5).view({2, 5});
   //A.display();
   ReLU relu;
   Tensor B = A.apply(relu);
   //B.display();
   cout << "B creada exitosamente" << endl;

   Tensor C = A + B ;
   //C.display();
   Tensor D = A - B ;
   Tensor E = A * B ;
   Tensor F = A * 2.0;
   cout << "Operaciones hechas exitosamente" << endl;

   */

/*
   Tensor A = Tensor :: ones ({2 , 2}) ;
   cout << "Tensor A creado" << endl ;
   Tensor B = Tensor :: zeros ({2 , 2}) ;
   cout << "Tensor B creado" << endl ;
   Tensor C = Tensor :: concat ({ A , B } , 0) ;
   cout << "Tensor C concatenado" << endl ;


Tensor D = dot(A, B);
   cout << "Tensor D multiplicado" << endl ;
   Tensor E = matmul(A, B);
   cout << "Tensor E multiplicado" << endl ;
*/


////SOLAMENTE LA PRUEBA 1 FUNCIONA JAJSAJSAJSA

//1. Crear un tensor de entrada de dimensiones 1000 × 20 × 20.
   Tensor tensorEntrada = Tensor::random({1000, 20, 20}, 0.0, 4.0);
   cout << "Paso 1 Completo\n";
   //tensorEntrada.print();
//2. Transformarlo a 1000 × 400 usando view.
   Tensor transformado = tensorEntrada.view({1000, 400});
   cout << "Paso 2 Completo\n";
//3. Multiplicarlo por una matriz 400 × 100.
   Tensor const mat1 = Tensor::random({400, 100}, 0.0, 4.0);
   Tensor multiplicado = matmul(transformado, mat1);
   cout << "Paso 3 Completo\n";
//4. Sumar una matriz 1 × 100.
   Tensor sumado = multiplicado + Tensor::random({1, 100}, 0.0, 1.0);
   cout << "Paso 4 Completo\n";
//5. Aplicar la funci´on ReLU.
   ReLU re_lu;
   Tensor tensorReLu = sumado.apply(re_lu);
//6. Multiplicar por una matriz 100 × 10.
   Tensor produto = matmul(tensorReLu, Tensor::random({100, 10}, 0.0, 4.0));
   cout << "Paso 6 Completo\n";
//7. Sumar una matriz 1 × 10.
    produto = produto + Tensor::random({1, 10}, 0.0, 4.0);
   cout << "Paso  7 Completo\n";
   produto.print();
//8. Aplicar la funci´on Sigmoid
   Sigmoid sigmoid;
   Tensor sigmoidTensor = produto.apply(sigmoid);
   cout << "Paso 8 Completo\n";
   sigmoidTensor.print();
}