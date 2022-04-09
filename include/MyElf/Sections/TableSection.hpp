#pragma once

#include "../Common.hpp"
#include "Section.hpp"

template <typename EntryT>
class TableSection : public Section
{
  public:
    std::vector<EntryT> Entries;

  protected:
  virtual void ReadEntry(EntryT& t, std::istream& stream, bool elf64) = 0;

  public:
  using Section::Section;

  void Read(std::istream& stream, bool elf64) override
  {
    if(this->Header.EntSize == 0)
      return;

    this->Entries.resize(this->Header.Size / this->Header.EntSize);

    for(EntryT& entry : this->Entries)
    {
      auto p1 = stream.tellg();
      this->ReadEntry(entry, stream, elf64);
      auto p2 = stream.tellg();

      U64 nRead = p2-p1;
      MYELF_ASSERTW(nRead <= this->Header.EntSize, 
                   "read more bytes than specifie by header \"EntSize\"");

      //seek forward diff of nRead-EntSize if we read less than EntSize
      if(nRead < this->Header.EntSize)
      {
        stream.seekg(this->Header.EntSize - nRead,
                     std::ios_base::cur);
      }

    }

  }

};

