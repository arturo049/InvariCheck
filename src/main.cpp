#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

#include "DataStructure.h"
#include "Verifier.h"

// ============================================================
// InvariCheck - Verificador de Invariantes en Estructuras de Datos
// ============================================================
// Aplicacion de consola que permite:
//  1. Elegir tipo de estructura (lista/pila/cola)
//  2. Definir invariantes formales en texto estructurado
//  3. Ejecutar operaciones (insertar/eliminar/consultar)
//  4. Verificar automaticamente los invariantes tras cada operacion
//  5/6/7. Mostrar y registrar resultado y estado en cada paso
//  8. Reportar la primera violacion detectada
//  9. Reiniciar la estructura manteniendo los invariantes
// 10. Exportar el historial completo a un archivo de texto
// ============================================================

static void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static StructureType chooseStructureType() {
    int opt = 0;
    while (true) {
        std::cout << "\nSeleccione el tipo de estructura a verificar:\n";
        std::cout << " 1. Lista\n 2. Pila\n 3. Cola\n";
        std::cout << "Opcion: ";
        if (std::cin >> opt && opt >= 1 && opt <= 3) break;
        std::cout << "Opcion invalida.\n";
        clearInputBuffer();
    }
    if (opt == 1) return StructureType::LIST;
    if (opt == 2) return StructureType::STACK;
    return StructureType::QUEUE;
}

// Permite al usuario escribir, linea por linea, los invariantes
// en el mini-lenguaje textual estructurado. Linea vacia termina.
static std::vector<std::string> defineInvariants() {
    std::vector<std::string> lines;
    std::cout << "\nDefina los invariantes formales (uno por linea).\n";
    std::cout << "Comandos disponibles: SIZE <=|>=|== N, NO_DUPLICATES, SORTED_ASC,\n";
    std::cout << "SORTED_DESC, ALL_POSITIVE, ALL_NEGATIVE, NOT_EMPTY, MAX_VALUE <= N, MIN_VALUE >= N\n";
    std::cout << "Escriba una linea vacia para terminar.\n";

    clearInputBuffer();
    while (true) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) break;
        try {
            Invariant test(line); // valida sintaxis
            lines.push_back(line);
            std::cout << "  Invariante aceptado: " << line << "\n";
        } catch (const std::exception& ex) {
            std::cout << "  ERROR de sintaxis: " << ex.what() << "\n";
            std::cout << "  Intente nuevamente con un formato valido.\n";
        }
    }
    return lines;
}

static void printMenu(StructureType t) {
    std::cout << "\n================ INVARICHECK ================\n";
    std::cout << "Estructura actual: " << structureTypeToString(t) << "\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << " 1. Insertar valor\n";
    std::cout << " 2. Eliminar valor\n";
    std::cout << " 3. Consultar valor\n";
    std::cout << " 4. Ver estado actual\n";
    std::cout << " 5. Ver historial completo\n";
    std::cout << " 6. Ver primera violacion detectada\n";
    std::cout << " 7. Reiniciar estructura (mismos invariantes)\n";
    std::cout << " 8. Exportar historial a archivo .txt\n";
    std::cout << " 9. Redefinir invariantes\n";
    std::cout << " 0. Salir\n";
    std::cout << "Opcion: ";
}

int main() {
    std::cout << "Bienvenido a InvariCheck\n";
    std::cout << "Verificador de Invariantes en Estructuras de Datos\n";

    StructureType type = chooseStructureType();
    DataStructure structure(type);
    Verifier verifier(structure);

    std::vector<std::string> invLines = defineInvariants();
    verifier.setInvariants(invLines);

    // Verificacion inicial (estructura vacia) para tener punto de partida en el historial.
    verifier.verifyAndLog("Estado inicial (estructura creada, sin operaciones)");

    bool running = true;
    while (running) {
        printMenu(type);
        int opt;
        if (!(std::cin >> opt)) { clearInputBuffer(); continue; }

        switch (opt) {
            case 1: { // Insertar
                int value;
                std::cout << "Valor a insertar: ";
                if (!(std::cin >> value)) { clearInputBuffer(); break; }
                int pos = -1;
                if (type == StructureType::LIST) {
                    std::cout << "Posicion (vacio/-1 = al final): ";
                    clearInputBuffer();
                    std::string posLine;
                    std::getline(std::cin, posLine);
                    if (!posLine.empty()) pos = std::stoi(posLine);
                } else {
                    clearInputBuffer();
                }
                structure.insert(value, pos);
                std::ostringstream desc;
                desc << "INSERTAR " << value
                     << (type == StructureType::LIST ? (" en posicion " + std::to_string(pos)) : "");
                verifier.verifyAndLog(desc.str());
                break;
            }
            case 2: { // Eliminar
                int pos = -1;
                if (type == StructureType::LIST) {
                    std::cout << "Posicion a eliminar (vacio/-1 = al final): ";
                    clearInputBuffer();
                    std::string posLine;
                    std::getline(std::cin, posLine);
                    if (!posLine.empty()) pos = std::stoi(posLine);
                } else {
                    clearInputBuffer();
                }
                try {
                    int val = structure.remove(pos);
                    std::ostringstream desc;
                    desc << "ELIMINAR -> valor extraido: " << val;
                    verifier.verifyAndLog(desc.str());
                } catch (const std::exception& ex) {
                    std::cout << "ERROR: " << ex.what() << "\n";
                }
                break;
            }
            case 3: { // Consultar
                int pos = -1;
                if (type == StructureType::LIST) {
                    std::cout << "Posicion a consultar (vacio/-1 = ultimo): ";
                    clearInputBuffer();
                    std::string posLine;
                    std::getline(std::cin, posLine);
                    if (!posLine.empty()) pos = std::stoi(posLine);
                } else {
                    clearInputBuffer();
                }
                try {
                    int val = structure.query(pos);
                    std::cout << "Valor consultado: " << val << "\n";
                    std::ostringstream desc;
                    desc << "CONSULTAR -> valor leido: " << val << " (no modifica el estado)";
                    verifier.verifyAndLog(desc.str());
                } catch (const std::exception& ex) {
                    std::cout << "ERROR: " << ex.what() << "\n";
                }
                break;
            }
            case 4: { // Ver estado actual
                std::cout << "Estado actual: " << structure.stateToString() << "\n";
                break;
            }
            case 5: { // Ver historial completo
                std::cout << "\n================ HISTORIAL COMPLETO ================\n";
                for (auto& e : verifier.getLogger().getHistory()) {
                    verifier.getLogger().printEntryToConsole(e);
                }
                break;
            }
            case 6: { // Ver primera violacion
                auto* fv = verifier.getLogger().getFirstViolation();
                if (fv == nullptr) {
                    std::cout << "No se ha detectado ninguna violacion de invariantes hasta el momento.\n";
                } else {
                    std::cout << "\n>>> PRIMERA VIOLACION DETECTADA <<<\n";
                    verifier.getLogger().printEntryToConsole(*fv);
                }
                break;
            }
            case 7: { // Reiniciar
                structure.clear();
                verifier.resetLogger();
                verifier.verifyAndLog("Reinicio de estructura (invariantes conservados)");
                std::cout << "Estructura reiniciada. Los invariantes se mantienen.\n";
                break;
            }
            case 8: { // Exportar
                std::cout << "Nombre del archivo de salida (ej: historial.txt): ";
                clearInputBuffer();
                std::string filename;
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "historial_invaricheck.txt";
                bool ok = verifier.getLogger().exportToFile(
                    filename, structureTypeToString(type), verifier.getInvariantTexts());
                if (ok) std::cout << "Historial exportado correctamente a: " << filename << "\n";
                else std::cout << "ERROR: no se pudo escribir el archivo.\n";
                break;
            }
            case 9: { // Redefinir invariantes
                invLines = defineInvariants();
                verifier.setInvariants(invLines);
                std::cout << "Invariantes redefinidos. Se mantiene el estado actual de la estructura.\n";
                verifier.verifyAndLog("Redefinicion de invariantes");
                break;
            }
            case 0: {
                running = false;
                break;
            }
            default:
                std::cout << "Opcion invalida.\n";
                clearInputBuffer();
        }
    }

    std::cout << "\nGracias por usar InvariCheck.\n";
    return 0;
}
