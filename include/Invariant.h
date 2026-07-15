#ifndef INVARIANT_H
#define INVARIANT_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

// ============================================================
// Clase Invariant
// ------------------------------------------------------------
// Representa un invariante formal escrito en un mini-lenguaje
// textual estructurado, por ejemplo:
//
//   SIZE <= 10
//   NO_DUPLICATES
//   SORTED_ASC
//   SORTED_DESC
//   ALL_POSITIVE
//   ALL_NEGATIVE
//   NOT_EMPTY
//   MAX_VALUE <= 100
//   MIN_VALUE >= 0
//
// Cada invariante se parsea una sola vez (al definirlo) y se
// evalúa contra el estado actual de la estructura (vector<int>)
// cada vez que se llama a evaluate().
// ============================================================
class Invariant {
public:
    enum class Type {
        SIZE_LE,
        SIZE_GE,
        SIZE_EQ,
        NO_DUPLICATES,
        SORTED_ASC,
        SORTED_DESC,
        ALL_POSITIVE,
        ALL_NEGATIVE,
        NOT_EMPTY,
        MAX_VALUE_LE,
        MIN_VALUE_GE,
        UNKNOWN
    };

    Invariant() = default;

    // Construye el invariante a partir de una línea de texto.
    // Lanza std::invalid_argument si la sintaxis no es reconocida.
    explicit Invariant(const std::string& rawLine) {
        original = rawLine;
        parse(rawLine);
    }

    // Evalúa el invariante contra el estado actual de la estructura.
    bool evaluate(const std::vector<int>& state) const {
        switch (type) {
            case Type::SIZE_LE:
                return static_cast<int>(state.size()) <= intValue;
            case Type::SIZE_GE:
                return static_cast<int>(state.size()) >= intValue;
            case Type::SIZE_EQ:
                return static_cast<int>(state.size()) == intValue;
            case Type::NO_DUPLICATES: {
                std::set<int> s(state.begin(), state.end());
                return s.size() == state.size();
            }
            case Type::SORTED_ASC:
                return std::is_sorted(state.begin(), state.end());
            case Type::SORTED_DESC:
                return std::is_sorted(state.begin(), state.end(), std::greater<int>());
            case Type::ALL_POSITIVE:
                return std::all_of(state.begin(), state.end(), [](int v){ return v > 0; });
            case Type::ALL_NEGATIVE:
                return std::all_of(state.begin(), state.end(), [](int v){ return v < 0; });
            case Type::NOT_EMPTY:
                return !state.empty();
            case Type::MAX_VALUE_LE:
                if (state.empty()) return true; // vacuamente verdadero
                return *std::max_element(state.begin(), state.end()) <= intValue;
            case Type::MIN_VALUE_GE:
                if (state.empty()) return true;
                return *std::min_element(state.begin(), state.end()) >= intValue;
            default:
                return false;
        }
    }

    const std::string& getOriginalText() const { return original; }

private:
    Type type = Type::UNKNOWN;
    int intValue = 0;
    std::string original;

    static std::string trim(const std::string& s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == std::string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    void parse(const std::string& rawLine) {
        std::string line = trim(rawLine);
        if (line.empty()) throw std::invalid_argument("Linea de invariante vacia");

        std::istringstream iss(line);
        std::string token;
        iss >> token;
        std::transform(token.begin(), token.end(), token.begin(), ::toupper);

        if (token == "NO_DUPLICATES") { type = Type::NO_DUPLICATES; return; }
        if (token == "SORTED_ASC")    { type = Type::SORTED_ASC; return; }
        if (token == "SORTED_DESC")   { type = Type::SORTED_DESC; return; }
        if (token == "ALL_POSITIVE")  { type = Type::ALL_POSITIVE; return; }
        if (token == "ALL_NEGATIVE")  { type = Type::ALL_NEGATIVE; return; }
        if (token == "NOT_EMPTY")     { type = Type::NOT_EMPTY; return; }

        if (token == "SIZE" || token == "MAX_VALUE" || token == "MIN_VALUE") {
            std::string op;
            int val;
            iss >> op >> val;
            if (op.empty()) throw std::invalid_argument("Falta operador en: " + line);

            if (token == "SIZE") {
                if (op == "<=") type = Type::SIZE_LE;
                else if (op == ">=") type = Type::SIZE_GE;
                else if (op == "==" || op == "=") type = Type::SIZE_EQ;
                else throw std::invalid_argument("Operador invalido para SIZE: " + op);
            } else if (token == "MAX_VALUE") {
                if (op != "<=") throw std::invalid_argument("MAX_VALUE solo soporta '<='");
                type = Type::MAX_VALUE_LE;
            } else { // MIN_VALUE
                if (op != ">=") throw std::invalid_argument("MIN_VALUE solo soporta '>='");
                type = Type::MIN_VALUE_GE;
            }
            intValue = val;
            return;
        }

        throw std::invalid_argument("Invariante no reconocido: " + line);
    }
};

#endif // INVARIANT_H
