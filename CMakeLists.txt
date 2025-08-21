cmake_minimum_required(VERSION 3.25)
project(EFIS)

set(CMAKE_CXX_STANDARD 17)

add_executable(EFIS
        main.cpp
        Turma.cpp
        AtribuicaoModalidades.cpp
        Restricoes.cpp
        LeitorCSV.cpp
        QualAno.cpp
)
