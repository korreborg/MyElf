#pragma once


#include "ElfHeader.hpp"
#include "Sections/Sections.hpp"
#include "Segments/Segments.hpp"
#include "Common.hpp"


struct Elf
{
  ElfHeader Header;
  std::vector< SPtr<Section> > Sections;
  std::vector< SPtr<Segment> > Segments;

  SPtr<Section> FindSectionByName(const char* name) const
  {
    for(auto section : this->Sections)
    {
      std::string secName = this->FindSectionName(section->Header.NameOffset);

      if(secName == name)
        return section;
    }

    return nullptr;
  }

  std::string FindSectionName(SPtr<Section> pSection, std::string noname="null") const
  {
    return this->FindSectionName(pSection->Header.NameOffset, noname);
  }

  std::string FindSectionName(U32 offset, std::string noname="null") const
  {
    SPtr<StringTable> strtab = this->GetSectionT<StringTable>(
                                                   this->Header.SectionHeaderStrIndex);

    if(strtab == nullptr)
      return noname;

    return std::string(strtab->GetStr(offset));

  }

  template <typename T>
  SPtr<T> GetSectionT(size_t index) const
  {
    if(Sections.size() <= index)
      return nullptr;

    return std::dynamic_pointer_cast<T>(Sections[index]);
  }

  void Read(std::istream& stream)
  {
    this->Header.Read(stream);
    readSections(stream);
    readSegments(stream);

  }

  private:

  void readSegments(std::istream& stream)
  {
    //pre-resize vector to needed size
    this->Segments.reserve(this->Header.NProgramEntries);

    //loop over segment entries
    for(size_t i = 0; i < this->Header.NProgramEntries; i++)
    {
      //seek to header offset
      stream.seekg(this->Header.ProgramHeaderTableOffset + 
                   (i*this->Header.ProgramHeaderSize));

      //read header
      SegmentHeader segHeader;
      segHeader.Read(stream, this->Header.Elf64());

      //seek to segment offset
      stream.seekg(segHeader.Offset);

      //switch over segment type and allocate corrosponding section into vector
      switch(segHeader.Type)
      {
         case PT_INTERP:
         {
           this->Segments.emplace_back( 
                           std::make_shared<InterpSegment>(std::move(segHeader))
                           );
         }break;
         default:
         {
           this->Segments.emplace_back( 
                              std::make_shared<RawSegment>(std::move(segHeader))
                              );
         }break;
      }


      //read newly allocated segment
      this->Segments.back()->Read(stream, this->Header.Elf64());
    }

  }

  void readSections(std::istream& stream)
  {

    //pre-resize vector to needed size
    this->Sections.reserve(this->Header.NSectionEntries);

    //loop over segment entries
    for(size_t i = 0; i < this->Header.NSectionEntries; i++)
    {
      //seek to header offset
      stream.seekg(this->Header.SectionHeaderTableOffset + 
                   (i*this->Header.SectionHeaderSize));

      //read header
      SectionHeader secHeader;
      secHeader.Read(stream, this->Header.Elf64());

      //seek to section offset
      stream.seekg(secHeader.Offset);

      //switch over section type and allocate corrosponding section into vector
      switch(secHeader.Type)
      {
         case SHT_STRTAB:
         {
           this->Sections.emplace_back( 
                             std::make_shared<StringTable>(std::move(secHeader))
                             );
         }break;
         case SHT_SYMTAB:
         {
           this->Sections.emplace_back( 
                             std::make_shared<SymbolTable>(std::move(secHeader))
                             );
         }break;
         case SHT_REL:
         case SHT_RELA:
         {
           this->Sections.emplace_back( 
                              std::make_shared<RelocTable>(std::move(secHeader))
                              );
         }break;
         default:
         {
           this->Sections.emplace_back( 
                              std::make_shared<RawSection>(std::move(secHeader))
                              );
         }break;
      }


      //read newly allocated section
      this->Sections.back()->Read(stream, this->Header.Elf64());


    }

  }

};
