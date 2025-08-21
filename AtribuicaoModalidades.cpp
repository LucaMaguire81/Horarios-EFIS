#include "AtribuicaoModalidades.h"

#include <map>
#include <random>
#include <algorithm>
#include <set>

using namespace std;

map<string, vector<string>> categoriasSegundoCiclo = {
    {"A", {"Pre-desportivos"}},
    {"B", {"Futebol", "Basquetebol", "Andebol", "Voleibol"}},
    {"C", {"Ginastica Solo", "Ginastica Aparelhos"}},
    {"D", {"Atletismo"}},
    {"E", {"Patinagem"}},
    {"F", {"Danca"}},
    {"G", {"Outras"}}
};

map<string, vector<string>> categoriasTerceiroSecundario = {
    {"A", {"Futebol", "Voleibol", "Basquetebol", "Andebol"}},
    {"B", {"Ginastica Solo", "Ginastica Aparelhos", "Ginastica Acrobatica"}},
    {"C", {"Atletismo"}},
    {"D", {"Patinagem"}},
    {"E", {"Danca"}},
    {"F", {"Raquetes"}},
    {"G", {"Outras"}}
};

random_device rd; // Gerador de números aleatórios.
mt19937 gen(rd());

string escolherAleatorioUnico(vector<string>& opcoes) { // Escolhe um elemento aleatório da lista, remove-o e retorna o valor da lista.

    if (opcoes.empty()) return "";

    uniform_int_distribution<> dist(0, opcoes.size() - 1);

    int idx = dist(gen);

    string escolhido = opcoes[idx];

    opcoes.erase(opcoes.begin() + idx);

    return escolhido;

}

vector<string> atribuirModalidadesParaAno(int ano) { // Recebe o ano da turma e retorna um vetor as modalidades selecionadas.

    vector<string> resultado;
    const size_t max_modalidades = 6;  // Limite máximo de modalidades

    if (ano == 5 || ano == 6) { // Escolhe 6 categorias de forma aleatória.

        vector<pair<string, vector<string>>> categorias(categoriasSegundoCiclo.begin(), categoriasSegundoCiclo.end());
        shuffle(categorias.begin(), categorias.end(), gen);

        for (int i = 0; i < 6; ++i) {

            vector<string> opcoes = categorias[i].second;
            resultado.push_back(escolherAleatorioUnico(opcoes));

        }

        // Limita o tamanho para o máximo definido
        if (resultado.size() > max_modalidades) {

            resultado.resize(max_modalidades);

        }

        return resultado;
    }

    if (ano >= 7 && ano <= 9) { // Começa por escolher 2 modalidades da categoria "A".
        // Depois escolhe aleatoriamente entre duas opções:
        // Opção 0: escolhe 2 modalidades da categoria "B" e uma de cada das categorias "C", "D", "E", "F" e "G".
        // Opção 1: escolhe 4 categorias aleatórias das restantes e uma modalidade de cada.

        vector<string> selecionadas;
        vector<string> a_modalidades = categoriasTerceiroSecundario["A"];

        for (int i = 0; i < 2; i++) {

            selecionadas.push_back(escolherAleatorioUnico(a_modalidades));

        }

        uniform_int_distribution<> distOp(0, 1);
        int opcao = distOp(gen);

        if (opcao == 0) {

            vector<string> b_modalidades = categoriasTerceiroSecundario["B"];

            for (int i = 0; i < 2; i++) {

                selecionadas.push_back(escolherAleatorioUnico(b_modalidades));

            }

            for (string cat : {"C", "D", "E", "F", "G"}) {

                vector<string> modal = categoriasTerceiroSecundario[cat];
                selecionadas.push_back(escolherAleatorioUnico(modal));

            }

        } else {

            vector<string> restantes = {"B", "C", "D", "E", "F", "G"};
            shuffle(restantes.begin(), restantes.end(), gen);

            for (int i = 0; i < 4; ++i) {

                vector<string> modal = categoriasTerceiroSecundario[restantes[i]];
                selecionadas.push_back(escolherAleatorioUnico(modal));

            }

        }

        // Limita o tamanho para o máximo definido
        if (selecionadas.size() > max_modalidades) {

            selecionadas.resize(max_modalidades);

        }

        resultado = selecionadas;
        return resultado;

    }

    if (ano >= 10 && ano <= 12) { // Escolhe 2 modalidades da categoria "A".
        // Escolhe aleatoriamente se seleciona 1 modalidade da categoria "B" ou "C".
        // Escolhe uma modalidade da categoria "E", exceto "Danca".
        // Escolhe uma modalidade adicional de categorias aleatórias D, F ou G.
        // Garante que há pelo menos 5 modalidades preenchidas.
        // Acrescenta "Danca" sempre na última posição da lista.

        vector<string> selecionadas;
        vector<string> a_modalidades = categoriasTerceiroSecundario["A"];

        for (int i = 0; i < 2; i++) {

            selecionadas.push_back(escolherAleatorioUnico(a_modalidades));

        }

        uniform_int_distribution<> distOp(0, 1);
        int opcao = distOp(gen);

        if (opcao == 0) {

            vector<string> b_modalidades = categoriasTerceiroSecundario["B"];
            selecionadas.push_back(escolherAleatorioUnico(b_modalidades));

        } else {

            vector<string> c_modalidades = categoriasTerceiroSecundario["C"];
            selecionadas.push_back(escolherAleatorioUnico(c_modalidades));

        }

        vector<string> e_modalidades = categoriasTerceiroSecundario["E"];
        vector<string> e_sem_danca;

        for (const auto& m : e_modalidades) {

            if (m != "Danca") e_sem_danca.push_back(m);

        }

        if (!e_sem_danca.empty()) {

            selecionadas.push_back(escolherAleatorioUnico(e_sem_danca));

        }

        vector<string> restantes = {"D", "F", "G"};
        shuffle(restantes.begin(), restantes.end(), gen);

        for (int i = 0; i < 1; i++) {

            vector<string> modal = categoriasTerceiroSecundario[restantes[i]];
            selecionadas.push_back(escolherAleatorioUnico(modal));

        }

        // Enquanto não tiver 5 modalidades, acrescenta vazio
        while (selecionadas.size() < 5) {

            selecionadas.push_back("");

        }

        selecionadas.push_back("Danca"); // Acrescenta "Danca" sempre no fim

        // Limita o tamanho para o máximo definido (6), mantendo "Danca" no fim
        if (selecionadas.size() > max_modalidades) {

            // Remove excedentes antes do último elemento
            selecionadas.erase(selecionadas.begin() + max_modalidades, selecionadas.end() - 1);

        }

        resultado = selecionadas;
        return resultado;

    }

    return resultado;

}
