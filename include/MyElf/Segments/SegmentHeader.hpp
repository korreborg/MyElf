#pragma once

#include "../Common.hpp"

#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7FFFFFFF

struct SegmentHeader
{
  U64 Type;
  U64 Offset;
  U64 VAddress;
  U64 PAddress;
  U64 Filesz;
  U64 Memsz;
  U64 Flags;
  U64 Align;

  virtual void Read(std::istream& stream, bool elf64)
  {
    if(!elf64)
    {
      U32 tmp;
      MYELF_READ(stream, tmp);
      this->Type = tmp;

      MYELF_READ(stream, tmp);
      this->Offset = tmp;

      MYELF_READ(stream, tmp);
      this->VAddress = tmp;

      MYELF_READ(stream, tmp);
      this->PAddress = tmp;

      MYELF_READ(stream, tmp);
      this->Filesz = tmp;

      MYELF_READ(stream, tmp);
      this->Memsz = tmp;

      MYELF_READ(stream, tmp);
      this->Flags = tmp;

      MYELF_READ(stream, tmp);
      this->Align = tmp;
    }
    else
    {
      MYELF_READ(stream, this->Type);
      MYELF_READ(stream, this->Offset);
      MYELF_READ(stream, this->VAddress);
      MYELF_READ(stream, this->PAddress);
      MYELF_READ(stream, this->Filesz);
      MYELF_READ(stream, this->Memsz);
      MYELF_READ(stream, this->Flags);
      MYELF_READ(stream, this->Align);
    }

  }

};
using ProgramHeader = SegmentHeader;
