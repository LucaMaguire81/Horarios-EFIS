#include "AtribuicaoModalidades.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

vector<string> atribuirModalidades(int ano) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);

    vector<string> atribuicao;

    // Categorias
    map<char, vector<string>> cat5e6 = {
        {'A', {"Pre-desportivos"}},
        {'B', {"Futebol", "Voleibol", "Basquetebol", "Andebol"}},
        {'C', {"Ginastica solo", "Ginastica aparelhos"}},
        {'D', {"Atletismo"}},
        {'E', {"Patinagem"}},
        {'F', {"Danca"}},
        {'G', {"outras"}}
    };

    map<char, vector<string>> cat7a12 = {
        {'A', {"Futebol", "Voleibol", "Basquetebol", "Andebol"}},
        {'B', {"Ginastica solo", "Ginastica aparelhos", "Ginastica acrobatica"}},
        {'C', {"Atletismo"}},
        {'D', {"Patinagem"}},
        {'E', {"Danca"}},
        {'F', {"Raquetes"}},
        {'G', {"outras"}}
    };

    if (ano == 5 || ano == 6) {
        // 5º e 6º: uma modalidade por categoria (total 6)
        vector<char> categorias = {'A', 'B', 'C', 'D', 'E', 'F'}; // Excluir G para ter apenas 6
        shuffle(categorias.begin(), categorias.end(), rng);
        
        for (char cat : categorias) {
            if (cat5e6.find(cat) != cat5e6.end() && !cat5e6[cat].empty()) {
                shuffle(cat5e6[cat].begin(), cat5e6[cat].end(), rng);
                atribuicao.push_back(cat5e6[cat][0]);
            }
        }
    } else if (ano >= 7 && ano <= 9) {
        // 7º-9º: 2A + 4 restantes (total 6)
        vector<string> opA = cat7a12['A'];
        vector<string> restantes = {cat7a12['B'][0], cat7a12['C'][0], cat7a12['D'][0], cat7a12['E'][0], cat7a12['F'][0], cat7a12['G'][0]};

        shuffle(opA.begin(), opA.end(), rng);
        shuffle(restantes.begin(), restantes.end(), rng);

        atribuicao.push_back(opA[0]);
        atribuicao.push_back(opA[1]);

        // Escolher 4 das 6 restantes para total de 6
        atribuicao.push_back(restantes[0]);
        atribuicao.push_back(restantes[1]);
        atribuicao.push_back(restantes[2]);
        atribuicao.push_back(restantes[3]);
    } else if (ano >= 10 && ano <= 12) {
        // 10º-12º: 2A + 1(B ou C) + 1E + 2 restantes (D, F ou o que sobrou de B/C)
        vector<string> opA = cat7a12['A'];
        shuffle(opA.begin(), opA.end(), rng);
        atribuicao.push_back(opA[0]);
        atribuicao.push_back(opA[1]);

        vector<string> bc = cat7a12['B'];
        bc.push_back(cat7a12['C'][0]);
        shuffle(bc.begin(), bc.end(), rng);
        string escolhidoBC = bc[0];
        atribuicao.push_back(escolhidoBC);

        atribuicao.push_back(cat7a12['E'][0]); // Danca

        // restantes: escolher 2 entre {D, F, G, e a categoria de BC não escolhida}
        vector<string> candidatos;
        if (find(cat7a12['B'].begin(), cat7a12['B'].end(), escolhidoBC) == cat7a12['B'].end()) {
            candidatos.push_back(cat7a12['B'][0]);
        } else {
            candidatos.push_back(cat7a12['C'][0]);
        }
        candidatos.push_back(cat7a12['D'][0]);
        candidatos.push_back(cat7a12['F'][0]);
        candidatos.push_back(cat7a12['G'][0]);
        shuffle(candidatos.begin(), candidatos.end(), rng);

        atribuicao.push_back(candidatos[0]);
        atribuicao.push_back(candidatos[1]);

        // Garantir Danca no final para o 12º ano
        if (ano == 12) {
            auto it = find(atribuicao.begin(), atribuicao.end(), "Danca");
            if (it != atribuicao.end()) {
                swap(*it, atribuicao.back());
            }
        }
    }

    // Aleatorizar a ordem final (exceto Danca no 12º)
    if (!(ano == 12)) {
        shuffle(atribuicao.begin(), atribuicao.end(), rng);
    }

    return atribuicao;
}
