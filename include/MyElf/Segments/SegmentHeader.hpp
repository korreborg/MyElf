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

  void Read(std::istream& stream, bool elf64)
  {
    MYELF_READ_DIFF(stream, this->Type, U32, elf64);
    MYELF_READ_DIFF(stream, this->Offset, U32, elf64);
    MYELF_READ_DIFF(stream, this->VAddress, U32, elf64);
    MYELF_READ_DIFF(stream, this->PAddress, U32, elf64);
    MYELF_READ_DIFF(stream, this->Filesz, U32, elf64);
    MYELF_READ_DIFF(stream, this->Memsz, U32, elf64);
    MYELF_READ_DIFF(stream, this->Flags, U32, elf64);
    MYELF_READ_DIFF(stream, this->Align, U32, elf64);
  }

};
using ProgramHeader = SegmentHeader;
