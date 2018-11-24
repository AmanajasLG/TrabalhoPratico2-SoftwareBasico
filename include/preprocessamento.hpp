#include "../include/analisador_de_instrucao.hpp"
#include "../include/tabelas.hpp"

#ifndef PREPROCESSING_H
/*
*   Pre-processamento 
*   Retira comentários e valida diretivas EQU e IF
*/
#define PREPROCESSING_H

class PreProcessing
{
  public:
    PreProcessing(std::string file_name, Tables tables, InstructionAnalizer analizer) : file_name(file_name), tables(tables), analizer(analizer)
    {
        file_asm.open(file_name + ".asm");
    }

    void CloseFiles()
    {
        file_asm.close();
        file_pre.close();
    }

    void PreProcess();

    bool isFileOpen();

  private:
    std::string file_name;
    std::ifstream file_asm;
    std::ofstream file_pre;
    Tables tables;
    InstructionAnalizer analizer;
};

#endif