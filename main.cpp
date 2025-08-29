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

        map<string, vector<string>> melhorModalidades;
        int melhorVazios = INT_MAX;
        int melhorSlots = 0;
        double melhorPerc = 100.0;
        int melhorTentativa = -1;

        const int MAX_TENTATIVAS_GLOBAIS = 200;
        for (int tentativa = 1; tentativa <= MAX_TENTATIVAS_GLOBAIS; ++tentativa) {
            map<string, vector<string>> modalidadesPorTurma;

            // Atribuir modalidades a cada turma, alinhando ao numero de blocos EFIS
            for (const auto& [nome, turma] : turmas) {
                int ano = extrairAnoTurma(nome);
                vector<string> modalidades;

                if (ano >= 1 && ano <= 3 && nome != "1" && nome != "2" && nome != "3") {
                    // Curso Profissional → só "Danca"
                    modalidades = {"", "", "", "", "", "Danca"};
                } else {
                    // Atribuir modalidades de acordo com o ano
                    modalidades = atribuirModalidades(ano);

                    // Aplicar restrição específica do 12º ano
                    if (ano == 12) {
                        aplicarRestricao12Ano(modalidades);
                    }
                }

                // Alinhar numero de modalidades com o numero de horarios EFIS desta turma (ordenados)
                vector<Horario> efis = turma.getHorariosEFISOrdenados();

                size_t numSlots = efis.size();
                // Nao reduzir o plano de modalidades (mantemos 6 planeadas)
                // Apenas garantir que ha modalidades suficientes para cobrir os slots existentes
                if (modalidades.size() < numSlots) {
                    modalidades.resize(numSlots, "");
                }

                modalidadesPorTurma[nome] = modalidades;
            }

            // Garantir que não existem conflitos de horários entre turmas
            aplicarRestricaoConflitos(turmas, modalidadesPorTurma);

            // Calcular resumo de vazios (5-12)
            int totalVazios = 0;
            int totalSlots5a12 = 0;
            for (const auto& [nomeTurma, mods] : modalidadesPorTurma) {
                int anoTurma = extrairAnoTurma(nomeTurma);
                if (anoTurma >= 5 && anoTurma <= 12) {
                    // contar vazios nas modalidades e total de slots EFIS reais
                    for (const auto& m : mods) if (m.empty()) ++totalVazios;
                    auto itTurma = turmas.find(nomeTurma);
                    if (itTurma != turmas.end()) {
                        totalSlots5a12 += static_cast<int>(itTurma->second.getHorariosEFISOrdenados().size());
                    }
                }
            }
            double perc = (totalSlots5a12 > 0) ? (100.0 * totalVazios / totalSlots5a12) : 0.0;

            if (perc < melhorPerc) {
                melhorPerc = perc;
                melhorVazios = totalVazios;
                melhorSlots = totalSlots5a12;
                melhorModalidades = modalidadesPorTurma;
                melhorTentativa = tentativa;
            }

            if (perc < 10) {
                break;
            }
        }

        // Usar o melhor resultado encontrado
        map<string, vector<string>> modalidadesPorTurma = move(melhorModalidades);

        cout << "Vazios totais (5-12): " << melhorVazios
             << " de " << melhorSlots
             << " slots (" << melhorPerc << "%)"
             << " | tentativa " << melhorTentativa << "\n\n";

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
