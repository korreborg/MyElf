#pragma once

#include "./TableSection.hpp"

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
    if(!elf64)
    {
      U32 tmp;
      MYELF_READ(stream, tmp);
      entry.Offset = tmp;
      MYELF_READ(stream, tmp);
      entry.Info = tmp;

      if(SectionHeader::Type == SHT_RELA)
      {
        MYELF_READ(stream, tmp);
        entry.Addend = tmp;
      }

      return;
    }

    MYELF_READ(stream, entry.Offset);
    MYELF_READ(stream, entry.Info);

    if(SectionHeader::Type == SHT_RELA)
      MYELF_READ(stream, entry.Addend);

  }
};

