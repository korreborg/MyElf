#pragma once

#include "TableSection.hpp"

#define ELF32_R_SYM(info)             ((info)>>8)
#define ELF32_R_TYPE(info)            ((unsigned char)(info))
#define ELF32_R_INFO(sym, type)       (((sym)<<8)+(unsigned char)(type))

#define ELF64_R_SYM(info)             ((info)>>32)
#define ELF64_R_TYPE(info)            ((Elf64_Word)(info))
#define ELF64_R_INFO(sym, type)       (((Elf64_Xword)(sym)<<32) + \
                                       (Elf64_Xword)(type))

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

