#pragma once

#include "./Segment.hpp"
#include "../Common.hpp"

class RawSegment : public Segment
{
  public:
    std::vector<U8> Bytes;

  public:
    using Segment::Segment;

    void Read(std::istream& stream) override
    {
      (void)stream;
      MYELF_ASSERT(false, "not implemented");
    }

};

