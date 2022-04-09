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
      (void)elf64;

      Bytes.resize(Segment::Header.Filesz);
      stream.read((char*)Bytes.data(), Bytes.size());
    }

};

