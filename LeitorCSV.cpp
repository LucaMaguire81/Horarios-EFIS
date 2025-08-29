#include "LeitorCSV.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>

using namespace std;

static inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

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
    if (!getline(ficheiro, linha)) return turmas;
    // validar cabecalhos (tab-separated): NIF,Turma,Disciplina,Sala,Dia,Tempo
    {
        stringstream sh(linha);
        vector<string> headers;
        string col;
        while (getline(sh, col, '\t')) headers.push_back(trim(col));
        if (headers.size() < 6 || headers[0] != "NIF" || headers[1] != "Turma" || headers[2] != "Disciplina") {
            // Cabecalho inesperado; prosseguir mesmo assim
        }
    }

    while (getline(ficheiro, linha)) { // Leitura de cada linha do nosso ficheiro.

        if (trim(linha).empty()) continue;
        stringstream ss(linha); // Lê os valores separados por vírgulas.

        string token;
        string nif, ano, letra, disciplina, sala;
        int dia, tempo;

        if (!getline(ss, token, '\t')) continue; nif = trim(token);
        if (!getline(ss, token, '\t')) continue; ano = trim(token);
        if (!getline(ss, token, '\t')) continue; letra = trim(token);
        if (!getline(ss, token, '\t')) continue; disciplina = trim(token);
        if (!getline(ss, token, '\t')) continue; sala = trim(token);
        if (!getline(ss, token, '\t')) continue; dia = safeStoi(trim(token)); // Dia e tempo são convertidos de string para int.
        if (!getline(ss, token, '\t')) continue; tempo = safeStoi(trim(token));

        if (disciplina != "EFIS") continue; // apenas EFIS interessa para atribuicao
        if (ano.empty() || letra.empty()) continue;

        string nomeTurma = ano + letra;

        Horario h{ disciplina, dia, tempo }; // Cria um horário com os valores lidos previamente.

        if (turmas.find(nomeTurma) == turmas.end()) { // Verifica se a turma já foi adicionada ao mapa.
            turmas[nomeTurma] = Turma(nomeTurma);
        }

        turmas[nomeTurma].adicionarHorario(h); // Atribuimos um horários à lista de Horários de cada turma.

    }

    return turmas;

}
