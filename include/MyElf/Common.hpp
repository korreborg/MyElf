#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <memory>
#include <cstdint>

using S32 = std::int32_t;

using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

template <typename T>
using SPtr = std::shared_ptr<T>;

//For simply reading variables when the size is fixed regardless of elf64
#define MYELF_READ(stream, var) stream.read((char*)&(var), sizeof(var))

//For reading variables when the size differ depending on elf64
#define MYELF_READ_DIFF(stream, var, t32, elf64) \
                         myelf_read_diff<decltype(var), t32>(stream, var, elf64)

template <typename VarT, typename T32>
constexpr
void myelf_read_diff(std::istream& stream, VarT& var, bool elf64)
{
  //if elf64 just read the full size of var
  if(elf64)
  {
    stream.read((char*)&var, sizeof(var));
    return;
  }

  //when not elf64, read sizeof(T32) bytes and cast to VarT
  T32 tmp;
  stream.read((char*)&tmp, sizeof(tmp));

  var = tmp;
}


