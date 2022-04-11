#pragma once

#include "Common.hpp"

#define ELF_MAG0 0x7F
#define ELF_MAG1 0x45
#define ELF_MAG2 0x4C
#define ELF_MAG3 0x46

#define EV_CURRENT 1

#define ELFCLASSNONE 0
#define ELFCLASS32   1
#define ELFCLASS64   2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSG 2

#define ET_NONE 0
#define ET_REL  1 
#define ET_EXEC 2
#define ET_DYN  3 
#define ET_CORE 4
#define ET_LOPROC 0xFF00
#define ET_HIPROC 0xFFFF

#define EM_M32         1
#define EM_SPARC       2
#define EM_386         3
#define EM_68K         4
#define EM_88k         5
#define EM_860         7
#define EM_MIPS        8
#define EM_MIPS_RS4_BE 10

struct ElfHeader 
{
  //TODO: unionize
  struct Ident_t
  {
    U8 Mag0;
    U8 Mag1;
    U8 Mag2;
    U8 Mag3;
    U8 Class;
    U8 Data;
    U8 Version;
    U8 Padding;
    U8 Unused[7]; 
  } Ident;

  U16 Type;
  U16 Machine;
  U32 Version;
  U64 Entry;
  U64 ProgramHeaderTableOffset;
  U64 SectionHeaderTableOffset;
  U32 Flags;
  U16 HeaderSize;
  U16 ProgramHeaderSize;
  U16 NProgramEntries;
  U16 SectionHeaderSize;
  U16 NSectionEntries;
  U16 SectionHeaderStrIndex;

  bool Elf64() const
  {
    return (Ident.Class == ELFCLASS64);
  }

  void Read(std::istream& stream)
  {
    MYELF_READ(stream, this->Ident.Mag0);
    MYELF_READ(stream, this->Ident.Mag1);
    MYELF_READ(stream, this->Ident.Mag2);
    MYELF_READ(stream, this->Ident.Mag3);
    MYELF_READ(stream, this->Ident.Class);
    MYELF_READ(stream, this->Ident.Data);
    MYELF_READ(stream, this->Ident.Version);
    MYELF_READ(stream, this->Ident.Padding);
    MYELF_READ(stream, this->Ident.Unused[0]);
    MYELF_READ(stream, this->Ident.Unused[1]);
    MYELF_READ(stream, this->Ident.Unused[2]);
    MYELF_READ(stream, this->Ident.Unused[3]);
    MYELF_READ(stream, this->Ident.Unused[4]);
    MYELF_READ(stream, this->Ident.Unused[5]);
    MYELF_READ(stream, this->Ident.Unused[6]);
    MYELF_READ(stream, this->Ident.Unused[7]);

    MYELF_READ(stream, this->Type);
    MYELF_READ(stream, this->Machine);
    MYELF_READ(stream, this->Version);

    MYELF_READ_DIFF(stream, this->Entry, U32, this->Elf64());
    MYELF_READ_DIFF(stream, this->ProgramHeaderTableOffset, U32, this->Elf64());
    MYELF_READ_DIFF(stream, this->SectionHeaderTableOffset, U32, this->Elf64());

    MYELF_READ(stream, this->Flags);
    MYELF_READ(stream, this->HeaderSize);
    MYELF_READ(stream, this->ProgramHeaderSize);
    MYELF_READ(stream, this->NProgramEntries);
    MYELF_READ(stream, this->SectionHeaderSize);
    MYELF_READ(stream, this->NSectionEntries);
    MYELF_READ(stream, this->SectionHeaderStrIndex);

  }
};
