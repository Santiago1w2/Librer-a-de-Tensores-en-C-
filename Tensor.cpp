//
// Created by smora on 3/04/2026.
//

#include "Tensor.h"
Tensor::Tensor(const vector<size_t>& shape, const vector<double>& values) {
    if (shape.size() > 3)
        throw invalid_argument("El tamaño maximo de dimnesiones tiene que ser 3");
    // Calcular tamaño total
    size_t total = 1;
    for (size_t s : shape) {
        total *= s;
    }
    if (values.size() != total) {
        throw invalid_argument("El tamanio de values no coincide con los requeridos por la dimension ingresada");
    }
    this->shape = shape;
    this->values = values;
}