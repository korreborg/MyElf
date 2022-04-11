#include <MyElf/Elf.hpp>

#include "Args/Flags.hpp"
#include "Args/Parser.hpp"
#include "Printing/PrintFuncs.hpp"

#include <iostream>
#include <fstream>
#include <chrono>

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
  Args::Flag interp{"--interpreter", "-i", "Print interpreter if present"};
  Args::Flag relocs{"--reloctable", "-r", "Print relocation tables"};

  Args::Parser parser{&help, &verbose, &elfHeader, &secHeaders, &progHeaders,
                      &symbols, &interp, &relocs};
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

  auto t1 = std::chrono::system_clock::now();
  elf.Read(file);
  auto t2 = std::chrono::system_clock::now();

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
  {
    std::cout << "[verbose] successfully parsed elf in "
    << (std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count() / 1000000.0f)
    << "ms" << std::endl;
  }

  if(elfHeader)
    PrintElfHeader(elf.Header);

  if(secHeaders)
    PrintSectionHeaderTable(elf);

  if(progHeaders)
    PrintProgramHeaderTable(elf);

  if(symbols)
    PrintSymbolTables(elf);

  if(interp)
    PrintInterpreter(elf);

  if(relocs)
    PrintRelocTable(elf);

  return 0;
}

