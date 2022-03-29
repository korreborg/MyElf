#pragma once

#include "../Common.hpp"

//special section indexes
#define SHN_UNDEF     0x0
#define SHN_LORESERVE 0xFF00
#define SHN_LOPROC    0xFF00
#define SHN_HIPROC    0xFF1F
#define SHN_ABS       0xFFF1
#define SHN_COMMON    0xFFF2
#define SHN_HIRESERVE 0xFFFF

//Section types
#define SHT_NULL     0
#define SHT_PROGBITS 1
#define SHT_SYMTAB   2
#define SHT_STRTAB   3
#define SHT_RELA     4
#define SHT_HASH     5
#define SHT_DYNAMIC  6
#define SHT_NOTE     7
#define SHT_NOBITS   8
#define SHT_REL      9
#define SHT_SHLIB   10
#define SHT_DYNSYM  11
#define SHT_LOPROC  0x70000000
#define SHT_HIPROC  0x7FFFFFFF
#define SHT_LOUSER  0x80000000
#define SHT_HIUSER  0xFFFFFFFF

struct SectionHeader
{
  U32 NameOffset;
  U32 Type;
  U64 Flags;
  U64 Address;
  U64 Offset;
  U64 Size;
  U32 Link;
  U32 Info;
  U64 AddrAlign;
  U64 EntSize;

  virtual void Read(std::istream& stream, bool elf64) 
  {
    MYELF_READ(stream, this->NameOffset);
    MYELF_READ(stream, this->Type);

    MYELF_READ_DIFF(stream, this->Flags, U32, elf64);
    MYELF_READ_DIFF(stream, this->Address, U32, elf64);
    MYELF_READ_DIFF(stream, this->Offset, U32, elf64);
    MYELF_READ_DIFF(stream, this->Size, U32, elf64);

    MYELF_READ(stream, this->Link);
    MYELF_READ(stream, this->Info);

    MYELF_READ_DIFF(stream, this->AddrAlign, U32, elf64);
    MYELF_READ_DIFF(stream, this->EntSize, U32, elf64);

  }
};
