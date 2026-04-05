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

}