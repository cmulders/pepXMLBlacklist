## Synopsis

pepXMLBlacklist uses a plain text list of peptides that will be filtered from the selected pepXML file. 
It can be used to filter out any contamination, or in my project, shared peptides from different proteins. 
The resulting peptides will therefore be unique. The peptides could be generated in-silico from a UniProt FASTA proteome with my other tool [https://github.com/cmulders/Protein-Digester].

## Installation

Requirements:
- CMAKE build automation
- Pugixml (included)

Use the available macros in the Makefile or use the cmake itself:

$ mkdir build; cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make install

## API Reference

The filterPeptides executable is located in the 'bin' folder. Use filterPeptides -h to view the available options and description.

## Third party licenses

### Pugixml v1.5

Repository: http://pugixml.org/

Copyright (C) 2006-2014, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
Report bugs and download new versions at http://pugixml.org/

This library is distributed under the MIT License:

Copyright (c) 2006-2014 Arseny Kapoulkine

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.