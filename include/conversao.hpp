#include "../include/tabelas.hpp"
#include "../include/analisador_de_instrucao.hpp"

#ifndef CONVERSION_H
/*  
*   Conversão
*   Classe que traduz as funções do assembly inventado para IA-32
*/
#define CONVERSION_H

class Conversion
{
  public:
    Conversion(std::string file_name, Tables tables, InstructionAnalizer analizer) : file_name(file_name), tables(tables), analizer(analizer)
    {
        file_asm.open(file_name + "pp.asm");
    }

    void CloseFiles()
    {
        file_asm.close();
        file_s.close();
        std::cout << "COLOCA PRA DELETAR DEPOIS!" << std::endl;
        // file_name += "pp.asm";
        // remove(file_name.c_str());
    }

    bool IsFileOpen();

    void Converting();
    void InitFileS();

  private:
    std::string file_name;
    std::ifstream file_asm;
    std::ofstream file_s;
    Tables tables;
    InstructionAnalizer analizer;
    bool hasMain = false;
};

#endif