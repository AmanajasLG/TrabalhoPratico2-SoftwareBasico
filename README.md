Alunos: 
    Luíza Amanajás 16/0056659

O trabalho foi desenvolvido em C++ e testado no UNIX LinuxMint 18.3 utilizando o ambiente de desenvolvimento VisualCode Studio.

Para compilar o programa é necessario g++ e ter a biblioteca boost instalada.

A biblioteca boost foi usada apenas para separação de palavras pela função boost::split e para verificar se duas palavras são iguais independente de sua case pela função boost::iequals.
Caso boost não esteja instalada no computador, ela pode ser instalda por: 
    sudo apt-get install libboost-all-dev 

Tradutor:
    Para compílar o tradutor basta entrar na pasta pelo terminal e colocar:
        g++ -o tradutor src/tradutor.cpp src/analisador_de_instrucao.cpp src/preprocessamento.cpp src/conversao.cpp src/tabelas.cpp  -std=c++11 -L/usr/local/lib/ -lboost_filesystem

    Será gerado o arquivo tradutor e para rodar basta colocar ./tradutor e o arquivo desejado. O arquivo precisa necessariamente estar dentro da pasta tradutor ou inserir o endereco junto com o nome.
    EX:
        - Arquivo dentro da pasta:
            ./tradutor teste
        -Arquivo fora da pasta:
            ./tradutor ../test

OBS: O programa considera que a linguagem hipotética possui apenas uma declaração de SECTION TEXT e faz a declaração dos procedimentos(LeerInteiro, EscreverInteiro, LeerChar, EscreverChar, LeerString, EscreverString e OverflowError) quando a mesma aparece.