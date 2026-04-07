//
// Hecho por Santiago Morales y Gabriel Díaz
//
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath> //cmath si se utiliza  (por la funcion exp) pero aparece como no utilizada

using namespace std;


#ifndef TENSORES_TENSOR_H
#define TENSORES_TENSOR_H

class TensorTransform;
class Tensor {
    bool eliminar = true;
    double* matriz;
    vector <size_t> shape;
public:

    //size_t es un tipo de dato que se usa para tamaños o indices o posiciones de memoria
    //Solo toma valores positvos y cero.
    Tensor(double* data, const vector<size_t>& shape);
    Tensor(const vector<size_t>& shape, const vector<double>& values);
    //constructor de copia
    static Tensor zeros(const vector<size_t>& shape);
    static Tensor ones(const vector<size_t>& shape);
    static Tensor random(const vector<size_t>& shape, const double& min,const double& max);
    static Tensor arange(const int& start, const int& end);
    size_t size() const;
    double& operator[](size_t i);
    const double& operator[](size_t i) const;

    // Constructores y Destructores requeridos en la clase Tensor
    Tensor(const Tensor& other);
    Tensor& operator=(const Tensor& other);
    Tensor(Tensor&& other) noexcept;
    Tensor& operator=(Tensor&& other) noexcept;
    ~Tensor();

    //Metodos de polimorfismo
    Tensor apply(const TensorTransform& transform) const;

    //Sobre carga de operadores
    Tensor operator+(const Tensor& other) const;
    Tensor operator-(const Tensor& other) const;
    Tensor operator*(const Tensor& other) const;
    Tensor operator*(double scalar) const;

    //Funcion view
    Tensor view(const vector<size_t>& new_shape) const;

    //Concatenacion
    static Tensor concat(const std::vector<Tensor>& tensors, int axis);

    //Funciones Amigas
    friend Tensor dot ( const Tensor & a , const Tensor & b ) ;
    friend Tensor matmul ( const Tensor & a , const Tensor & b );

    //Funcion para imprimir las matrices
    void print() const ;
};

//Clase abstracta
class TensorTransform {
public:
    virtual Tensor apply(const Tensor& t) const = 0;
    virtual ~TensorTransform() = default;
};

//Clase heredada ReLU
class ReLU : public TensorTransform {
public:
    Tensor apply(const Tensor& t) const override;
};

//Clase heredada Sigmoid
class Sigmoid: public TensorTransform {
public:
    Tensor apply(const Tensor& t) const override;
};
#endif //TENSORES_TENSOR_H