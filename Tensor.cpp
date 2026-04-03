//
// Created by smora on 3/04/2026.
//

#include "Tensor.h"
Tensor::Tensor(const vector<size_t>& shape, const vector<double>& values) {
    if (shape.size() > 3)
        throw invalid_argument("El tamaño maximo de dimnesiones tiene que ser 3");
    // Calcular tamaño total
    size_t total = 1;
    for (size_t m : shape) {
        total *= m;
    }
    if (values.size() != total) {
        throw invalid_argument("El tamanio de values no coincide con los requeridos por la dimension ingresada");
    }
    this->shape = shape;
    matriz = new double [total];
    for (size_t i = 0; i < total; i++) {
        matriz[i] = values[i];
    }
}
// Prototipos requeridos en la clase Tensor
//Constructor de copia
Tensor::Tensor(const Tensor& other) {
    this -> shape = other.shape;
    size_t total = 1;
    for (size_t m : other.shape) {
        total *= m;
    }
    for (int i = 0; i < total; i++) {
        matriz[i] = other.matriz[i];
    }
}

// Sobre caraga de =, copia
Tensor& Tensor::operator=(const Tensor& other) {
    //Evitar copiar lo mismo
    if (this == &other) {
        return *this;
    }
    delete [] matriz;
    this->shape = other.shape;
    size_t total = 1;
    for (size_t m : other.shape) {
        total *= m;
    }
    this->matriz = new double [total];
    for (int i = 0; i < total; i++) {
        matriz[i] = other.matriz[i];
    }
    return *this;
}

//Constructor de movimiento
Tensor::Tensor(Tensor&& other) noexcept {
    // Se usa move para transferir los datos del vector sin copiarlos,
    // evitando una copia innecesaria y mejorando el rendimiento
    this->shape = move(other.shape);
    this->matriz = other.matriz;
    other.matriz = nullptr;
    other.shape.clear();
}

//Sobrecarga =, movimeinto

Tensor& Tensor::operator=(Tensor&& other) noexcept {
    //Evitar copiar lo mismo
    if (this == &other) {
        return *this;
    }
    delete [] matriz;
    this->shape = move(other.shape);
    this->matriz = other.matriz;
    other.matriz = nullptr;
    other.shape.clear();
    return *this;
}































//Metodos estaticos para facilitar la creación de Tensores
Tensor Tensor::zeros(const vector<size_t>& shape) {
    size_t total_values = 1;
    for (size_t m : shape) {
        total_values *= m;
    }
    vector<double> values (total_values,0.0);
    return Tensor(shape,values);
}
Tensor Tensor::ones(const vector<size_t>& shape) {
    size_t total_values = 1;
    for (size_t m : shape) {
        total_values *= m;
    }
    vector<double> values (total_values,1.0);
    return Tensor(shape,values);
}
Tensor Tensor::random(const vector<size_t>& shape, const double& min,const double& max){
    vector<double> values;
    size_t total_values = 1;
    for (size_t m : shape) {
        total_values *= m;
    }
    for (int i = 0; i < total_values; i++) {
        double r = min + (double)rand() / RAND_MAX * (max - min);
        values.push_back(r);
    }
    return Tensor(shape,values);
}
Tensor Tensor::arange(const int& start, const int& end) {
    vector<double> values;
    size_t total_values = 0;
    for (int i = start; i < end; i++) {
        values.push_back(i);
        total_values++;
    }
    vector<size_t> shape = {total_values};
    return Tensor(shape,values);
}