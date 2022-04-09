#include <MyElf/Elf.hpp>

#include "Args/Flags.hpp"
#include "Args/Parser.hpp"
#include "Printing.hpp"

#include <iostream>
#include <fstream>

void PrintSymbolTables(const Elf& elf)
{
  int i = 0;
  for(auto section : elf.Sections)
  {
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

    i++;
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

void PrintUsage(const char* progName)
{
  std::cout << "Usage: " << progName << " file <option(s)>\n";
}

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    PrintUsage(argv[0]);
    return -1;
  }

  Args::Flag help{"--help", "  ", "Display program options & usage"};
  Args::Flag verbose{false, "--verbose", "-v", "Prints additional debug information"};
  Args::Flag elfHeader{"--elfheader", "-h", "Print ELF file header"};
  Args::Flag secHeaders{"--secheaders", "-s", "Print section headers"};
  Args::Flag progHeaders{"--progheaders", "-p", "Print program (segment) headers"};
  Args::Flag symbols{"--symboltable", "-y", "Print symbol tables"};

  Args::Parser parser{&help, &verbose, &elfHeader, &secHeaders, &progHeaders,
                      &symbols};
  parser.ParseFlags(argc, argv);

  if(help)
  {
    PrintUsage(argv[0]);
    std::cout << " This program can be used to display the info stored in elf files.\n";
    std::cout << " Program options are:\n";

    for(Args::Flag* flag : parser.GetFlags())
    {
      std::cout << "  " << flag->Alias << ' ' << flag->Name << '\t' << flag->Desc << '\n';
    }

    std::cout << std::endl;
    return 0;
  }

  std::ifstream file{argv[1]};

  if(!file.good())
  {
    std::cout << "Error: failed to open file \"" << argv[1] << "\"\n";
    return -1;
  }

  Elf elf;
  elf.Read(file);

  if(elf.Header.Ident.Mag0 != 0x4f && elf.Header.Ident.Mag1 != 'E' &&
     elf.Header.Ident.Mag2 != 'L' && elf.Header.Ident.Mag3 != 'F')
  {
    std::cout << "Error: invalid elf file\n";

    if(verbose)
    {
      std::cout << "[verbose] invalid magic identity bytes: " << std::hex;
      std::cout << (int)elf.Header.Ident.Mag0 << ' ' << (int)elf.Header.Ident.Mag1 << ' ' 
                << (int)elf.Header.Ident.Mag2 << ' ' << (int)elf.Header.Ident.Mag3 << '\n';
    }

    return -1;
  }

  if(file.bad() || file.fail())
  {
    std::cout << "Error: failed to parse file (stream went bad).\n";
    return -1;
  }


  if(verbose)
    std::cout << "[verbose] successfully parsed elf\n";

  if(elfHeader)
    PrintElfHeader(elf.Header);

  if(secHeaders)
    PrintSectionHeaderTable(elf);

  if(progHeaders)
    PrintProgramHeaderTable(elf);

  if(symbols)
    PrintSymbolTables(elf);

  return 0;
}

