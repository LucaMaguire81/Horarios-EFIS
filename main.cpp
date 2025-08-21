#include "LeitorCSV.h"
#include "AtribuicaoModalidades.h"
#include "QualAno.h"
#include "Restricoes.h"
#include "Turma.h"

#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

int main() {

    try {

        map<string, Turma> turmas = lerHorariosCSV("FicheiroTeste.csv"); // Cria um map com o nome da turma como chave e o objeto Turma como valor.

        map<string, vector<string>> modalidadesPorTurma; // Guardamos a lista de modalidades atribuídas a cada turma.

        for (const auto& [nome, turma] : turmas) { // Percorre todas as turmas e executa todas as condições.

            int ano = extrairAnoTurma(nome);
            vector<string> modalidades;

            if (ano >= 1 && ano <= 3 && nome != "1" && nome != "2" && nome != "3") {

                modalidades = {"", "", "", "", "", "Danca"};

            } else {

                modalidades = atribuirModalidadesParaAno(ano);

                if (ano == 12) {

                    aplicarRestricao12Ano(modalidades);

                }

                if (ano >= 10 && ano <= 12) {

                    auto it = find(modalidades.begin(), modalidades.end(), std::string("Danca"));

                    if (it != modalidades.end() && it != modalidades.end() - 1) {

                        modalidades.erase(it);
                        modalidades.push_back("Danca");

                    }

                }

            }

            modalidadesPorTurma[nome] = modalidades;

        }

        aplicarRestricaoConflitos(turmas, modalidadesPorTurma); // Evita que duas turmas com EFIS no mesmo horário tenham a mesma modalidade.

        for (const auto& [nome, turma] : turmas) { // Output do programa.

            cout << "Turma: " << nome << "\n";

            for (const auto& h : turma.getHorariosEFIS()) {

                cout << "Dia " << h.DiaSemana << ", Tempo Letivo " << h.TempoLetivo << ", " << h.Disciplina << "\n";

            }

            cout << "Modalidades atribuidas: ";

            for (const auto& m : modalidadesPorTurma[nome]) {

                cout << (m.empty() ? "(vazio)" : m) << "; ";

            }

            cout << "\n\n";

        }

    } catch (const std::exception& e) {

        cerr << "Erro: " << e.what() << std::endl;

    }

    return 0;

}
