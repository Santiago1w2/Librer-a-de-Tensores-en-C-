//
// Created by smora on 3/04/2026.
//
#include <iostream>
#include <vector>
using namespace std;


#ifndef TENSORES_TENSOR_H
#define TENSORES_TENSOR_H


class Tensor {
    double* matriz;
    vector <size_t> shape;
public:
    //size_t es un tipo de dato que se ua para tamaños o indices o posiciones de memoria
    //Solo toma valores positovs y cero.
    Tensor(const vector<size_t>& shape, const vector<double>& values);
    static Tensor zeros(const vector<size_t>& shape);
    static Tensor ones(const vector<size_t>& shape);
    static Tensor random(const vector<size_t>& shape, const double& min,const double& max);
    static Tensor arange(const int& start, const int& end);
    Tensor(const Tensor& other);
    Tensor& operator=(const Tensor& other);
    Tensor(Tensor&& other) noexcept;
};


#endif //TENSORES_TENSOR_H