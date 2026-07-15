#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>

// ============================================================
// Estructura que representa una entrada del historial:
// una operacion ejecutada + el resultado de verificar todos
// los invariantes definidos en ese momento.
// ============================================================
struct LogEntry {
    int sequence;
    std::string timestamp;
    std::string operationDescription;
    std::string stateAfter;
    std::vector<std::pair<std::string,bool>> invariantResults; // texto invariante -> cumplido/violado
    bool anyViolation;
};

// ============================================================
// Clase Logger
// ------------------------------------------------------------
// Mantiene el historial de operaciones, detecta y reporta la
// primera violacion ocurrida, y permite exportar todo a un
// archivo de texto estructurado.
// ============================================================
class Logger {
public:
    void addEntry(const LogEntry& entry) {
        history.push_back(entry);
        if (entry.anyViolation && firstViolationIndex == -1) {
            firstViolationIndex = static_cast<int>(history.size()) - 1;
        }
    }

    void clear() {
        history.clear();
        firstViolationIndex = -1;
    }

    bool hasViolation() const { return firstViolationIndex != -1; }

    const LogEntry* getFirstViolation() const {
        if (firstViolationIndex == -1) return nullptr;
        return &history[firstViolationIndex];
    }

    const std::vector<LogEntry>& getHistory() const { return history; }

    static std::string nowTimestamp() {
        std::time_t t = std::time(nullptr);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        return std::string(buf);
    }

    void printEntryToConsole(const LogEntry& e) const {
        std::cout << "------------------------------------------------------------\n";
        std::cout << "[#" << e.sequence << "] " << e.timestamp << "\n";
        std::cout << "Operacion : " << e.operationDescription << "\n";
        std::cout << "Estado    : " << e.stateAfter << "\n";
        std::cout << "Verificacion de invariantes:\n";
        for (auto& r : e.invariantResults) {
            std::cout << "   - " << r.first << " -> "
                      << (r.second ? "CUMPLIDA" : "VIOLADA") << "\n";
        }
        if (e.anyViolation) {
            std::cout << ">>> ALERTA: se detecto violacion de invariante en esta operacion.\n";
        }
    }

    // Exporta todo el historial a un archivo de texto estructurado.
    bool exportToFile(const std::string& filepath, const std::string& structureName,
                       const std::vector<std::string>& invariantTexts) const {
        std::ofstream out(filepath);
        if (!out.is_open()) return false;

        out << "==================================================\n";
        out << " INVARICHECK - REPORTE DE EJECUCION Y VERIFICACION\n";
        out << "==================================================\n";
        out << "Estructura: " << structureName << "\n";
        out << "Generado  : " << nowTimestamp() << "\n";
        out << "Invariantes definidos:\n";
        for (auto& inv : invariantTexts) out << "  - " << inv << "\n";
        out << "--------------------------------------------------\n\n";

        for (auto& e : history) {
            out << "[#" << e.sequence << "] " << e.timestamp << "\n";
            out << "Operacion: " << e.operationDescription << "\n";
            out << "Estado resultante: " << e.stateAfter << "\n";
            out << "Resultados de verificacion:\n";
            for (auto& r : e.invariantResults) {
                out << "   - " << r.first << " : " << (r.second ? "CUMPLIDA" : "VIOLADA") << "\n";
            }
            out << "Resultado global: " << (e.anyViolation ? "VIOLACION DETECTADA" : "OK") << "\n";
            out << "\n";
        }

        out << "--------------------------------------------------\n";
        if (hasViolation()) {
            const LogEntry& fv = *getFirstViolation();
            out << "PRIMERA VIOLACION DETECTADA EN: [#" << fv.sequence << "] " << fv.timestamp << "\n";
            out << "Operacion causante: " << fv.operationDescription << "\n";
        } else {
            out << "No se detectaron violaciones de invariantes durante la ejecucion.\n";
        }
        out << "==================================================\n";

        out.close();
        return true;
    }

private:
    std::vector<LogEntry> history;
    int firstViolationIndex = -1;
};

#endif // LOGGER_H
