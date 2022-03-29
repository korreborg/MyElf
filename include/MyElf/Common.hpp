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

#define MYELF_READ(stream, member) stream.read((char*)&(member), sizeof(member))


