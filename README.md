# 🧠 C++ Tensor & Neural Framework

Una implementación ligera y eficiente de una biblioteca de **Tensores** y componentes de **Redes Neuronales** escrita en C++ puro. Este proyecto destaca por su gestión manual de memoria y el uso de semántica de movimiento para optimizar el rendimiento en operaciones de álgebra lineal.

## 🚀 Características Principales

* **Soporte Multidimensional:** Manejo de tensores de hasta 3 dimensiones con redimensionamiento dinámico mediante `view`.
* **Gestión de Memoria Robusta:** Implementación completa de la *Rule of Five* (constructor de copia, movimiento y operadores de asignación) para evitar fugas de memoria.
* **Operaciones Matemáticas:**
    * Aritmética básica: Suma (`+`), Resta (`-`), Multiplicación elemento a elemento (`*`) y por escalar.
    * Álgebra lineal: Producto punto (`dot`) y multiplicación de matrices (`matmul`).
* **Transformaciones No Lineales:** Sistema basado en polimorfismo para aplicar funciones de activación como **ReLU** y **Sigmoid**.
* **Utilidades de Inicialización:** Generación de tensores aleatorios, de ceros, de unos y rangos secuenciales (`arange`).

---

## 🛠 Estructura del Código

El corazón del proyecto se divide en:
* `Tensor.h` / `Tensor.cpp`: La clase núcleo que gestiona el almacenamiento en un arreglo plano de `double*` para maximizar la contigüidad en memoria.
* `TensorTransform`: Una interfaz abstracta que permite extender el framework con nuevas funciones de activación mediante herencia.
* `main.cpp`: Un pipeline de ejemplo que simula el flujo de datos: Transformación -> Matmul -> Bias -> Activación.

---

## 💻 Ejemplo de Uso (Pipeline de Red Neuronal)

El siguiente ejemplo (basado en el código de prueba) muestra cómo procesar datos a través de una estructura de red:

```cpp
// 1. Crear un tensor de entrada (100x20x20)
Tensor tensorEntrada = Tensor::random({100, 20, 20}, 0.0, 4.0);

// 2. Aplanarlo a 1000x400 usando view
Tensor transformado = tensorEntrada.view({1000, 400});

// 3. Multiplicación de matriz (Capa Dense)
Tensor mat1 = Tensor::random({400, 100}, 0.0, 4.0);
Tensor multiplicado = matmul(transformado, mat1);

// 4. Aplicar la función de activación ReLU
ReLU re_lu;
Tensor tensorReLu = multiplicado.apply(re_lu);
