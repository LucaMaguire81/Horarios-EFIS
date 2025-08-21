Atribuição Automática de Modalidades
---
---

Este programa tem como objetivo ler um ficheiro CSV com os horários de todas as turmas de uma escola, organizar essas informações e atribuir automaticamente modalidades desportivas a cada turma nos tempos letivos da disciplina de EFIS, tendo em conta todas as restrições especificas a cada ciclo de ensino.

---

O programa garante:

Aleatoriedade controlada → Cada turma recebe modalidades e ordens diferentes.

Respeito pelas regras de cada ciclo → 2.º ciclo, 3.º ciclo, secundário e cursos profissionais.

Respeito pelas restrições → Por exemplo, "Dança" com última modalidade para turmas do secundário.

Prevenção de conflitos → Evita sobreposição de modalidades no mesmo horário.

---

Estrutura do Projeto:

├── AtribuicaoModalidades.cpp   # Lógica de seleção das modalidades
├── LeitorCSV.cpp               # Leitura e parsing do ficheiro CSV
├── Turma.cpp                   # Classe que representa cada turma
├── QualAno.cpp                 # Extrai o ano letivo a partir do nome da turma
├── Restricoes.cpp              # Regras adicionais e prevenção de conflitos
├── main.cpp                    # Ponto de entrada do programa
├── FicheiroTeste.csv           # Ficheiro com um horários completo para teste
└── README.md                   # Documentação do projeto

---

Compilação:

g++ -std=c++17 main.cpp AtribuicaoModalidades.cpp LeitorCSV.cpp Turma.cpp QualAno.cpp Restricoes.cpp -o EFIS

---

Execução:

./EFIS