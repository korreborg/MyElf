#pragma once

#include "../Common.hpp"
#include "SectionHeader.hpp"

template <typename EntryT>
struct TableSection : public SectionHeader
{
  std::vector<EntryT> Entries;

  virtual void ReadEntry(EntryT& t, std::istream& stream, bool elf64) = 0;

  void Read(std::istream& stream, bool elf64)
  {
    SectionHeader::Read(stream, elf64);
    stream.seekg(SectionHeader::Offset);

    if(SectionHeader::EntSize == 0)
      return;

    this->Entries.resize(SectionHeader::Size / SectionHeader::EntSize);

    for(EntryT& entry : this->Entries)
    {
      auto p1 = stream.tellg();
      this->ReadEntry(entry, stream, elf64);
      auto p2 = stream.tellg();

      U64 nRead = p2-p1;
      assert(nRead <= SectionHeader::EntSize);

      if(nRead < SectionHeader::EntSize)
      {
        stream.seekg(SectionHeader::EntSize - nRead,
                     std::ios_base::cur);
      }

    }

  }

};

