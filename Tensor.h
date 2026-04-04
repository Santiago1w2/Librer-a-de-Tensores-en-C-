//
// Created by smora on 3/04/2026.
//
#include <iostream>
#include <vector>

using namespace std;


#ifndef TENSORES_TENSOR_H
#define TENSORES_TENSOR_H
class TensorTransform;

class Tensor {

    double* matriz;
    vector <size_t> shape;
public:
    //Getters
     vector<size_t> get_shape() const {
        return shape;
    }

    //size_t es un tipo de dato que se ua para tamaños o indices o posiciones de memoria
    //Solo toma valores positovs y cero.
    Tensor(const vector<size_t>& shape, const vector<double>& values);
    static Tensor zeros(const vector<size_t>& shape);
    static Tensor ones(const vector<size_t>& shape);
    static Tensor random(const vector<size_t>& shape, const double& min,const double& max);
    static Tensor arange(const int& start, const int& end);

    // Prototipos requeridos en la clase Tensor
    Tensor(const Tensor& other);
    Tensor& operator=(const Tensor& other);
    Tensor(Tensor&& other) noexcept;
    Tensor& operator=(Tensor&& other) noexcept;
    ~Tensor();
    //metodo apply
    Tensor apply(const TensorTransform& transform) const;

};

class TensorTransform {
    public:
    virtual Tensor apply(const Tensor& t) const = 0;
    virtual ~TensorTransform() = default;
};

class ReLu : public TensorTransform {
    Tensor apply(const Tensor &t) const override {
        vector<double> values;
        for (size_t m: t.get_shape()) {

        }
    }

}

class Sigmoid : public TensorTransform {
    Tensor apply(const Tensor &t) const override {
        for (size_t m: t.get_shape()) {

        }
    }
}


#endif //TENSORES_TENSOR_H