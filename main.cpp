#include <iostream>
#include <map>
#include "LeitorCSV.h"

int main() {
    try {
        std::map<std::string, Turma> turmas = lerHorariosCSV("horarios.csv");

        for (const auto& [nome, turma] : turmas) {
            std::cout << "Turma: " << nome << "\n";
            for (const auto& h : turma.horarios) {
                std::cout << "  Dia " << h.DiaSemana << ", Tempo " << h.TempoLetivo << " - " << h.Disciplina << "\n";
            }
            std::cout << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    return 0;
}
