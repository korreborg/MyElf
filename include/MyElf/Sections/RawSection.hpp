#pragma once

#include "Section.hpp"

class RawSection : public Section
{
  public:
    std::vector<U8> Bytes;

  public:
    using Section::Section;

    virtual void Read(std::istream& stream, bool elf64)
    {
      (void)elf64;

      Bytes.resize(Section::Header.Size);
      stream.read((char*)Bytes.data(), Bytes.size());
    }
};


