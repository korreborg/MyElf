#pragma once

#include "TableSection.hpp"

struct RelocEntry
{
  U64 Offset;
  U64 Info;
  U64 Addend;
};

class RelocTable : public TableSection<RelocEntry>
{
  public:
    using TableSection<RelocEntry>::TableSection;

  protected:
    void ReadEntry(RelocEntry& entry, std::istream& stream, bool elf64) override
    {
      MYELF_READ_DIFF(stream, entry.Offset, U32, elf64);
      MYELF_READ_DIFF(stream, entry.Info, U32, elf64);

      if(this->Header.Type == SHT_RELA)
        MYELF_READ_DIFF(stream, entry.Addend, U32, elf64);

    }
};

