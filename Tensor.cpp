//
// Created by smora on 3/04/2026.
//

#include "Tensor.h"
using namespace std;
Tensor::Tensor(double* data, const vector<size_t>& shape)
    : matriz(data), shape(shape), eliminar(false) {}
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

//Sobre carga requerida pra trabajar con las clases heredadas
size_t Tensor::size() const {
    size_t total = 1;
    for (size_t m : this->shape) {
        total *= m;
    }
    return total;
}
double& Tensor::operator[](size_t i) {
    return matriz[i];
}

const double& Tensor::operator[](size_t i) const {
    return matriz[i];
}


// Prototipos requeridos en la clase Tensor
//Constructor de copia
Tensor::Tensor(const Tensor& other) {
    // 1. Copiar metadatos
    this->shape = other.shape;
    this->eliminar = true;

    // 2. Calcular tamaño necesario
    size_t total_size = 1;
    for (size_t dim : other.shape) {
        total_size *= dim;
    }

    // 3. RESERVAR NUEVA MEMORIA (Esto evita el crash)
    this->matriz = new double[total_size];

    // 4. Copiar los valores uno a uno
    for (size_t i = 0; i < total_size; ++i) {
        this->matriz[i] = other.matriz[i];
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
//Destrcutor
Tensor::~Tensor() {
    if (eliminar)
        delete[] matriz;
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

Tensor Tensor::apply(const TensorTransform& transform) const {
    return transform.apply(*this);
}


//Clase heradada ReLU
Tensor ReLU::apply(const Tensor& t) const {
    // 1. Creamos la copia que vamos a modificar
    Tensor res = t;

    // 2. Usamos size_t para que coincida con el retorno de res.size()
    size_t n = res.size();

    for (size_t i = 0; i < n; i++) {
        // 3. Usamos std::max explícitamente
        // Nota: Asegúrate de que res[i] devuelva una referencia (double&)
        res[i] = std::max(0.0, res[i]);
    }
    cout << "Relu aplicado exitosamente\n";

    return res; // El compilador aplicará RVO (Return Value Optimization)
}

//Clase heradada Sigmoid
Tensor Sigmoid::apply(const Tensor& t) const {
    Tensor result = t;
    for (size_t i = 0; i < result.size(); i++) {
        result[i] = 1.0 / (1.0 + exp(-result[i]));
    }
    return result;
}


//Sobrecarga de operadores
Tensor Tensor::operator+(const Tensor& other) const {
    // 1. Verificación estricta
    if (this->shape != other.shape) {
        throw std::invalid_argument("Error: Los tensores deben tener el mismo shape para sumarse.");
    }

    // 2. Crear el tensor resultado (usando el constructor de copia profunda)
    Tensor resultado(*this);

    // 3. Operar
    size_t n = resultado.size();
    for (size_t i = 0; i < n; i++) {
        resultado[i] += other[i];
    }

    // 4. Retornar por valor (esto activará el movimiento si está implementado)
    return resultado;
}

Tensor Tensor::operator-(const Tensor& other) const {
    if (shape != other.shape) {
        throw invalid_argument("El shape no es incompatible entre ambos tensores");
    }
    Tensor m = *this;
    for (size_t i = 0; i < m.size(); i++) {
        m[i] -= other[i];
    }
    return m;
}
Tensor Tensor::operator*(const Tensor& other) const {
    if (shape != other.shape) {
        throw invalid_argument("El shape es incompatible entre ambos tensores");
    }
    Tensor m = *this;
    for (size_t i = 0; i < m.size(); i++) {
        m[i] *= other[i];
    }
    return m;
}
Tensor Tensor::operator*(double num) const{
    Tensor m = *this;
    for (size_t i = 0; i < m.size(); i++) {
        m[i] *= num;
    }
    return m;
}


Tensor Tensor::view(const vector<size_t>& new_shape) const {
    if (shape.size() > 3)
        throw invalid_argument("El tamaño maximo de dimnesiones tiene que ser 3");

    size_t total_values = 1;
    for (size_t m : new_shape) {
        total_values *= m;
    }
    if (size() != total_values) {
        throw invalid_argument("El shape es incompatible entre ambos tensores");
    }
    Tensor result(matriz, new_shape);

    return result;
}
//concatenacion

Tensor Tensor::concat(const std::vector<Tensor>& tensors, int axis) {
    //throw bota error aparentemente
    //hubiera puesto return pero no se si retornar un tensor vacio sea lo ideal

    if (tensors.empty()) {
        throw invalid_argument("Concat: La lista de tensores está vacía.");
    }
    // Validar dimensiones base y preparar el nuevo shape
    vector<size_t> first_shape = tensors[0].shape;
    size_t dims = first_shape.size();

    if (axis < 0) axis += dims;
    if (axis < 0 || axis >= dims) {

        throw out_of_range("Concat: Eje (axis) fuera de rango.");
    }
    vector<size_t> new_shape = first_shape;
    size_t total_concat_dim = 0;

    for (const auto& t : tensors) {
        if (t.shape.size() != dims) {
            throw invalid_argument("Concat: Todos los tensores deben tener el mismo número de dimensiones.");
        }
        for (size_t i = 0; i < dims; ++i) {
            if (i != (size_t)axis && t.shape[i] != first_shape[i]) {
                throw invalid_argument("Concat: Dimensiones incompatibles en ejes distintos al de concatenación.");
            }
        }
        total_concat_dim += t.shape[axis];
    }
    new_shape[axis] = total_concat_dim;

    //Reservar memoria para el nuevo tensor
    Tensor result = zeros(new_shape);
    // Suponiendo que zeros reserva memoria y pone eliminar = true

    //Copia controlada de datos
    /* Lógica: Dividimos el tensor en tres partes:
       - 'outer': bloques antes del eje de concatenación.
       - 'axis': el eje que estamos uniendo.
       - 'inner': bloques después del eje (los datos contiguos).
    */
    size_t outer_size = 1;
    for (int i = 0; i < axis; ++i) outer_size *= new_shape[i];

    size_t inner_size = 1;
    for (size_t i = axis + 1; i < dims; ++i) inner_size *= new_shape[i];

    size_t current_axis_offset = 0;
    for (const auto& t : tensors) {
        size_t t_axis_dim = t.shape[axis];

        for (size_t o = 0; o < outer_size; ++o) {
            // Calculamos el origen en el tensor pequeño y el destino en el grande
            double* dest = &result.matriz[(o * total_concat_dim + current_axis_offset) * inner_size];
            const double* src = &t.matriz[o * t_axis_dim * inner_size];

            // Copiamos el bloque contiguo de memoria
            std::copy(src, src + (t_axis_dim * inner_size), dest);
        }
        current_axis_offset += t_axis_dim;
    }

    //Devolver usando move (C++ lo hace automático al retornar un objeto local)
    return result;
}



//FUNCIONES AMIGAS

Tensor dot ( const Tensor & a , const Tensor & b ) {
    if (a.shape != b.shape) {
        throw std::invalid_argument("Dot product: Los tensores deben tener el mismo shape.");
    }
    double suma = 0.0;
    size_t n = a.size();
    // Acceso directo a la memoria interna (posible por ser friend)
    for (size_t i = 0; i < n; ++i) {
        suma += a.matriz[i] * b.matriz[i];
    }
    // Devolvemos un tensor de un solo elemento
    return Tensor({1}, {suma});
}

Tensor matmul ( const Tensor & a , const Tensor & b ) {
    // 1. Validar que sean bidimensionales
    if (a.shape.size() != 2 || b.shape.size() != 2) {
        throw std::invalid_argument("Matmul: Ambos tensores deben ser 2D.");
    }
    // 2. Validar compatibilidad (Columnas de A == Filas de B)
    size_t m = a.shape[0];
    size_t n = a.shape[1];
    size_t p = b.shape[1];
    if (n != b.shape[0]) {
        throw std::invalid_argument("Matmul: Dimensiones incompatibles para multiplicacion.");
    }
    // 3. Crear tensor resultado (inicializado en ceros)
    Tensor res = Tensor::zeros({m, p});
    // 4. Multiplicación (Algoritmo i, j, k)
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < p; ++j) {
            double temp = 0.0;
            for (size_t k = 0; k < n; ++k) {
                // Acceso manual al índice: fila * total_columnas + columna
                temp += a.matriz[i * n + k] * b.matriz[k * p + j];
            }
            res.matriz[i * p + j] = temp;
        }
    }
    // 5. Devolver por valor (C++ activará el Constructor de Movimiento automáticamente)
    return res;
}



/*
//Metodo display para mostrar matrices y probarlas
//NO FUNCIONA - SE LOQUEA CON LA MEMORIA JAJA

void Tensor::display() {
    if (matriz == nullptr) {
        std::cout << "Tensor vacío" << std::endl;
        return;
    }

    size_t total_elements = size();
    int dims = shape.size();

    std::cout << "Tensor (shape: [";
    for (size_t i = 0; i < shape.size(); ++i) {
        std::cout << shape[i] << (i == shape.size() - 1 ? "" : ", ");
    }
    std::cout << "])" << std::endl;

    // Caso especial: Escalar o vector de 1D
    if (dims == 1) {
        std::cout << "[";
        for (size_t i = 0; i < shape[0]; ++i) {
            std::cout << matriz[i] << (i == shape[0] - 1 ? "" : ", ");
        }
        std::cout << "]" << std::endl;
        return;
    }

    // Para 2D o más, calculamos pasos (strides) para navegar el arreglo plano
    for (size_t i = 0; i < total_elements; ++i) {
        // Lógica de apertura de corchetes
        size_t temp_idx = i;
        size_t stride = total_elements;
        for (int d = 0; d < dims; ++d) {
            stride /= shape[d];
            if (temp_idx % stride == 0) {
                std::cout << "[";
            }
        }

        // Imprimir el valor con formato
        std::cout << std::setw(8) << matriz[i];

        // Lógica de cierre de corchetes y comas
        temp_idx = i + 1;
        stride = 1;
        bool closed = false;
        for (int d = dims - 1; d >= 0; --d) {
            if (temp_idx % (stride * shape[d]) == 0) {
                std::cout << "]";
                stride *= shape[d];
                closed = true;
            } else {
                if (closed) std::cout << "\n";
                else if (i < total_elements - 1) std::cout << ", ";
                break;
            }
        }
    }
    std::cout << std::endl;
}

*/