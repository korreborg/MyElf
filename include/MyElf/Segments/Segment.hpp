#pragma once

#include "SegmentHeader.hpp"

class Segment
{
  public:
    SegmentHeader Header;

  public:
    Segment(SegmentHeader header={}) : Header{header} {}
    virtual ~Segment() = default;

    virtual void Read(std::istream& stream, bool elf64) = 0;
};


