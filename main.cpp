#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "LeitorCSV.h"
#include "Turma.h"
#include "QualAno.h"
#include "AtribuicaoModalidades.h"
#include "Restricoes.h"

using namespace std;

int main() {
    try {
        // Ler o ficheiro CSV e criar turmas
        map<string, Turma> turmas = lerHorariosCSV("FicheiroTeste.csv");

        map<string, vector<string>> modalidadesPorTurma;

        // Atribuir modalidades a cada turma
        for (const auto& [nome, turma] : turmas) {
            int ano = extrairAnoTurma(nome);
            vector<string> modalidades;

            if (ano >= 1 && ano <= 3 && nome != "1" && nome != "2" && nome != "3") {
                // Curso Profissional → só "Dança"
                modalidades = {"", "", "", "", "", "Dança"};
            } else {
                // Atribuir modalidades de acordo com o ano
                modalidades = atribuirModalidades(ano);

                // Aplicar restrição específica do 12º ano
                if (ano == 12) {
                    aplicarRestricao12Ano(modalidades);
                }
            }

            modalidadesPorTurma[nome] = modalidades;
        }

        // Garantir que não existem conflitos de horários entre turmas
        aplicarRestricaoConflitos(turmas, modalidadesPorTurma);

        // Mostrar resultados
        for (const auto& [nome, turma] : turmas) {
            cout << "Turma: " << nome << "\n";
            for (const auto& h : turma.getHorariosEFIS()) {
                cout << "Dia " << h.DiaSemana
                     << ", Tempo Letivo " << h.TempoLetivo
                     << ", " << h.Disciplina << "\n";
            }

            cout << "Modalidades atribuidas: ";
            for (const auto& m : modalidadesPorTurma[nome]) {
                cout << (m.empty() ? "(vazio)" : m) << "; ";
            }
            cout << "\n\n";
        }

    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    return 0;
}
