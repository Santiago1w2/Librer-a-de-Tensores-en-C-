# Tensor & Neural Framework C++ 
> Librearía de **Tensores** y componentes para **Redes Neuronales** escrita en C++ para su verión 11 o superiores. 
---
## Características Principales

* **Compatibilidad Multidimensional:** Manejo de tensores de hasta 3 dimensiones con redimensionamiento dinámico mediante `view`.
* **Gestión de Memoria Robusta:** Implementación completa de la *Rule of Five* (constructor de copia, movimiento y operadores de asignación) para evitar fugas de memoria.
* **Operaciones Matemáticas:**
    * Aritmética básica: Suma (`+`), Resta (`-`), Multiplicación elemento a elemento (`*`) y por escalar.
    * Álgebra lineal: Producto punto (`dot`) y multiplicación de matrices (`matmul`).
* **Transformaciones No Lineales:** Sistema basado en polimorfismo para aplicar funciones de activación como **ReLU** y **Sigmoid**.
* **Utilidades de Inicialización:** Generación de tensores aleatorios, de ceros, de unos y rangos secuenciales (`arange`).

---
## Instrucciones de aplicación
* Incluir el archivo `Tensor.h` para utilizar los métodos de Tensor y Framework neuronal.
     * El header utiliza por defecto las librerías `iostream`, `vector`, `iomanip` y `cmath` , así como utiliza por defecto el namespace `std`.
     * La librería utiliza
* El archivo `main.cpp` tiene, por defecto, un pipeline de prueba que emplea todos los métodos necesarios para ejecutar una red neuronal, en base a datos aleatorios.
   *Además de ello, ('main.cpp) posee otras pruebas individuales de los métodos y operaciones básicos de la clase Tensor, listos para la edición y ejecució por el usuario.
---
## Notas de Desarrollo
* La clase Tensor tienen una variable (`bool eliminar`, utilizada como flag para destruir el objeto al realizar ciertas operaciones.
---
## Detalles técnicos y documentación
### Estructura del Código

El proyecto se estructura de la siguiente forma:
* `Tensor.h` / `Tensor.cpp`: La clase núcleo que gestiona el almacenamiento en un arreglo plano de `double*` para maximizar la contigüidad en memoria.
* `TensorTransform`: Una interfaz abstracta que permite extender el framework con nuevas funciones de activación mediante herencia.
* `main.cpp`: Un pipeline de ejemplo que simula el flujo de datos: Transformación -> Matmul -> Bias -> Activación.

### Métodos y operadores
#### Tensor
* `Tensor::Tensor(double* data, const vector<size_t>& shape)`: Constructor que recibe un puntero a un arreglo de doubles ya existente y un vector con las dimensiones, inicializando el tensor sin reclamar la propiedad de la memoria inicialmente.

* `Tensor::Tensor(const vector<size_t>& shape, const vector<double>& values)`: Constructor que crea un tensor a partir de una forma específica y un vector de valores, validando que el tamaño total coincida con las dimensiones proporcionadas.

* `Tensor::Tensor(const Tensor& other)`: Constructor de copia que realiza una copia profunda de los datos y metadatos de otro tensor para asegurar independencia de memoria.

* `Tensor::Tensor(Tensor&& other) noexcept`: Constructor de movimiento que transfiere la propiedad de los recursos de un tensor temporal a uno nuevo, optimizando el rendimiento al evitar copias innecesarias.

* `Tensor::~Tensor()`: Destructor encargado de liberar la memoria dinámica del arreglo matriz si la bandera eliminar está activa.

* `size_t Tensor::size() const`: Método que calcula y retorna el número total de elementos contenidos en el tensor multiplicando todas sus dimensiones.

* `static Tensor Tensor::zeros(const vector<size_t>& shape)`: Método estático que crea y retorna un nuevo tensor inicializado completamente con ceros.

* `static Tensor Tensor::ones(const vector<size_t>& shape)`: Método estático que genera un tensor donde todos sus elementos tienen el valor de 1.0.

* `static Tensor Tensor::random(const vector<size_t>& shape, const double& min, const double& max)`: Método estático que llena un tensor con valores aleatorios dentro de un rango definido por el usuario.

* `static Tensor Tensor::arange(const int& start, const int& end)`: Método estático que crea un tensor unidimensional con una secuencia numérica desde un valor inicial hasta uno final.

* `Tensor Tensor::view(const vector<size_t>& new_shape) const`: Método que permite cambiar la interpretación de las dimensiones del tensor sin modificar ni copiar los datos subyacentes en memoria.

* `Tensor Tensor::apply(const TensorTransform& transform) const`: Método que permite aplicar una transformación (como funciones de activación) utilizando polimorfismo a través de la interfaz TensorTransform.

* `static Tensor Tensor::concat(const std::vector<Tensor>& tensors, int axis)`: Método estático que une múltiples tensores a lo largo de un eje específico, gestionando el reacomodo de los datos en un nuevo bloque de memoria.

* `void Tensor::print() const`: Método de visualización que detecta automáticamente si el tensor es 1D, 2D o 3D, formateando la salida con corchetes anidados y etiquetas de "Capa" para el caso tridimensional.

##### Operadores Sobrecargados
* `double& Tensor::operator[](size_t i)`: Sobrecarga del operador de índice para permitir el acceso y modificación directa de los elementos en el arreglo plano.

* `Tensor Tensor::operator+(const Tensor& other) const`: Realiza la suma elemento a elemento entre dos tensores del mismo tamaño.

* `Tensor Tensor::operator*(double num) const`: Realiza la multiplicación de todos los elementos del tensor por un valor escalar de tipo double.

##### Funciones Amigas (Friend Functions)
* `Tensor dot(const Tensor& a, const Tensor& b)`: Calcula el producto punto entre dos tensores, retornando un tensor de un solo elemento con el resultado escalar.

* `Tensor matmul(const Tensor& a, const Tensor& b)`: Retorna un Tensor resultante de la multiplicación matricial 2D. Incluye una lógica de pre-procesamiento que ignora dimensiones triviales (de tamaño 1) para tratar tensores 3D como matrices si es posible.

#### TensorTransform
* `virtual Tensor TensorTransform::apply(const Tensor& t) const = 0`: Método virtual puro que establece el contrato obligatorio para que las clases derivadas implementen su propia lógica de transformación sobre un objeto Tensor.

* `virtual TensorTransform::~TensorTransform() = default`: Destructor virtual por defecto que asegura una limpieza correcta de la memoria cuando se destruyen objetos de clases derivadas a través de un puntero de la clase base.

* `class ReLU : public TensorTransform`: Clase que hereda de TensorTransform para implementar la unidad lineal rectificada (Rectified Linear Unit), una de las funciones de activación más utilizadas en redes neuronales para introducir no linealidad.

* `Tensor ReLU::apply(const Tensor& t) const`: Implementación que recorre el tensor y aplica la función $`f(x) = \max(0, x)`$ a cada elemento; utiliza una copia del tensor original para devolver los resultados, garantizando que los valores negativos se conviertan en cero.

* `class Sigmoid : public TensorTransform`: Clase que hereda de TensorTransform para implementar la función de activación sigmoide (logística), útil para normalizar salidas en un rango entre 0 y 1.

* `Tensor Sigmoid::apply(const Tensor& t) const`: Implementación que aplica la fórmula matemática $`\sigma(x) = \frac{1}{1 + e^{-x}}`$ sobre cada elemento del tensor utilizando la función exponencial de la librería estándar, retornando un nuevo tensor transformado.
----
## Créditos
* Código hecho por Santiago Morales y Gabriel Díaz para el curso de Programación III, UTEC.
* Parte de la descripción se hizo con el apoyo de herramientas de IA generativas, revisadas cuidadosamente por los autores.

