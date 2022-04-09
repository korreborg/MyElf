#pragma once

#include "TableSection.hpp"

//Relocation types
#define R


struct RelocEntry
{
  U64 Offset;
  U64 Info;
  U64 Addend;
};

struct RelocTable : public TableSection<RelocEntry>
{
  void ReadEntry(RelocEntry& entry, std::istream& stream, bool elf64)
  {
    MYELF_READ_DIFF(stream, entry.Offset, U32, elf64);
    MYELF_READ_DIFF(stream, entry.Info, U32, elf64);

    if(SectionHeader::Type == SHT_RELA)
      MYELF_READ_DIFF(stream, entry.Addend, U32, elf64);

  }
};

