#include "Turma.h"

using namespace std;

Turma::Turma(const string& nome) // Inicializa o nome da turma. Tipo 9D ou 10C.
    : nome(nome) {}

void Turma::adicionarHorario(const Horario& h) { // Adiciona um novo Horário ao vetor horários.

    horarios.push_back(h);

}

vector<Horario> Turma::getHorariosEFIS() const { // Filtra o horário completo de uma turma e devolve apenas os da disciplina "EFIS"

    vector<Horario> efisHorarios;

    for (const auto& h : horarios) {

        if (h.Disciplina == "EFIS") {

            efisHorarios.push_back(h);

        }

    }

    return efisHorarios;

}
