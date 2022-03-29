### MyElf
ELF CLI utility / header-only c++ library for reading ELF files

#### Building (reqs: c++17, cmake 3.8)

<code>mkdir build && cd build</code>

<code>cmake ..</code>

<code>cmake --build .</code>

#### CLI usage
Examine an ELF file by running the <code>myelf</code> program, passing a filename followed by one or more flags. <br/>
Ex: <code>./myelf [filename] -y </code> to print symbol table entries. <br/>
The available flags can be viewed by using the <code>--help</code> flag.
<br/><br/>
Example output of command <code>./myelf ./myelf -h</code>:
```
Elf Header
  header.Ident.Mag0:               0x7f
  header.Ident.Mag1:               0x45
  header.Ident.Mag2:               0x4c
  header.Ident.Mag3:               0x46
  header.Ident.Class:              0x2
  header.Ident.Data:               0x1
  header.Ident.Version:            0x1
  header.Type:                     0x03
  header.Machine:                  0x3e
  header.Version:                  0x0001
  header.Entry:                    0x000037c0
  header.ProgramHeaderTableOffset: 0x00000040
  header.SectionHeaderTableOffset: 0x00037c48
  header.Flags:                    0x0000
  header.HeaderSize:               0x40
  header.ProgramHeaderSize:        0x38
  header.NProgramEntries:          0x0d
  header.SectionHeaderSize:        0x40
  header.NSectionEntries:          0x21
  header.SectionHeaderStrIndex:    0x20
 ```
 
 #### Linking
To link with the library simply copy or add the <code>include/</code> directory to your own projects include path.<br/>
As of right now there are no cmake install targets so installing & adding a cmake dependency is not possible.