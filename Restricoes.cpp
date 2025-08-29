#include "Restricoes.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <climits>

using namespace std;

void aplicarRestricao12Ano(vector<string>& modalidades) {
    auto it = find(modalidades.begin(), modalidades.end(), "Danca");
    if (it != modalidades.end()) swap(*it, modalidades.back());
    else modalidades.back() = "Danca";
}

void aplicarRestricaoConflitos(const map<string, Turma>& turmas,
                               map<string, vector<string>>& modalidadesPorTurma) {
    // Tentar multiplas resolucoes e guardar a com menos vazios
    const int MAX_PASSES = 60;
    unsigned seedGlobal = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rngGlobal(seedGlobal);

    map<string, vector<string>> melhor = modalidadesPorTurma;
    int melhorVazios = INT_MAX;

    vector<pair<string, Turma>> ordemTurmas(turmas.begin(), turmas.end());

    for (int pass = 0; pass < MAX_PASSES; ++pass) {
        // Em cada passe, trabalhar numa copia
        map<string, vector<string>> work = modalidadesPorTurma;
        map<pair<int,int>, set<string>> ocupacao;
        map<pair<int,int>, map<string,int>> slotCount;

        shuffle(ordemTurmas.begin(), ordemTurmas.end(), rngGlobal);

        map<string,int> duplicadosUsados;

        for (const auto& par : ordemTurmas) {
            const string& nomeTurma = par.first;
            const Turma& turma = par.second;

            // Usar accessor ordenado para indexacao estavel slot-a-slot
            vector<Horario> efis = turma.getHorariosEFISOrdenados();

            auto& mods = work[nomeTurma];
            // Garantia defensiva
            if (mods.size() < efis.size()) mods.resize(efis.size(), "");

            // Manter conjunto de modalidades ja usadas nesta turma para evitar duplicados internos
            set<string> usadasNaTurma;
            for (const string& m0 : mods) if (!m0.empty()) usadasNaTurma.insert(m0);

            // RNG para tentativas por slot
            unsigned seed = chrono::system_clock::now().time_since_epoch().count() + pass;
            mt19937 rng(seed);
            const int MAX_TENTATIVAS = 100;

            for (size_t i = 0; i < efis.size(); ++i) {
                pair<int,int> slot = {efis[i].DiaSemana, efis[i].TempoLetivo};
                auto& usadas = ocupacao[slot];

                auto pick_alternativa = [&](const string& atual) -> string {
                    // Tentar varias ordens de candidatos antes de desistir, priorizando os menos usados neste slot
                    vector<string> candidatos = mods;
                    for (int tentativa = 0; tentativa < MAX_TENTATIVAS; ++tentativa) {
                        // sort by frequency ascending, with slight shuffle to break ties
                        shuffle(candidatos.begin(), candidatos.end(), rng);
                        stable_sort(candidatos.begin(), candidatos.end(), [&](const string& a, const string& b){
                            int ca = slotCount[slot][a];
                            int cb = slotCount[slot][b];
                            return ca < cb;
                        });
                        for (const string& cand : candidatos) {
                            if (!cand.empty() && !usadas.count(cand) && (!usadasNaTurma.count(cand) || cand == atual)) {
                                return cand;
                            }
                        }
                    }
                    return string("");
                };

                bool precisaTroca = (!mods[i].empty() && usadas.count(mods[i])) || (!mods[i].empty() && usadasNaTurma.count(mods[i]) > 0 && count(mods.begin(), mods.end(), mods[i]) > 1);
                if (precisaTroca) {
                    string alternativa = pick_alternativa(mods[i]);
                    mods[i] = alternativa.empty() ? "" : alternativa;
                }

                if (!mods[i].empty()) {
                    usadas.insert(mods[i]);
                    usadasNaTurma.insert(mods[i]);
                    slotCount[slot][mods[i]]++;
                }
            }

        }

        // Avaliar numero de vazios
        int vazios = 0;
        for (auto& kv : work) {
            for (auto& m : kv.second) if (m.empty()) ++vazios;
        }

        if (vazios < melhorVazios) {
            melhorVazios = vazios;
            melhor = move(work);
            if (vazios == 0) break; // melhor possivel
        }
    }

    modalidadesPorTurma = move(melhor);
}
