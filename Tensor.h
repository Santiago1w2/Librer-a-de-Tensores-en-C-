//
// Created by smora on 3/04/2026.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;


#ifndef TENSORES_TENSOR_H
#define TENSORES_TENSOR_H

class TensorTransform;
class Tensor {
    bool eliminar = true;
    double* matriz;
    vector <size_t> shape;
public:

    //size_t es un tipo de dato que se ua para tamaños o indices o posiciones de memoria
    //Solo toma valores positovs y cero.
    Tensor(double* data, const vector<size_t>& shape);
    Tensor(const vector<size_t>& shape, const vector<double>& values);
    static Tensor zeros(const vector<size_t>& shape);
    static Tensor ones(const vector<size_t>& shape);
    static Tensor random(const vector<size_t>& shape, const double& min,const double& max);
    static Tensor arange(const int& start, const int& end);
    size_t size() const;
    double& operator[](size_t i);
    const double& operator[](size_t i) const;

    // Prototipos requeridos en la clase Tensor
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

    Tensor view(const vector<size_t>& new_shape) const;

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