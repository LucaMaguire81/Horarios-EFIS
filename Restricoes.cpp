#include "Restricoes.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void aplicarRestricao12Ano(vector<string>& modalidades) {
    auto it = find(modalidades.begin(), modalidades.end(), "Danca");
    if (it != modalidades.end()) swap(*it, modalidades.back());
    else modalidades.back() = "Danca";
}

void aplicarRestricaoConflitos(const map<string, Turma>& turmas,
                               map<string, vector<string>>& modalidadesPorTurma) {
    map<pair<int,int>, set<string>> ocupacao;

    for (auto& [nomeTurma, turma] : turmas) {
        auto& mods = modalidadesPorTurma[nomeTurma];

        for (auto& h : turma.getHorariosEFIS()) {
            pair<int,int> slot = {h.DiaSemana, h.TempoLetivo};
            set<string> usadas = ocupacao[slot];

            for (size_t i=0;i<mods.size();++i) {
                if (usadas.count(mods[i])) {
                    // Tentar encontrar alternativa que nao viole regras
                    string alternativa = "(Vazio)";
                    for (auto &cand : mods) {
                        if (!usadas.count(cand)) {
                            alternativa = cand;
                            break;
                        }
                    }
                    mods[i] = alternativa;
                }
                usadas.insert(mods[i]);
            }
            ocupacao[slot] = usadas;
        }
    }
}
