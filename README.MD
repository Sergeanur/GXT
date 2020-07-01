This repository contains decompiled GXT files and a compiler for them.</br>
Run `build_[game]_[platform].bat` to quickly build GXTs for your desired game.

## Decompiled GXT
The GXT files were decompiled into TXT using a syntax of an original source. The strings are mostly sorted in an order in which they were in the original source file instead of an alphabetical order (like most GXT Editors do). That benefits in having related strings grouped together and not scattered around (e.g. vehicle names).


All files where sorted into directories of their games and platforms accordingly.

## GXT Compiler
**gxt.exe** is a compiler for GXT files.</br>
The source code of it is in `compiler/ ` directory.

Currently supports:
* III EFIGS (including mobile)
* III Russian (Buka)
* III Polish
* VC EFIGS (including mobile)
* SA EFIGS
* LCS EFIGS
* VCS EFIGS
* IV (not fully tested but expected to produce compatible results)

***Parameters:***

-g [game]: game for which you're compiling a GXT file</br>
Supported values: III, VC, SA, LCS, VCS, IV

-m: flag for mobile VC support (does not required for III)


-i [input files]: list of txt files which you want to compile into GXT (can be one or multiple files)</br>
Supported codepages:</br>
- **UCS-2 LE BOM**</br>
- **UTF-8 BOM**</br>
- Windows-1252 (for EFIGS)</br>
- Windows-1251 (for russian)</br>
- Windows-1250 (for polish)

-o [output_file]: name of a GXT file that will be generated


***Optional parameters:***

-r: is russian text</br>
Currently supported only for III. Does nothing for the other games.

-p: is polish text</br>
Currently supported only for III. Does nothing for the other games.

-j: for future japanese support. Does nothing at this moment.