#include "QualAno.h"

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int extrairAnoTurma(const string& nomeTurma) {

    string digitos;

    for (char c : nomeTurma) { // Percorre os caracteres do "nomeTurma". Junta os números iniciais à string digitos.

        if (isdigit(c)) digitos += c;
        else break;

    }

    if (digitos.empty() || digitos.size() > 4) { // Pequenas verificações

        cout << "Erro: string sem digitos no inicio ou numero muito grande.\n";
        return -1;

    }

    try { // Converte de string para inteiro. Se der erro, retorna -1;

        int ano = std::stoi(digitos);
        return ano;

    } catch (...) {

        cout << "Erro: excecao na conversao stoi\n";
        return -1;

    }

}
