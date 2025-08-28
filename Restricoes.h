#ifndef RESTRICOES_H
#define RESTRICOES_H

#include "LeitorCSV.h"
#include "Turma.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

/**
 * Aplica a restrição específica do 12º ano:
 * Garante que "Dança" esteja sempre como última modalidade.
 */
void aplicarRestricao12Ano(vector<string>& modalidades);

/**
 * Aplica a prevenção de conflitos de modalidades no mesmo horário.
 * Não permite que duas turmas tenham a mesma modalidade ao mesmo tempo.
 * A função garante que todas as alternativas respeitam as regras de categorias
 * e limitações já estipuladas por ano/ciclo.
 */
void aplicarRestricaoConflitos(const map<string, Turma>& turmas,
                               map<string, vector<string>>& modalidadesPorTurma);

#endif // RESTRICOES_H
