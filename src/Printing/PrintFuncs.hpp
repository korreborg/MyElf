#pragma once

#include <MyElf/Elf.hpp>

#include "PrintVar.hpp"

#include <iostream>
#include <string>


void PrintRelocTable(const Elf& elf)
{
  int i = -1;
  for(auto pSec : elf.Sections)
  {
    i++;
    SPtr<RelocTable> pRelocTab= std::dynamic_pointer_cast<RelocTable>(pSec);

    //skip if section is not a reloc table
    if(pRelocTab== nullptr)
      continue;

    std::string relocName = elf.FindSectionName(pRelocTab,
                                    std::string("Section #")+std::to_string(i));

    std::cout << relocName<< ":\n";
    std::cout << "--------------------\n";

    SPtr<SymbolTable> pSymTab = std::dynamic_pointer_cast<SymbolTable>(
                                              elf.FindSectionByName(".symtab"));
    SPtr<StringTable> pStrTab = std::dynamic_pointer_cast<StringTable>(
                                              elf.FindSectionByName(".strtab"));


    int entryI = 0;
    for(const RelocEntry& entry : pRelocTab->Entries)
    {
      std::string entryName{"Reloc #"};
      entryName += std::to_string(entryI);

      if(pSymTab != nullptr && pStrTab != nullptr)
      {
        int index = elf.Header.Elf64() ? 
                    ELF64_R_SYM(entry.Info) : ELF32_R_SYM(entry.Info);

        std::string symbolName = 
          std::string(pStrTab->GetStr(pSymTab->Entries[index].NameOffset));

        entryName += std::string(": ") + symbolName;
      }


      PrintVarGroup
      {
        entryName.c_str(),
        PRINT_VAR(entry.Offset),
        PRINT_VAR(entry.Info),
        PRINT_VAR(entry.Addend),
      };
      std::cout << std::endl;

      entryI++;
    }

  }


}

void PrintInterpreter(const Elf& elf)
{
  int i = -1;
  for(auto pSeg : elf.Segments)
  {
    i++;

    SPtr<InterpSegment> interp = std::dynamic_pointer_cast<InterpSegment>(pSeg);

    //skip if segment is not interp segment
    if(interp == nullptr)
      continue;

    std::cout << "Segment #" << i << " [PT_INTERP]:\n";
    std::cout << "  InterpreterPath: " << interp->InterpreterPath << std::endl;

  }
}

void PrintSymbolTables(const Elf& elf)
{
  int i = -1;
  for(auto section : elf.Sections)
  {
    i++;
    SPtr<SymbolTable> symt = std::dynamic_pointer_cast<SymbolTable>(section);

    //skip if section is not a symbol table
    if(symt == nullptr)
      continue;

    std::string symtName = elf.FindSectionName(section,
                                    std::string("Section #")+std::to_string(i));

    std::cout << symtName << ":\n";
    std::cout << "--------------------\n";


    int symi = 0;
    for(const SymbolEntry& symbol: symt->Entries)
    {
      SPtr<StringTable> symStrTable = 
        std::dynamic_pointer_cast<StringTable>(elf.FindSectionByName(".strtab"));

      std::string groupName = std::string("Symbol #") + std::to_string(symi++) +
                           ": " + (symStrTable == nullptr ? "" : 
                           std::string(symStrTable->GetStr(symbol.NameOffset)));

      PrintVarGroup
      {
        groupName.c_str(),
        PRINT_VAR(symbol.NameOffset),
        PRINT_VAR(symbol.Info),
        PRINT_VAR(symbol.Other),
        PRINT_VAR(symbol.SectionHeaderIndex),
        PRINT_VAR(symbol.Value),
        PRINT_VAR(symbol.Size),
      };
      std::cout << std::endl;

    }

  }

}

void PrintProgramHeaderTable(const Elf& elf)
{
  int i = 0;
  for(auto segment : elf.Segments)
  {
    std::string groupName = "Segment #" + std::to_string(i++);

    PrintVarGroup
    {
      groupName.c_str(),
      PRINT_VAR(segment->Header.Type),
      PRINT_VAR(segment->Header.Offset),
      PRINT_VAR(segment->Header.VAddress),
      PRINT_VAR(segment->Header.PAddress),
      PRINT_VAR(segment->Header.Filesz),
      PRINT_VAR(segment->Header.Memsz),
      PRINT_VAR(segment->Header.Flags),
      PRINT_VAR(segment->Header.Align),
    };
    std::cout << std::endl;

  }
}

void PrintSectionHeaderTable(const Elf& elf)
{
  SPtr<StringTable> secStrTable = elf.GetSectionT<StringTable>(
                                                elf.Header.SectionHeaderStrIndex
                                                );

  int i = 0;

  for(auto section : elf.Sections)
  {
    std::string groupName = "Section #" + std::to_string(i) + 
      (secStrTable != nullptr? 
      ": " + std::string(secStrTable->GetStr(section->Header.NameOffset)) : "");
     

    PrintVarGroup
    {
      groupName.c_str(),
      PRINT_VAR(section->Header.NameOffset),
      PRINT_VAR(section->Header.Type),
      PRINT_VAR(section->Header.Flags),
      PRINT_VAR(section->Header.Address),
      PRINT_VAR(section->Header.Offset),
      PRINT_VAR(section->Header.Size),
      PRINT_VAR(section->Header.Link),
      PRINT_VAR(section->Header.Info),
      PRINT_VAR(section->Header.AddrAlign),
      PRINT_VAR(section->Header.EntSize),
    };
    std::cout << std::endl;
    i++;
  }


}

void PrintElfHeader(const ElfHeader& header)
{
  PrintVarGroup
  {
    "Elf Header",
    PRINT_VAR(header.Ident.Mag0),
    PRINT_VAR(header.Ident.Mag1),
    PRINT_VAR(header.Ident.Mag2),
    PRINT_VAR(header.Ident.Mag3),
    PRINT_VAR(header.Ident.Class),
    PRINT_VAR(header.Ident.Data),
    PRINT_VAR(header.Ident.Version),
    PRINT_VAR(header.Type),
    PRINT_VAR(header.Machine),
    PRINT_VAR(header.Version),
    PRINT_VAR(header.Entry),
    PRINT_VAR(header.ProgramHeaderTableOffset),
    PRINT_VAR(header.SectionHeaderTableOffset),
    PRINT_VAR(header.Flags),
    PRINT_VAR(header.HeaderSize),
    PRINT_VAR(header.ProgramHeaderSize),
    PRINT_VAR(header.NProgramEntries),
    PRINT_VAR(header.SectionHeaderSize),
    PRINT_VAR(header.NSectionEntries),
    PRINT_VAR(header.SectionHeaderStrIndex),
  };
}
