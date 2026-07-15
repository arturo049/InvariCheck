#ifndef VERIFIER_H
#define VERIFIER_H

#include "DataStructure.h"
#include "Invariant.h"
#include "Logger.h"
#include <vector>
#include <string>

// ============================================================
// Clase Verifier
// ------------------------------------------------------------
// Orquesta el flujo: recibe una operacion ya ejecutada sobre la
// DataStructure, evalua todos los invariantes definidos contra
// el estado resultante, y registra todo en el Logger.
// ============================================================
class Verifier {
public:
    Verifier(DataStructure& ds) : structure(ds) {}

    void setInvariants(const std::vector<std::string>& rawLines) {
        invariants.clear();
        invariantTexts.clear();
        for (auto& line : rawLines) {
            invariants.emplace_back(line);
            invariantTexts.push_back(line);
        }
    }

    const std::vector<std::string>& getInvariantTexts() const { return invariantTexts; }

    // Verifica el estado actual y registra una entrada en el logger.
    // Devuelve true si NO hubo violaciones.
    bool verifyAndLog(const std::string& operationDescription) {
        LogEntry entry;
        entry.sequence = static_cast<int>(logger.getHistory().size()) + 1;
        entry.timestamp = Logger::nowTimestamp();
        entry.operationDescription = operationDescription;
        entry.stateAfter = structure.stateToString();
        entry.anyViolation = false;

        const std::vector<int>& state = structure.getState();
        for (size_t i = 0; i < invariants.size(); ++i) {
            bool ok = invariants[i].evaluate(state);
            entry.invariantResults.push_back({invariantTexts[i], ok});
            if (!ok) entry.anyViolation = true;
        }

        logger.addEntry(entry);
        logger.printEntryToConsole(entry);
        return !entry.anyViolation;
    }

    Logger& getLogger() { return logger; }
    void resetLogger() { logger.clear(); }

private:
    DataStructure& structure;
    std::vector<Invariant> invariants;
    std::vector<std::string> invariantTexts;
    Logger logger;
};

#endif // VERIFIER_H
