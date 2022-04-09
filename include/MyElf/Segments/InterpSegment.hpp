#pragma once

#include "Segment.hpp"
#include "../Common.hpp"

//This segment simply contains a string specifying the path to an interpreter 
//which is invoked by the program loader to help setup the process image.
//
//This segment is mandatory to have in for example dynamically linked executables
//as they need something to resolve dynamic symbols before execution is handed 
//over. Typically the path is set to the dynamic linker ld.so
//
//There can only be one segment of type PT_INTERP in a file and it must precede
//any loadable (PT_LOAD) segments
class InterpSegment : public Segment
{
  public:
    std::string InterpreterPath;

  public:
    using Segment::Segment;

    InterpSegment(SegmentHeader header, const char* interpreterPath) 
      : Segment{header}, InterpreterPath{interpreterPath} {}

    void Read(std::istream& stream, bool elf64) override
    {
      (void)elf64;

      //TODO: report warning if tellg + Filesz != \0 

      MYELF_ASSERTW(Segment::Header.Filesz == Segment::Header.Memsz, 
                    "Filesz & Memsz doesnt match!");

      this->InterpreterPath.resize(Segment::Header.Memsz);
      stream.read((char*)&(this->InterpreterPath[0]), this->InterpreterPath.size());
    }

};

