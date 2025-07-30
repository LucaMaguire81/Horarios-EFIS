#pragma once
#include <fstream>
#include <sstream>
#include <map>
#include "Turma.h"

std::map<std::string, Turma> lerHorariosCSV(const std::string& nomeFicheiro) {
  
    std::map<std::string, Turma> turmas;
    std::ifstream ficheiro(nomeFicheiro);
    std::string linha;

    std::getline(ficheiro, linha); // Ignora cabeçalho

    while (std::getline(ficheiro, linha)) {
        std::stringstream ss(linha);
        std::string token;

        std::string nif, nomeTurma, disciplina, sala;
        int dia, tempo;

        std::getline(ss, nif, ',');
        std::getline(ss, nomeTurma, ',');
        std::getline(ss, disciplina, ',');
        std::getline(ss, sala, ',');
        std::getline(ss, token, ',');
        dia = std::stoi(token);
        std::getline(ss, token, ',');
        tempo = std::stoi(token);

        Horario h { dia, tempo, disciplina };

        if (turmas.find(nomeTurma) == turmas.end()) {
            turmas[nomeTurma] = Turma(nomeTurma);
        }

        turmas[nomeTurma].adicionarHorario(h);
    }

    return turmas;
}

