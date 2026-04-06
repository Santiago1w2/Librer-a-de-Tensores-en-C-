//
// Created by smora on 3/04/2026.
//

#include "Tensor.h"
using namespace std;
//constructor general para destructor
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

    // Se reserva nueva memoria
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
    // Se crea la copia se va a modificar
    Tensor res = t;

    // Se usa size_t para que coincida con el retorno de res.size()
    size_t n = res.size();

    for (size_t i = 0; i < n; i++) {
        res[i] = max(0.0, res[i]);
    }
    cout << "Relu aplicado exitosamente\n";

    return res;
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

    //Asegurar 3 dimensiones
    vector<size_t> A = this->shape;
    vector<size_t> B = other.shape;

    while (A.size() < 3) A.insert(A.begin(), 1);
    while (B.size() < 3) B.insert(B.begin(), 1);

    //Shape resultado
    vector<size_t> R(3);
    for (int i = 0; i < 3; i++) {
        if (A[i] == B[i] || A[i] == 1 || B[i] == 1) {
            R[i] = max(A[i], B[i]);
        } else {
            throw std::invalid_argument("Shapes incompatibles suma");
        }
    }

    //Crear vector de datos
    size_t total = R[0] * R[1] * R[2];
    vector<double> resultData(total);

    size_t A1=A[0], A2=A[1], A3=A[2];
    size_t B1=B[0], B2=B[1], B3=B[2];
    size_t R1=R[0], R2=R[1], R3=R[2];

    for (size_t i = 0; i < R1; i++) {
        for (size_t j = 0; j < R2; j++) {
            for (size_t k = 0; k < R3; k++) {

                size_t iA = (A1 == 1) ? 0 : i;
                size_t jA = (A2 == 1) ? 0 : j;
                size_t kA = (A3 == 1) ? 0 : k;

                size_t iB = (B1 == 1) ? 0 : i;
                size_t jB = (B2 == 1) ? 0 : j;
                size_t kB = (B3 == 1) ? 0 : k;

                size_t idxA = iA*A2*A3 + jA*A3 + kA;
                size_t idxB = iB*B2*B3 + jB*B3 + kB;
                size_t idxR = i*R2*R3 + j*R3 + k;

                resultData[idxR] = this->matriz[idxA] + other.matriz[idxB];
            }
        }
    }

    //Construir tensor correctamente
    return Tensor(R,resultData);
}

Tensor Tensor::operator-(const Tensor& other) const {

    //Asegurar 3 dimensiones
    vector<size_t> A = this->shape;
    vector<size_t> B = other.shape;

    while (A.size() < 3) A.insert(A.begin(), 1);
    while (B.size() < 3) B.insert(B.begin(), 1);

    //Shape resultado
    vector<size_t> R(3);
    for (int i = 0; i < 3; i++) {
        if (A[i] == B[i] || A[i] == 1 || B[i] == 1) {
            R[i] = max(A[i], B[i]);
        } else {
            throw std::invalid_argument("Shapes incompatibles resta");
        }
    }

    //Crear vector de datos
    size_t total = R[0] * R[1] * R[2];
    vector<double> resultData(total);

    size_t A1=A[0], A2=A[1], A3=A[2];
    size_t B1=B[0], B2=B[1], B3=B[2];
    size_t R1=R[0], R2=R[1], R3=R[2];

    for (size_t i = 0; i < R1; i++) {
        for (size_t j = 0; j < R2; j++) {
            for (size_t k = 0; k < R3; k++) {

                size_t iA = (A1 == 1) ? 0 : i;
                size_t jA = (A2 == 1) ? 0 : j;
                size_t kA = (A3 == 1) ? 0 : k;

                size_t iB = (B1 == 1) ? 0 : i;
                size_t jB = (B2 == 1) ? 0 : j;
                size_t kB = (B3 == 1) ? 0 : k;

                size_t idxA = iA*A2*A3 + jA*A3 + kA;
                size_t idxB = iB*B2*B3 + jB*B3 + kB;
                size_t idxR = i*R2*R3 + j*R3 + k;

                resultData[idxR] = this->matriz[idxA] - other.matriz[idxB];
            }
        }
    }

    //Construir tensor correctamente
    return Tensor(R,resultData);
}
Tensor Tensor::operator*(const Tensor& other) const {

    //Asegurar 3 dimensiones
    vector<size_t> A = this->shape;
    vector<size_t> B = other.shape;

    while (A.size() < 3) A.insert(A.begin(), 1);
    while (B.size() < 3) B.insert(B.begin(), 1);

    //Shape resultado
    vector<size_t> R(3);
    for (int i = 0; i < 3; i++) {
        if (A[i] == B[i] || A[i] == 1 || B[i] == 1) {
            R[i] = max(A[i], B[i]);
        } else {
            throw std::invalid_argument("Shapes incompatibles multi");
        }
    }

    //Crear vector de datos
    size_t total = R[0] * R[1] * R[2];
    vector<double> resultData(total);

    size_t A1=A[0], A2=A[1], A3=A[2];
    size_t B1=B[0], B2=B[1], B3=B[2];
    size_t R1=R[0], R2=R[1], R3=R[2];

    for (size_t i = 0; i < R1; i++) {
        for (size_t j = 0; j < R2; j++) {
            for (size_t k = 0; k < R3; k++) {

                size_t iA = (A1 == 1) ? 0 : i;
                size_t jA = (A2 == 1) ? 0 : j;
                size_t kA = (A3 == 1) ? 0 : k;

                size_t iB = (B1 == 1) ? 0 : i;
                size_t jB = (B2 == 1) ? 0 : j;
                size_t kB = (B3 == 1) ? 0 : k;

                size_t idxA = iA*A2*A3 + jA*A3 + kA;
                size_t idxB = iB*B2*B3 + jB*B3 + kB;
                size_t idxR = i*R2*R3 + j*R3 + k;

                resultData[idxR] = this->matriz[idxA] * other.matriz[idxB];
            }
        }
    }

    //Construir tensor correctamente
    return Tensor(R,resultData);
}
Tensor Tensor::operator*(double num) const{
    Tensor m = *this;
    for (size_t i = 0; i < m.size(); i++) {
        m[i] *= num;
    }
    return m;
}


Tensor Tensor::view(const vector<size_t>& new_shape) const {
    // Validar máximo 3 dimensiones
    if (new_shape.size() > 3)
        throw invalid_argument("Máximo 3 dimensiones");

    size_t total = 1;
    for (auto d : new_shape) total *= d;

    // El número total de elementos debe mantenerse constante
    if (total != this->size())
        throw invalid_argument("Shape incompatible para view");

    // Crear una copia que comparta el puntero (o usar el constructor de movimiento)
    Tensor result = *this;
    result.shape = new_shape; // Actualiza a 1D, 2D o 3D según se pida

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



//Funciones Amigas

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

Tensor matmul(const Tensor& a, const Tensor& b) {
    // Extraer solo las dimensiones significativas (las que no son 1)
    vector<size_t> A, B;
    for (size_t s : a.shape)
        if (s > 1)
            A.push_back(s);
    for (size_t s : b.shape)
        if (s > 1)
            B.push_back(s);

    // Si es un vector de 1 dimención, convertirlo lógicamente a 2D para la multiplicación
    if (A.size() == 1) A.insert(A.begin(), 1);
    if (B.size() == 1) B.push_back(1);

    // Validar compatibilidad matricial
    if (A.size() != 2 || B.size() != 2) {
        throw invalid_argument("Se requieren dimensiones compatibles con 2D en el matmul.");
    }

    size_t M = A[0]; // Filas A
    size_t K = A[1]; // Columnas A
    size_t N = B[1]; // Columnas B

    if (K != B[0]) { // Columnas A deben ser igual a Filas B
        throw invalid_argument("Matmul: Dimensiones incompatibles (K != filas de B).");
    }

    Tensor res = Tensor::zeros({M, N});
    // Multiplicación
    for (size_t i = 0; i < M; ++i) {
        for (size_t k = 0; k < K; ++k) {
            for (size_t j = 0; j < N; ++j) {
                res.matriz[i * N + j] += a.matriz[i * K + k] * b.matriz[k * N + j];
            }
        }
    }
    return res;
}

void Tensor::print() const {
    //Verificar si las dimensiones estan vacias
    if (this->shape.empty()) {
        cout << "Tensor vacío" << endl;
        return;
    }

    // Caso 1D: [Dim0]
    if (this->shape.size() == 1) {
        cout << "[";
        for (size_t i = 0; i < shape[0]; ++i) {
            cout << matriz[i] << (i == shape[0] - 1 ? "" : ", ");
        }
        cout << "]" <<endl;
    }
    // Caso 2D: [Filas, Columnas]
    else if (this->shape.size() == 2) {
        size_t fil = shape[0];
        size_t cols = shape[1];
        for (size_t i = 0; i < fil; ++i) {
            if (i == 0)
                cout<<"[[";
            else
                cout<<" [";
            for (size_t j = 0; j < cols; ++j) {
                cout << matriz[i * cols + j] << (j == cols - 1 ? "" : ", ");
            }
            if (i == fil-1)
                cout<<"]]";
            else
                cout<<"]\n";
        }
        cout << endl;
    }
    // Caso 3D: [Capas, Filas, Columnas]
    else if (this->shape.size() == 3) {
        size_t capa = shape[0];
        size_t fil = shape[1];
        size_t cols = shape[2];

        for (size_t l = 0; l < capa; ++l) {
            cout << "Capa " << l << ":" << endl;
            for (size_t i = 0; i < fil; ++i) {
                if (i == 0)
                    cout <<" [[";
                else
                    cout<<"  [";
                for (size_t j = 0; j < cols; ++j) {
                    // El indice se calcula como: (capa * filas * columnas) + (fila * columnas) + columna
                    size_t idx = (l * fil * cols) + (i * cols) + j;
                    if (j == cols - 1)
                        cout << matriz[idx]<<"";
                    else
                        cout << matriz[idx]<<" ";
                }
                if (i == fil - 1)
                    cout<<"]]";
                else
                    cout<<"]\n";
            }
            cout << "\n" <<endl;
        }
    }
}