# InvariCheck

**Verificador de Invariantes en Estructuras de Datos**
Proyecto del curso de Métodos de Software — Categorías: Métodos formales, Verificación, Invariantes, C++.

## 1. Descripción

InvariCheck es una aplicación de consola en C++ que permite:

1. Elegir un tipo de estructura de datos a verificar: **lista**, **pila** o **cola**.
2. Definir sus **invariantes formales** mediante un mini-lenguaje textual estructurado.
3. Ejecutar **operaciones** (insertar, eliminar, consultar) respetando la semántica propia de cada estructura.
4. **Verificar automáticamente** todos los invariantes después de cada operación.
5. Registrar el resultado de cada verificación (**CUMPLIDA** / **VIOLADA**).
6. Visualizar el **estado actual** de la estructura tras cada operación.
7. Mantener un **historial completo** de operaciones y resultados.
8. Reportar la **operación que causó la primera violación** de un invariante.
9. **Reiniciar** la estructura conservando los invariantes definidos.
10. **Exportar** el historial completo a un archivo de texto (`.txt`).

## 2. Lenguaje de invariantes

Cada invariante se escribe en una línea, con una sintaxis fija y validada al momento de definirla:

| Invariante              | Significado                                          |
|--------------------------|------------------------------------------------------|
| `SIZE <= N`              | El tamaño de la estructura no debe superar N         |
| `SIZE >= N`              | El tamaño debe ser al menos N                        |
| `SIZE == N`              | El tamaño debe ser exactamente N                      |
| `NO_DUPLICATES`          | No deben existir elementos repetidos                 |
| `SORTED_ASC`             | Los elementos deben estar en orden ascendente        |
| `SORTED_DESC`            | Los elementos deben estar en orden descendente       |
| `ALL_POSITIVE`           | Todos los elementos deben ser > 0                    |
| `ALL_NEGATIVE`           | Todos los elementos deben ser < 0                    |
| `NOT_EMPTY`              | La estructura nunca debe quedar vacía                |
| `MAX_VALUE <= N`         | El valor máximo presente no debe superar N           |
| `MIN_VALUE >= N`         | El valor mínimo presente no debe ser menor que N      |

Ejemplo de definición de invariantes para una pila de capacidad acotada:

```
SIZE <= 5
ALL_POSITIVE
MAX_VALUE <= 1000
```

Ver también `docs/example_invariants.txt`.

## 3. Estructura del proyecto

```
InvariCheck/
├── include/
│   ├── DataStructure.h   # Modela lista/pila/cola y sus operaciones permitidas
│   ├── Invariant.h       # Parseo y evaluación de invariantes formales
│   ├── Verifier.h        # Orquesta estructura + invariantes + logger
│   └── Logger.h          # Historial de operaciones, primera violación, exportación
├── src/
│   └── main.cpp          # Interfaz de consola (menús)
├── docs/
│   └── example_invariants.txt
├── Makefile
└── README.md
```

### Diseño (resumen)

- **`DataStructure`**: representa internamente la estructura como `std::vector<int>`, pero **restringe** las operaciones según el tipo (p. ej. en una cola solo se puede eliminar por el frente; en una pila solo por el tope).
- **`Invariant`**: parsea una línea de texto a un tipo enumerado + parámetro, y sabe evaluarse contra un estado (`evaluate(state)`), sin acoplarse a la estructura concreta.
- **`Verifier`**: después de cada operación, evalúa **todos** los invariantes contra el estado resultante y construye una entrada de historial (`LogEntry`).
- **`Logger`**: acumula las entradas, detecta automáticamente la **primera** violación, imprime en consola y exporta a archivo de texto.

Esta separación permite, por ejemplo, añadir un nuevo tipo de invariante (`Invariant.h`) o un nuevo tipo de estructura (`DataStructure.h`) sin tocar la lógica de verificación (`Verifier.h`).

## 4. Compilación

Requiere `g++` con soporte C++17.

```bash
make            # compila el binario "invaricheck"
make run        # compila y ejecuta
make clean      # limpia binarios y archivos .txt generados
```

O manualmente:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude -o invaricheck src/main.cpp
./invaricheck
```

## 5. Uso típico

1. Ejecutar `./invaricheck`.
2. Elegir el tipo de estructura (1 = lista, 2 = pila, 3 = cola).
3. Escribir los invariantes, uno por línea (línea vacía para terminar).
4. Usar el menú para insertar / eliminar / consultar valores.
5. Tras cada operación se imprime el estado y el resultado de cada invariante.
6. Usar la opción **6** para ver la primera violación detectada (si existe).
7. Usar la opción **8** para exportar todo el historial a un `.txt`.
8. Usar la opción **7** para reiniciar la estructura sin perder los invariantes definidos.

## 6. Control de versiones

El proyecto está pensado para versionarse con Git desde el inicio:

```bash
git init
git add .
git commit -m "InvariCheck: version inicial funcional"
```

Se recomienda usar ramas por funcionalidad (`feature/invariantes`, `feature/exportacion`, etc.) y comparar la evolución del verificador con `git log`/`git diff` como parte del análisis de métodos de software.
