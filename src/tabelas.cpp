#include "../include/tabelas.hpp"
#include <boost/algorithm/string.hpp>

void Tables::AddElementSymbolTable(std::string symbol, int addr, std::string section, bool isVariable, int value, bool isZero, bool isConst, bool isVector, int vectorSize)
{
    if (symbol.back() == ':')
        symbol.pop_back();

    if (IsSymbolInSymbolTable(symbol))
    {
        std::cout << "Simbolo ja esta na tabela!" << std::endl;
        return;
    }
    _symbols.push_back(SymbolTable{symbol, addr, section, isVariable, value, isZero, isConst, isVector, vectorSize});
}

int Tables::GetSymbolAddr(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].addr;
        }
    }

    return -1;
}

int Tables::GetSymbolValue(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].value;
        }
    }

    return -1;
}

int Tables::GetVectorSize(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].vectorSize;
        }
    }

    return -1;
}

bool Tables::IsSymbolValueZero(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].isZero;
        }
    }
}

bool Tables::IsSymbolConst(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].isConst;
        }
    }
}

bool Tables::IsSymbolVariable(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].isVariable;
        }
    }
}

bool Tables::IsSymbolVector(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
        {
            return _symbols[i].isVector;
        }
    }
}

bool Tables::IsSymbolInSymbolTable(std::string symbol)
{
    if (symbol.back() == ',')
        symbol.pop_back();

    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
            return true;
    }

    return false;
}

std::string Tables::SymbolSection(std::string symbol)
{
    for (int i = 0; i < _symbols.size(); i++)
    {
        if (boost::iequals(symbol, _symbols[i].symbol))
            return _symbols[i].section;
    }
}
