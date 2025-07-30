#include "Turma.h"

Turma::Turma(const std::string& nome) : nome(nome) {}

void Turma::adicionarHorario(const Horario& h) {
    horarios.push_back(h);
}
