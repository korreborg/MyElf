#pragma once


#include "./ElfHeader.hpp"
#include "./Sections/Sections.hpp"
#include "./Segments/Segments.hpp"
#include "./Common.hpp"


struct Elf
{
  ElfHeader Header;

  std::vector< SPtr<SectionHeader> > Sections;
  std::vector< SPtr<SegmentHeader> > Segments;

  SPtr<SectionHeader> GetSectionByName(const char* name) const
  {
    for(auto secHeader : this->Sections)
    {
      std::string secName = this->GetSectionHeaderName(secHeader->NameOffset);

      if(secName == name)
        return secHeader;
    }

    return nullptr;
  }

  std::string GetSectionHeaderName(U32 offset, std::string noname="null") const
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
    this->Segments.reserve(this->Header.NProgramEntries);

    for(size_t i = 0; i < this->Header.NProgramEntries; i++)
    {
      stream.seekg(this->Header.ProgramHeaderTableOffset + 
                   (i*this->Header.ProgramHeaderSize));

      //TODO: iterate type like readSections
      this->Segments.emplace_back(
                                  std::make_shared<SegmentHeader>()
                                 );

      this->Segments.back()->Read(stream, this->Header.Elf64());

    }

  }

  void readSections(std::istream& stream)
  {

    this->Sections.reserve(this->Header.NSectionEntries);

    for(size_t i = 0; i < this->Header.NSectionEntries; i++)
    {
      stream.seekg(this->Header.SectionHeaderTableOffset + 
                   (i*this->Header.SectionHeaderSize));


      //this is cringe but i have to know the type of the section
      //before i initialize the object
      //(NameIndex comes before type)
      U32 SectionNameIndex;
      U32 SectionType;
      MYELF_READ(stream, SectionNameIndex);
      MYELF_READ(stream, SectionType);

      //lole
      stream.seekg(-8, std::ios_base::cur);

      switch(SectionType)
      {
         case SHT_STRTAB:
         {
           this->Sections.emplace_back( 
                                       std::make_shared<StringTable>()
                                      );
         }break;
         case SHT_SYMTAB:
         {
           this->Sections.emplace_back( 
                                       std::make_shared<SymbolTable>()
                                      );
         }break;
         case SHT_REL:
         case SHT_RELA:
         {
           this->Sections.emplace_back( 
                                       std::make_shared<RelocTable>()
                                      );
         }break;
         default:
         {
           this->Sections.emplace_back(
                                       std::make_shared<SectionHeader>()
                                      );
         }break;
      }

      this->Sections.back()->Read(stream, this->Header.Elf64());
    }

  }

};
