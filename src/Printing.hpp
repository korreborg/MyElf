#pragma once

#include<iostream>
#include<iomanip>
#include<cstring>


#define PRINT_VAR(var) PrintVarT{#var, var}

class PrintVar
{
  public:
    PrintVar(const char* varName): m_varName{varName} {}
    virtual ~PrintVar() = default;

    virtual void PrintName()  const
    {
      std::cout << m_varName;
    }

    virtual void PrintValue() const = 0;

    virtual int GetNameLen() const
    {
      return strlen(m_varName);
    }

  private:
    const char* m_varName;

};

/*
template <typename T>
class PrintFormat
{
  public:
    virtual void Print(const T& value)
    {
      std::cout << value;
    }

};

class AlphaOrHex : public PrintFormat
{
  public:

};
*/

template <typename T>
class PrintVarT : public PrintVar
{
  public:
    PrintVarT(const char* varName, const T& var) //, const PrintFormat& format)
    :PrintVar{varName}, m_var{var} {}
    virtual ~PrintVarT() = default;

    virtual void PrintValue() const override
    {
      size_t size = sizeof(m_var);
      std::cout << "0x" << std::setw(size) << std::setfill('0') << std::hex;
      std::cout << (int)m_var;
      std::cout << std::setfill(' ') << std::dec;
    }

  private:
    const T& m_var;
};

template <typename T>
using uPtr = std::unique_ptr<T>;

class PrintVarGroup
{
  public:

    template<class... TArgs>
    PrintVarGroup(const char* groupName, TArgs...args)
    {
      /*
      (void)groupName;
      for(auto& printVar : printVars)
      {
        printVar->PrintName();
        std::cout << "         ";
        printVar->PrintValue();
        std::cout << std::endl;
      }
      */

      std::cout << groupName << '\n';
      (findLongestName(args), ...);
      (printVar(args), ...);
    }

    virtual ~PrintVarGroup() = default;

  private:
    void printVar(const PrintVar& printVar)
    {
      std::cout << std::string(m_preIndent, ' ');
      printVar.PrintName();
      std::cout << ": ";

      int indentBeforeValue = m_longestNameLen - printVar.GetNameLen();
      std::cout << std::string(indentBeforeValue, ' ');

      printVar.PrintValue();
      std::cout << std::endl;
    }

    void findLongestName(const PrintVar& printVar)
    {
      int len = printVar.GetNameLen();
      if(len > m_longestNameLen)
        m_longestNameLen = len;
    }

  private:
    int m_longestNameLen{0};
    const int m_preIndent{2};

};

/*
template <typename T>
void PrintVar(const char* name, const T var)
{
  std::cout << "  " << name << ":\t" << (int)var << std::endl;
}

*/

