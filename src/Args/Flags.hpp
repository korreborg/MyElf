#pragma once

#include <string>

namespace Args
{

struct Flag
{
  std::string Name;
  std::string Alias;
  std::string Desc;
  bool Set;

  Flag(const char* name, const char* alias="", const char* desc=""):
    Name{name}, Alias{alias}, Desc{desc}, Set{false}
  {}

  Flag(bool set, const char* name, const char* alias="", const char* desc=""):
    Name{name}, Alias{alias}, Desc{desc}, Set{set}
  {}

  virtual ~Flag() = default;

  operator bool() const
  {
    return Set;
  }

};

struct StrFlag : public Flag
{
  std::string Value{""};

  using Flag::Flag;
};

struct IntFlag : public Flag
{
  int Value;

  using Flag::Flag;

  IntFlag(int value, const char* name, const char* alias="", const char* desc=""):
    Flag::Flag{name,alias,desc}, Value{value}
  {}

};

}
