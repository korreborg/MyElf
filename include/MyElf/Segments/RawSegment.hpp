#pragma once

#include "Segment.hpp"
#include "../Common.hpp"

class RawSegment : public Segment
{
  public:
    std::vector<U8> Bytes;

  public:
    using Segment::Segment;

    void Read(std::istream& stream, bool elf64) override
    {
      (void)stream;
      (void)elf64;
      MYELF_ASSERTW(false, "not implemented");
    }

};

