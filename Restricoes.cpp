#include "Restricoes.h"

#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

void aplicarRestricao12Ano(vector<string>& modalidades) { // Exige que a modalidade dança esteja no final da lista de modalidades.

    bool encontrou = false;

    for (int i = modalidades.size() - 3; i < modalidades.size(); ++i) {

        if (modalidades[i] == "Danca") {
            encontrou = true;
            break;

        }

    }

    if (!encontrou) {

        for (int i = 0; i < modalidades.size() - 3; ++i) {

            if (modalidades[i] == "Danca") {

                swap(modalidades[i], modalidades.back());
                return;

            }

        }

        modalidades.back() = "Danca";

    }

}

void aplicarRestricaoConflitos(const map<string, Turma>& turmas, map<string, vector<string>>& modalidadesPorTurma) {

    // Evita conflitos de modalidades no mesmo horário, ou seja, duas turmas que partilham o mesmo horário de EFIS não podem ter a mesma modalidade ao mesmo tempo.

    map<pair<int, int>, set<string>> ocupadas;

    for (const auto& [nomeTurma, turma] : turmas) {

        auto& modalidades = modalidadesPorTurma[nomeTurma];

        for (const auto& horario : turma.getHorariosEFIS()) {

            pair<int, int> slot = {horario.DiaSemana, horario.TempoLetivo};
            set<string>& usadas = ocupadas[slot];

            for (string& mod : modalidades) {

                if (usadas.count(mod)) {

                    for (string nova : {"Futebol", "Voleibol", "Basquetebol", "Andebol", "Ginastica Solo", "Ginastica Aparelhos", "Ginastica Acrobatica", "Atletismo", "Patinagem", "Danca", "Raquetes", "Outras"}) {

                        if (!usadas.count(nova)) {

                            mod = nova;
                            break;

                        }

                    }

                }

                usadas.insert(mod);

            }

        }

    }

}
