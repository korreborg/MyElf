#pragma once

#include "../Common.hpp"
#include "./Section.hpp"

class StringTable : public Section
{
  public:
    std::vector<U8> Buffer;

  public:
    using Section::Section;

    const char* GetStr(U32 offset)
    {
      if(offset >= Buffer.size())
        return "\0";

      return (const char*)Buffer.data()+offset;
    }

    void Read(std::istream& stream, bool elf64) override
    {
      (void)elf64;

      Buffer.resize(Section::Header.Size);
      stream.read((char*)Buffer.data(), Buffer.size());
    }

};

