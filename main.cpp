#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "LeitorCSV.h"
#include "Turma.h"
#include "QualAno.h"
#include "AtribuicaoModalidades.h"
#include "Restricoes.h"

using namespace std;

// Função para formatar modalidades com hífen
string formatarModalidade(const string& modalidade) {
    if (modalidade.empty()) return "(vazio)";
    
    string resultado = modalidade;
    // Substituir espaços por hífens
    for (char& c : resultado) {
        if (c == ' ') c = '-';
    }
    return resultado;
}

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

        // Mostrar resultados ordenados por ano (5º ao 12º, depois profissionais)
        vector<pair<string, string>> turmasOrdenadas;
        
        // Primeiro, separar turmas por tipo
        vector<pair<string, string>> turmas5a12;
        vector<pair<string, string>> turmasProfissionais;
        
        for (const auto& [nome, turma] : turmas) {
            int ano = extrairAnoTurma(nome);
            string modalidades = "";
            for (const auto& m : modalidadesPorTurma[nome]) {
                if (!modalidades.empty()) modalidades += " ";
                modalidades += formatarModalidade(m);
            }
            
            if (ano >= 5 && ano <= 12) {
                turmas5a12.push_back({nome, modalidades});
            } else if (ano >= 1 && ano <= 3 && nome != "1" && nome != "2" && nome != "3") {
                turmasProfissionais.push_back({nome, modalidades});
            }
        }
        
        // Ordenar turmas 5º ao 12º por ano
        sort(turmas5a12.begin(), turmas5a12.end(), [](const pair<string, string>& a, const pair<string, string>& b) {
            int anoA = extrairAnoTurma(a.first);
            int anoB = extrairAnoTurma(b.first);
            if (anoA != anoB) return anoA < anoB;
            return a.first < b.first; // Se mesmo ano, ordenar alfabeticamente
        });
        
        // Ordenar turmas profissionais alfabeticamente
        sort(turmasProfissionais.begin(), turmasProfissionais.end());
        
        // Mostrar turmas 5º ao 12º
        for (const auto& [nome, modalidades] : turmas5a12) {
            cout << "Turma: " << nome << " - " << modalidades << "\n";
        }
        
        // Mostrar turmas profissionais
        for (const auto& [nome, modalidades] : turmasProfissionais) {
            cout << "Turma: " << nome << " - " << modalidades << "\n";
        }

    } catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    return 0;
}
