#pragma once

#include "Flags.hpp"

#include <vector>

namespace Args
{

class Parser
{
  public:
    Parser(std::initializer_list<Flag*> flags): m_flags{flags} {}

    ~Parser() = default;

    void ParseFlags(int argc, char** argv)
    {


      for(int i = 0; i < argc; i++)
      {
        if(argv[i][0] != '-')
          continue;

        for(Flag* pFlag : m_flags)
        {
          if(pFlag->Name == argv[i] || pFlag->Alias == argv[i])
          {
            pFlag->Set = true;

            if(i+1 >= argc)
              break;

            if(dynamic_cast<StrFlag*>(pFlag) != nullptr)
            {
              dynamic_cast<StrFlag*>(pFlag)->Value = argv[i+1];
              i++;
            }

            if(dynamic_cast<IntFlag*>(pFlag) != nullptr)
            {
              dynamic_cast<IntFlag*>(pFlag)->Value = std::atoi(argv[i+1]);
              i++;
            }
          }

        }

      }

    }

    std::vector<Flag*>& GetFlags()
    {
      return this->m_flags;
    }


  private:
    std::vector<Flag*> m_flags;

};

}
