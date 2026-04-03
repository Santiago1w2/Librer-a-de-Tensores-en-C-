//
// Created by smora on 3/04/2026.
//
#include <iostream>
#include <vector>
using namespace std;


#ifndef TENSORES_TENSOR_H
#define TENSORES_TENSOR_H


class Tensor {
    double*** Matriz;
    vector <size_t> shape;
    vector <double> values;
public:
    //size_t es un tipo de dato que se ua para tamaños o indices o posiciones de memoria
    //Solo toma valores positovs y cero.
    Tensor(const vector<size_t>& shape, const vector<double>& values);

};


#endif //TENSORES_TENSOR_H