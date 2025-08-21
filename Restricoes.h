#include "LeitorCSV.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

void aplicarRestricao12Ano(vector<string>& modalidades);

void aplicarRestricaoConflitos(const map<string, Turma>& turmas, map<string, vector<string>>& modalidadesPorTurma);
