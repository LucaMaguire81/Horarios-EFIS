#include "LeitorCSV.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

int safeStoi(const string& s) { // Tenta converter uma string para um int. Se der erro retorna -1.

    try {
        return stoi(s);

    } catch (...) {
        return -1;
    }

}

map<string, Turma> lerHorariosCSV(const string& nomeFicheiro) {

    map<string, Turma> turmas;

    ifstream ficheiro(nomeFicheiro);

    string linha;
    getline(ficheiro, linha);

    while (getline(ficheiro, linha)) { // Leitura de cada linha do nosso ficheiro.

        stringstream ss(linha); // Lê os valores separados por vírgulas.

        string token;
        string nif, ano, letra, disciplina, sala;
        int dia, tempo;

        getline(ss, nif, '\t');
        getline(ss, ano, '\t');
        getline(ss, letra, '\t');
        getline(ss, disciplina, '\t');
        getline(ss, sala, '\t');
        getline(ss, token, '\t');
        dia = safeStoi(token); // Dia e tempo são convertidos de string para int.
        getline(ss, token, '\t');
        tempo = safeStoi(token);

        string nomeTurma = ano + letra;

        Horario h{ disciplina, dia, tempo }; // Cria um horário com os valores lidos previamente.

        if (turmas.find(nomeTurma) == turmas.end()) { // Verifica se a turma já foi adicionada ao mapa.
            turmas[nomeTurma] = Turma(nomeTurma);
        }

        turmas[nomeTurma].adicionarHorario(h); // Atribuimos um horários à lista de Horários de cada turma.

    }

    return turmas;

}
