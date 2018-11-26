#include "../include/conversao.hpp"
#include <ctype.h>
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

/* Funcao analiza as comandos do assembly inventado e converte para ia32 */
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
        boost::split(words, line, [](char c) { return c == ' ' || c == '\n' || c == '\0' || c == '\t' || c == '\r' || c == '\v' || c == '\f'; });

        if (analizer.IsLable(words[index]))
        {
            if (!analizer.IsSectionText())
                words[index].pop_back();

            file_s << "     " << words[index] << "    ";

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
            file_s << "     add eax, dword ";

            if (IsNumber(words[index + 1]))
            {
                file_s << words[index + 1];
            }
            else
            {
                file_s << "[" << words[index + 1];

                if (words[index + 2] == "+")
                {
                    int addr = std::stoi(words[index + 3]) * 4;
                    file_s << " + " << std::to_string(addr);
                }

                file_s << "]";
            }

            file_s << "\n";
        }
        else if (boost::iequals(words[index], "SUB"))
        {
            file_s << "     sub eax, dword ";

            if (IsNumber(words[index + 1]))
            {
                file_s << words[index + 1];
            }
            else
            {
                file_s << "[" << words[index + 1];

                if (words[index + 2] == "+")
                {
                    int addr = std::stoi(words[index + 3]) * 4;
                    file_s << " + " << std::to_string(addr);
                }

                file_s << "]";
            }

            file_s << "\n";
        }
        else if (boost::iequals(words[index], "MULT"))
        {
            file_s << "     imul dword ";

            if (IsNumber(words[index + 1]))
            {
                file_s << words[index + 1];
            }
            else
            {
                file_s << "[" << words[index + 1];

                if (words[index + 2] == "+")
                {
                    int addr = std::stoi(words[index + 3]) * 4;
                    file_s << " + " << std::to_string(addr);
                }
            }

            file_s << "]\n";
            "     jo OverflowError\n";
        }
        else if (boost::iequals(words[index], "DIV"))
        {
            file_s << "     cdq\n"
                      "     idiv dword ";

            if (IsNumber(words[index + 1]))
            {
                file_s << words[index + 1];
            }
            else
            {
                file_s << "[" << words[index + 1];

                if (words[index + 2] == "+")
                {
                    int addr = std::stoi(words[index + 3]) * 4;
                    file_s << " + " << std::to_string(addr);
                }

                file_s << "]";
            }

            file_s << "\n";
        }
        else if (boost::iequals(words[index], "JMP"))
        {
            file_s << "     jmp " << words[index + 1] << "\n";
        }
        else if (boost::iequals(words[index], "JMPN"))
        {
            file_s << "     cmp eax,0\n"
                      "     jl "
                   << words[index + 1] << "\n";
        }
        else if (boost::iequals(words[index], "JMPP"))
        {
            file_s << "     cmp eax,0\n"
                      "     jg "
                   << words[index + 1] << "\n";
        }
        else if (boost::iequals(words[index], "JMPZ"))
        {
            file_s << "     cmp eax,0\n"
                      "     je "
                   << words[index + 1] << "\n";
        }
        else if (boost::iequals(words[index], "COPY"))
        {
            if (words[index + 1].back() == ',')
                words[index + 1].pop_back();

            std::string src = "", dest = "";

            dest += "[" + words[index + 1];

            if (words[index + 2] == "+")
            {
                words[index + 3].pop_back();

                int addr = std::stoi(words[index + 3]) * 4;
                dest += " + " + std::to_string(addr);

                index += 2;
            }

            dest += "]";

            if (IsNumber(words[index + 2]))
            {
                src += words[index + 2];
            }
            else
            {
                src += "[" + words[index + 2];

                if (words[index + 3] == "+")
                {
                    int addr = std::stoi(words[index + 4]) * 4;
                    src += " + " + std::to_string(addr);
                }
                src += "]";
            }

            file_s << "     push ebx\n"
                      "     mov ebx, "
                   << src << "\n     mov "
                   << dest << ", ebx\n"
                              "     pop ebx\n";
        }
        else if (boost::iequals(words[index], "LOAD"))
        {
            file_s << "     mov eax, ";

            if (IsNumber(words[index + 1]))
            {
                file_s << words[index + 1];
            }
            else
            {
                file_s << "[" << words[index + 1];

                if (words[index + 2] == "+")
                {
                    int addr = std::stoi(words[index + 3]) * 4;
                    file_s << " + " << std::to_string(addr);
                }

                file_s << "]";
            }

            file_s << "\n";
        }
        else if (boost::iequals(words[index], "STORE"))
        {
            file_s << "     mov [" << words[index + 1];

            if (words[index + 2] == "+")
            {
                int addr = std::stoi(words[index + 3]) * 4;
                file_s << " + " << std::to_string(addr);
            }

            file_s << "], eax\n";
        }
        else if (boost::iequals(words[index], "INPUT"))
        {
            file_s << "     mov eax, SYS_WRITE\n"
                      "     mov ebx, STDOUT\n"
                      "     mov ecx, numberMsg\n"
                      "     mov edx, lenNumberMsg\n"
                      "     int 0x80\n"
                      "     push dword "
                   << words[index + 1];

            if (words[index + 2] == "+")
            {
                int addr = std::stoi(words[index + 3]) * 4;
                file_s << " + " << std::to_string(addr);
            }

            file_s << "\n     call LeerInteiro\n";
        }
        else if (boost::iequals(words[index], "OUTPUT"))
        {
            file_s << "     push aux\n"
                      "     push dword ["
                   << words[index + 1];

            if (words[index + 2] == "+")
            {
                int addr = std::stoi(words[index + 3]) * 4;
                file_s << " + " << std::to_string(addr);
            }

            file_s << "]\n     call EscreverInteiro\n";
        }
        else if (boost::iequals(words[index], "C_INPUT"))
        {
            file_s << "     pusha\n"
                      "     mov eax, SYS_WRITE\n"
                      "     mov ebx, STDOUT\n"
                      "     mov ecx, charMsg\n"
                      "     mov edx, lenCharMsg\n"
                      "     int 0x80\n"
                      "     popa\n"
                      "     push dword "
                   << words[index + 1];

            if (words[index + 2] == "+")
            {
                int addr = std::stoi(words[index + 3]) * 4;
                file_s << " + " << std::to_string(addr);
            }

            file_s << "\n     call LeerChar\n";
        }
        else if (boost::iequals(words[index], "C_OUTPUT"))
        {
            file_s << "     push dword "
                   << words[index + 1];

            if (words[index + 2] == "+")
            {
                int addr = std::stoi(words[index + 3]) * 4;
                file_s << " + " << std::to_string(addr);
            }

            file_s << "\n     call EscreverChar\n";
        }
        else if (boost::iequals(words[index], "S_INPUT"))
        {
            words[index + 1].pop_back();

            file_s << "     pusha\n"
                      "     mov eax, SYS_WRITE\n"
                      "     mov ebx, STDOUT\n"
                      "     mov ecx, stringMsg\n"
                      "     mov edx, lenStringMsg\n"
                      "     int 0x80\n"
                      "     popa\n"
                      "     push dword "
                   << words[index + 1]
                   << "\n     push dword "
                   << words[index + 2]
                   << "\n     call LeerString\n";
        }
        else if (boost::iequals(words[index], "S_OUTPUT"))
        {
            words[index + 1].pop_back();

            file_s << "     push dword "
                   << words[index + 1]
                   << "\n     push dword "
                   << words[index + 2]
                   << "\n     call EscreverString\n";
        }
        else if (boost::iequals(words[index], "STOP"))
        {
            file_s << "     mov eax, SYS_EXIT\n"
                      "     mov ebx, 0\n"
                      "     int 0x80\n"
                      "\n"
                      "    OverflowError:\n"
                      "     mov edx, lenOverflowMsg\n"
                      "     mov ecx, overflowMsg\n"
                      "     mov ebx, STDOUT\n"
                      "     mov eax, SYS_WRITE\n"
                      "     int 0x80\n"
                      "     mov eax, SYS_EXIT\n"
                      "     mov ebx, -1\n"
                      "     int 0x80\n";
            AddIOFunctions();
        }
        else if (boost::iequals(words[index], "SECTION"))
        {
            if (boost::iequals(words[index + 1], "DATA"))
            {
                analizer.ChangeSection(false);
                file_s << "\nsection .data\n";
            }
            else if (boost::iequals(words[index + 1], "BSS"))
            {
                analizer.ChangeSection(false);
                file_s << "\nsection .bss\n"
                          "     aux:     resb    100\n";
            }
            else
            {
                analizer.ChangeSection(true);
                file_s << "\nsection .text\n"
                          "     global _start\n"
                          "_start:\n";
            }
        }
        else if (boost::iequals(words[index], "SPACE"))
        {
            file_s << "resd     ";

            if (words.size() > 2 && words[index + 1] != "")
            {
                file_s << words[index + 1] << "\n";
            }
            else
            {
                file_s << "1\n";
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
              "\n";
}

void Conversion::AddIOFunctions()
{
    file_s << "section .data\n"
              "     overflowMsg         dd          'ERRO - Mult gerou overflow!',0xA\n"
              "     lenOverflowMsg      equ         $-overflowMsg\n"
              "     numberError         db          'Entrada invalida para numero', 0xA\n"
              "     lenNumberError      EQU         $-numberError\n"
              "     numberMsg           db          'Insira um numero: ',0xA\n"
              "     lenNumberMsg        equ         $-numberMsg\n"
              "     stringMsg           db          'Insira uma string: ',0xA\n"
              "     lenStringMsg        equ         $-stringMsg\n"
              "     charMsg             db          'Insira um char: ',0xA\n"
              "     lenCharMsg          equ         $-charMsg\n"
              "     endl                db          0xa\n"
              "\n"
              "section .text"
              "\nLeerInteiro:\n"
              "     enter 6,0\n"
              "     push eax\n"
              "     push ebx\n"
              "     push ecx\n"
              "     push edx\n"
              "     mov dword [ebp-6], 0\n"
              "     mov byte [ebp-2], 0\n"
              "     mov eax, SYS_READ\n"
              "     mov ebx, STDIN\n"
              "     mov ecx, ebp\n"
              "     sub ecx, 1\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     cmp byte [ebp-1], 0x2D\n"
              "     jne checkNum\n"
              "     mov byte [ebp-2], 0x2D\n"
              "     loopNum:\n"
              "     mov eax, SYS_READ\n"
              "     mov ebx, STDIN\n"
              "     mov ecx, ebp\n"
              "     sub ecx, 1\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     checkNum:\n"
              "     mov esi, [ebp-6]\n"
              "     cmp byte [ebp-1], 0xA\n"
              "     je endNumberIn\n"
              "     cmp byte [ebp-1], 0x30 \n"
              "     jb numberInvalid\n"
              "     cmp byte [ebp-1], 0x39\n"
              "     ja numberInvalid\n"
              "     mov eax, 10\n"
              "     mul dword [ebp-6]\n"
              "     add byte AL, [ebp-1]\n"
              "     adc AH, 0\n"
              "     sub eax, 0x30\n"
              "     mov dword [ebp-6], eax\n"
              "     jmp loopNum\n"
              "     numberInvalid: \n"
              "     mov eax, SYS_WRITE\n"
              "     mov ebx, STDOUT\n"
              "     mov ecx, dword numberError\n"
              "     mov edx, lenNumberError\n"
              "     int 0x80\n"
              "     endNumberIn:\n"
              "     cmp byte [ebp-2], 0x2D\n"
              "     jne return\n"
              "     neg dword [ebp-6]\n"
              "     return:\n"
              "     mov eax, [ebp-6]\n"
              "     mov ebx, [ebp+8]\n"
              "     mov dword [ebx], eax\n"
              "     pop edx\n"
              "     pop ecx\n"
              "     pop ebx\n"
              "     pop eax\n"
              "     leave\n"
              "     ret 4\n"
              "\n"
              "EscreverInteiro:\n"
              "     enter 4,0\n"
              "     pusha\n"
              "     mov ecx, 0\n"
              "     mov eax, [ebp+8]\n"
              "     mov ebx, [ebp+12]\n"
              "     mov dword [ebp-4],10\n"
              "     cmp eax, 0\n"
              "     jge loopNumberOut\n"
              "     neg eax\n"
              "     loopNumberOut:\n"
              "     cdq\n"
              "     div dword [ebp-4]\n"
              "     add edx, 0x30\n"
              "     mov byte [ebx + ecx], dl\n"
              "     inc ecx\n"
              "     cmp eax, 0\n"
              "     jne loopNumberOut\n"
              "     mov eax, [ebp+8]\n"
              "     cmp eax, 0\n"
              "     jge endNumberOut\n"
              "     mov byte [ebx + ecx ], 0x2D\n"
              "     inc ecx\n"
              "     endNumberOut:\n"
              "     mov eax, ecx\n"
              "     dec ecx\n"
              "     push ebx\n"
              "     push ecx\n"
              "     call swap\n"
              "     mov edx, eax\n"
              "     mov ecx, ebx\n"
              "     mov eax, SYS_WRITE\n"
              "     mov ebx, STDOUT\n"
              "     int 0x80\n"
              "     add eax, 1\n"
              "     mov eax, SYS_WRITE\n"
              "     mov ebx, STDOUT\n"
              "     mov ecx, endl\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     popa\n"
              "     leave\n"
              "     ret 8\n"
              "\n"
              "LeerChar:\n"
              "     enter 0,0\n"
              "     push ebx\n"
              "     push ecx\n"
              "     push edx\n"
              "     mov eax, SYS_READ\n"
              "     mov ebx, STDIN\n"
              "     mov ecx, [ebp+8]\n"
              "     mov edx, 2\n"
              "     int 0x80\n"
              "     pop edx\n"
              "     pop ecx\n"
              "     pop ebx\n"
              "     leave\n"
              "     ret 4\n"
              "\n"
              "EscreverChar:\n"
              "     enter 0,0\n"
              "     push eax\n"
              "     push ebx\n"
              "     push ecx\n"
              "     push edx\n"
              "     mov eax, SYS_WRITE\n"
              "     mov ebx, STDOUT\n"
              "     mov ecx, dword [ebp+8]\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     mov eax, SYS_WRITE\n"
              "     mov ebx, STDOUT\n"
              "     mov ecx, endl\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     pop edx\n"
              "     pop ecx\n"
              "     pop ebx\n"
              "     pop eax\n"
              "     leave\n"
              "     ret 4\n"
              "\n"
              "LeerString:\n"
              "     enter 0,0\n"
              "     push ebx\n"
              "     push ecx\n"
              "     push edx\n"
              "     mov ecx, [ebp+8]\n"
              "     mov ebx, [ebp+12]\n"
              "     add ecx, ebx\n"
              "     loopStringIn:\n"
              "     push ebx\n"
              "     call Leer\n"
              "     cmp byte [ebx], 0xA\n"
              "     je endStringIn\n"
              "     inc ebx\n"
              "     cmp ebx, ecx\n"
              "     jb loopStringIn\n"
              "     mov byte [ebx], 0xA \n"
              "     endStringIn:\n"
              "     mov eax, ebx\n"
              "     sub eax,[ebp+12]\n"
              "     pop edx\n"
              "     pop ecx\n"
              "     pop ebx\n"
              "     leave\n"
              "     ret 8\n"
              "\n"
              "EscreverString:\n"
              "     enter 0,0\n"
              "     push ebx\n"
              "     push ecx\n"
              "     push edx\n"
              "     mov ecx, [ebp+8]\n"
              "     mov ebx, [ebp+12]\n"
              "     add ecx, ebx\n"
              "     loopStringOut:\n"
              "     push ebx\n"
              "     call Escrever\n"
              "     cmp byte [ebx], 0xA\n"
              "     je endStringOut\n"
              "     inc ebx\n"
              "     cmp ebx, ecx\n"
              "     jb loopStringOut\n"
              "     endStringOut:\n"
              "     mov eax, ebx\n"
              "     sub eax,[ebp+12]\n"
              "     mov eax, SYS_WRITE\n"
              "     mov ebx, STDOUT\n"
              "     mov ecx, endl\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     int 80h\n"
              "     pop edx\n"
              "     pop ecx\n"
              "     pop ebx\n"
              "     leave\n"
              "     ret 8\n"
              "\n"
              "Leer:\n"
              "     enter 0,0\n"
              "     push ebx\n"
              "     push ecx\n"
              "     push edx\n"
              "     mov eax, SYS_READ\n"
              "     mov ebx, STDIN\n"
              "     mov ecx, [ebp+8]\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     pop edx\n"
              "     pop ecx\n"
              "     pop ebx\n"
              "     leave\n"
              "     ret 4\n"
              "\n"
              "Escrever:\n"
              "     enter 0,0\n"
              "     push eax\n"
              "     push ebx\n"
              "     push ecx\n"
              "     push edx\n"
              "     mov eax, SYS_WRITE\n"
              "     mov ebx, STDOUT\n"
              "     mov ecx, dword [ebp+8]\n"
              "     mov edx, 1\n"
              "     int 0x80\n"
              "     pop edx\n"
              "     pop ecx\n"
              "     pop ebx\n"
              "     pop eax\n"
              "     leave\n"
              "     ret 4\n"
              "\n"
              "swap:\n"
              "     enter 0, 0\n"
              "     pusha\n"
              "     mov eax, [ebp+12]\n"
              "     mov ecx, [ebp+8]\n"
              "     add ecx, eax\n"
              "     loopSwap: cmp eax, ecx\n"
              "     jae endSwap\n"
              "     mov bl, [eax]\n"
              "     mov dl, [ecx]\n"
              "     mov [eax], dl\n"
              "     mov [ecx], bl\n"
              "     inc eax\n"
              "     dec ecx\n"
              "     jmp loopSwap\n"
              "     endSwap:\n"
              "     popa\n"
              "     leave   \n"
              "     ret 8\n"
              "\n";
}
