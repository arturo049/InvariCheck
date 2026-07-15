#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

// ============================================================
// Tipo de estructura soportada
// ============================================================
enum class StructureType { LIST, STACK, QUEUE };

inline std::string structureTypeToString(StructureType t) {
    switch (t) {
        case StructureType::LIST:  return "LISTA";
        case StructureType::STACK: return "PILA";
        case StructureType::QUEUE: return "COLA";
    }
    return "DESCONOCIDA";
}

// ============================================================
// Clase DataStructure
// ------------------------------------------------------------
// Modela internamente la estructura como un vector<int> y
// restringe las operaciones permitidas segun el tipo elegido
// (lista, pila o cola), respetando su semantica natural:
//   - LISTA: insertar/eliminar en cualquier posicion, consultar
//   - PILA : insertar (push) al final, eliminar (pop) al final
//   - COLA : insertar (enqueue) al final, eliminar (dequeue) al inicio
// ============================================================
class DataStructure {
public:
    explicit DataStructure(StructureType t) : type(t) {}

    StructureType getType() const { return type; }

    // Inserta un valor. Para LISTA se puede indicar posicion (-1 = al final).
    void insert(int value, int position = -1) {
        if (type == StructureType::LIST) {
            if (position < 0 || position > static_cast<int>(data.size()))
                data.push_back(value);
            else
                data.insert(data.begin() + position, value);
        } else {
            // PILA y COLA siempre insertan al final
            data.push_back(value);
        }
    }

    // Elimina un valor. Para LISTA se puede indicar posicion (-1 = al final).
    int remove(int position = -1) {
        if (data.empty()) throw std::runtime_error("Estructura vacia: no se puede eliminar");

        if (type == StructureType::LIST) {
            int idx = (position < 0 || position >= static_cast<int>(data.size()))
                        ? static_cast<int>(data.size()) - 1 : position;
            int val = data[idx];
            data.erase(data.begin() + idx);
            return val;
        } else if (type == StructureType::STACK) {
            int val = data.back();
            data.pop_back();
            return val;
        } else { // QUEUE
            int val = data.front();
            data.erase(data.begin());
            return val;
        }
    }

    // Consulta (no modifica el estado): tope/frente segun tipo, o por posicion en LISTA.
    int query(int position = -1) const {
        if (data.empty()) throw std::runtime_error("Estructura vacia: no se puede consultar");
        if (type == StructureType::LIST) {
            int idx = (position < 0 || position >= static_cast<int>(data.size()))
                        ? static_cast<int>(data.size()) - 1 : position;
            return data[idx];
        } else if (type == StructureType::STACK) {
            return data.back();
        } else {
            return data.front();
        }
    }

    void clear() { data.clear(); }

    const std::vector<int>& getState() const { return data; }

    std::string stateToString() const {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < data.size(); ++i) {
            oss << data[i];
            if (i + 1 < data.size()) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

private:
    StructureType type;
    std::vector<int> data;
};

#endif // DATA_STRUCTURE_H
