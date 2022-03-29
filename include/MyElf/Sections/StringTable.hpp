#pragma once

#include "../Common.hpp"
#include "./SectionHeader.hpp"

struct StringTable : public SectionHeader
{
  std::vector<U8> Buffer;

  const char* GetStr(U32 offset)
  {
    if(offset >= Buffer.size())
      return "\0";

    return (const char*)Buffer.data()+offset;
  }

  void Read(std::istream& stream, bool elf64)
  {
    SectionHeader::Read(stream, elf64);

    stream.seekg(SectionHeader::Offset, std::ios_base::beg);

    Buffer.resize(SectionHeader::Size);
    stream.read((char*)Buffer.data(), Buffer.size());
  }

};

