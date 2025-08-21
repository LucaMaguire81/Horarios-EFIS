#pragma once

#include <string>
#include <vector>

using namespace std;

struct Horario {

    string Disciplina;
    int DiaSemana;
    int TempoLetivo;

};

class Turma {

public:

    string nome;
    vector<Horario> horarios;

    Turma() = default;
    Turma(const string& nome);

    void adicionarHorario(const Horario& h);

    vector<Horario> getHorariosEFIS() const;

};
