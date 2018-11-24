#include "../include/conversao.hpp"
#include <boost/algorithm/string.hpp>

/* Funcao verifica se o arquivo incluido pelo usuario pode ser aberto */
bool Conversion::IsFileOpen()
{
    if (!file_asm.is_open())
    {
        std::cout << "Erro - Arquivo " << file_name << "pp.asm nao pode ser aberto" << std::endl;
        return false;
    }

    /* Se puder ela ja gera  o arquivo .pre */
    file_s.open(file_name + ".s", std::ofstream::out);
    return true;
}

void Conversion::Converting()
{
    InitFileS();
    std::string line;
    std::string write_line = "";
    std::vector<std::string> words;
    int index;

    while (!file_asm.eof())
    {
        index = 0;
        std::getline(file_asm, line);
        boost::split(words, line, [](char c) { return c == ' ' || c == '\n' || c == '\0' || c == '\t' || c == '\r'; });

        if (analizer.IsLable(words[index]))
        {
            if (!analizer.IsSectionText())
                words[index].pop_back();

            file_s << words[index] << "     ";

            if (words.size() == 1)
            {
                file_s << "\n";
                continue;
            }

            if (analizer.IsSectionText())
                file_s << "\n";

            index++;
        }

        if (boost::iequals(words[index], "ADD"))
        {
        }
        else if (boost::iequals(words[index], "SUB"))
        {
        }
        else if (boost::iequals(words[index], "MULT"))
        {
        }
        else if (boost::iequals(words[index], "DIV"))
        {
        }
        else if (boost::iequals(words[index], "JMP"))
        {
        }
        else if (boost::iequals(words[index], "JMPN"))
        {
        }
        else if (boost::iequals(words[index], "JMPP"))
        {
        }
        else if (boost::iequals(words[index], "JMPZ"))
        {
        }
        else if (boost::iequals(words[index], "COPY"))
        {
        }
        else if (boost::iequals(words[index], "LOAD"))
        {

            file_s << "mov dword eax, [" << words[index + 1] << "]\n";
        }
        else if (boost::iequals(words[index], "STORE"))
        {
        }
        else if (boost::iequals(words[index], "INPUT"))
        {
        }
        else if (boost::iequals(words[index], "OUTPUT"))
        {
        }
        else if (boost::iequals(words[index], "C_INPUT"))
        {
        }
        else if (boost::iequals(words[index], "C_OUTPUT"))
        {
        }
        else if (boost::iequals(words[index], "S_INPUT"))
        {
        }
        else if (boost::iequals(words[index], "S_OUTPUT"))
        {
        }
        else if (boost::iequals(words[index], "STOP"))
        {
            file_s << "     mov eax, SYS_EXIT\n"
                      "     mov ebx, ebx\n"
                      "     int 0x80\n";
        }
        else if (boost::iequals(words[index], "SECTION"))
        {
            if (boost::iequals(words[index + 1], "DATA"))
            {
                analizer.ChangeSection(false);
                file_s << "\nsegment .data\n";
            }
            else if (boost::iequals(words[index + 1], "BSS"))
            {
                analizer.ChangeSection(false);
                file_s << "\nsegment .bss\n";
            }
            else
            {
                analizer.ChangeSection(true);
                file_s << "\nsection .text\n"
                          "\n"
                          "OverflowError:\n"
                          "     mov edx, len_msg\n"
                          "     mov ecx, overflow_msg\n"
                          "     mov ebx, OUTPUT\n"
                          "     mov eax, SYS_WRITE\n"
                          "     int 0x80\n"
                          "     mov eax, SYS_EXIT\n"
                          "     mov ebx, ebx\n"
                          "     int 0x80\n"
                          "\n"
                          "LeerInteiro:\n"
                          "     ret\n"
                          "\n"
                          "EscreverInteiro:\n"
                          "     ret\n"
                          "\n"
                          "LeerChar:\n"
                          "     ret\n"
                          "\n"
                          "EscreverChar:\n"
                          "     ret\n"
                          "\n"
                          "LeerString:\n"
                          "     ret\n"
                          "\n"
                          "EscreverString:\n"
                          "     ret\n"
                          "\n"
                          "     global main\n"
                          "main:\n";
            }
        }
        else if (boost::iequals(words[index], "SPACE"))
        {
            file_s << "resd     ";

            if (words.size() == 2 || words[index + 1] != " ")
            {
                file_s << "1\n";
            }
            else
            {
                file_s << words[index + 1] << "\n";
            }
        }
        else if (boost::iequals(words[index], "CONST"))
        {
            file_s << "dd    " << words[index + 1] << "\n";
        }
    }
}

void Conversion::InitFileS()
{
    analizer.ChangeSection(true);

    file_s << "SYS_EXIT     equ     1\n"
              "SYS_READ     equ     3\n"
              "SYS_WRITE    equ     4\n"
              "STDIN        equ     0\n"
              "STDOUT       equ     1\n"
              "\n"
              "segment .data\n"
              "overflow_msg     dd      'ERRO - Mult gerou overflow!',0xA\n"
              "len_msg          equ     $-overflow_msg\n"
              "\n";
}