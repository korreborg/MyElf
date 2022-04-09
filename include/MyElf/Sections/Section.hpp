#pragma once

#include "SectionHeader.hpp"

class Section
{
  public:
    SectionHeader Header;

  public:
    Section(SectionHeader header={}) : Header{header} {}
    virtual ~Section() = default;

    virtual void Read(std::istream& stream, bool elf64) = 0;
};


