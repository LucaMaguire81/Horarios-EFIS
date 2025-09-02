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
    // Modalidades que só podem ter 1 turma por horário (exclusivas)
    set<string> modalidadesExclusivas = {"Danca", "Raquetes", "outras"};
    
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
        map<pair<int,int>, map<string,int>> slotCount; // Conta quantas turmas têm cada modalidade por slot

        shuffle(ordemTurmas.begin(), ordemTurmas.end(), rngGlobal);



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
                auto& count = slotCount[slot];

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
                            if (!cand.empty()) {
                                // Verificar se a modalidade pode ser usada neste slot
                                bool podeUsar = false;
                                if (modalidadesExclusivas.count(cand)) {
                                    // Modalidade exclusiva: máximo 1 turma
                                    podeUsar = (count[cand] == 0);
                                } else {
                                    // Modalidade normal: máximo 2 turmas
                                    podeUsar = (count[cand] < 2);
                                }
                                
                                // Verificar se não é duplicada na mesma turma (exceto se for a atual)
                                bool naoDuplicada = (!usadasNaTurma.count(cand) || cand == atual);
                                
                                if (podeUsar && naoDuplicada) {
                                    return cand;
                                }
                            }
                        }
                    }
                    return string("");
                };

                // Verificar se a modalidade atual pode ser mantida
                bool podeManterAtual = false;
                if (!mods[i].empty()) {
                    if (modalidadesExclusivas.count(mods[i])) {
                        podeManterAtual = (count[mods[i]] == 0);
                    } else {
                        podeManterAtual = (count[mods[i]] < 2);
                    }
                    
                    // Verificar duplicação na turma (se já existe esta modalidade noutro slot)
                    int contagemAtual = 0;
                    for (const auto& m : mods) {
                        if (m == mods[i]) contagemAtual++;
                    }
                    if (podeManterAtual && contagemAtual > 1) {
                        podeManterAtual = false;
                    }
                }

                bool precisaTroca = !podeManterAtual;
                if (precisaTroca) {
                    string alternativa = pick_alternativa(mods[i]);
                    mods[i] = alternativa.empty() ? "" : alternativa;
                }

                if (!mods[i].empty()) {
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
