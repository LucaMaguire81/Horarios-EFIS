#pragma once
#include <string>
#include <vector>

struct Horario {
    int DiaSemana;      // 1 = Segunda, ..., 5 = Sexta
    int TempoLetivo;          // Bloco do dia (ex: 2ª aula)
    std::string Disciplina; // Ex: "EFIS"
};

class Turma {
public:
    std::string nome;
    std::vector<Horario> horarios;

    Turma() = default;
    Turma(const std::string& nome);

    void adicionarHorario(const Horario& h);
};
