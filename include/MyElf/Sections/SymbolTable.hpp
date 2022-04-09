#pragma once

#include "TableSection.hpp"

#define ELF32_ST_BIND(i)   ( (i)>>4 )
#define ELF32_ST_TYPE(i)   ( (i)&0xF )
#define ELF32_ST_INFO(b,t) ( ((b)<<4) + ((t)&0xF) )

//symbol bindings
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2  
#define STB_LOOS   10
#define STB_LOPROC 13
#define STB_HIPROC 15

//symbol types
#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_COMMON  5
#define STT_TLS     6
#define STT_LOOS    10
#define STT_HIOS    12
#define STT_LOPROC  13
#define STT_HIPROC  15

#define ELF32_ST_VISIBILITY(o) ((o)&0x3)

#define STV_DEFAULT   0
#define STV_INTERNAL  1
#define STV_HIDDEN    2
#define STV_PROTECTED 3
#define STV_EXPORTED  4
#define STV_SINGLETON 5
#define STV_ELIMINATE 6

struct SymbolEntry
{
  //why 32? i have no clue. 
  //Every spec i've read says this variable should be 64 bits when aprsing elf64 
  //objects, but it literally isn't. In every 64 bit elf object i've parsed its
  //32 bits
  U32 NameOffset;
  U8  Info;
  U8  Other;

  //again why 16 bit when every spec says it should be 32? 
  //same reason as NameOffset...
  U16 SectionHeaderIndex;
  U64 Value;
  U64 Size;
};

struct SymbolTable : public TableSection<SymbolEntry>
{
  void ReadEntry(SymbolEntry& entry, std::istream& stream, bool elf64)
  {
    MYELF_READ(stream, entry.NameOffset);

    if(!elf64)
    {
      MYELF_READ_DIFF(stream, entry.Value, U32, elf64);
      MYELF_READ_DIFF(stream, entry.Size, U32, elf64);
      MYELF_READ(stream, entry.Info);
      MYELF_READ(stream, entry.Other);
      MYELF_READ_DIFF(stream, entry.SectionHeaderIndex, U8, elf64);
    }
    else
    {
      MYELF_READ(stream, entry.Info);
      MYELF_READ(stream, entry.Other);
      MYELF_READ(stream, entry.SectionHeaderIndex);
      MYELF_READ(stream, entry.Value);
      MYELF_READ(stream, entry.Size);
    }

  }

};

