# ARINC 665 Tool Suite
![ProjectLogo.svg](ProjectLogo.svg)

Provides library and applications for generating, reading and handling ARINC 665 Media Sets.

This tool suite supports the generation and interpretation of media sets according to ARINC 665-2, ARINC 665-3, ARINC 665-4, and ARINC 665-5.

A library is provided for:
- generating and parsing ARINC 665 files
- media set model, and
- utilities for generation and management of media sets.

An XML format and corresponding tools are provided to ease generation of media sets.

For building graphical applications a Qt based library is provided which consist of models, dialogs, and wizards to work with ARINC 665 media sets.

## Dependencies
First level dependencies:
- [Helper Library](https://git.thomas-vogt.de/thomas-vogt/helper)
- [Qt Icon Resources](https://git.thomas-vogt.de/thomas-vogt/qt_icon_resources)
- [ARINC 645 Tool Suite](https://git.thomas-vogt.de/thomas-vogt/arinc_645)
- [Commands Library](https://git.thomas-vogt.de/thomas-vogt/commands)
- [Boost Libraries](https://www.boost.org/)
- libxml++
- [spdlog](https://github.com/gabime/spdlog)
- Optionally [Qt 6](https://www.qt.io/)

## Building
The library uses [CMake](https://cmake.org/) to handle build configuration.
CMake Presets are provided to generate builds compiling with:
- GNU GCC,
- Clang, and
- MSVC.

For each compiler and environment, the following variants can be built:
- Static debug,
- Static release,
- Shared debug, and
- Shared release.

Test Environments are:
- Linux,
- Windows MinGW, and
- Windows MSVC.

**Note:**
For managing dependencies, i.e. Windows MSVC, a VCPKG configuration is provided.

## License
This project is licensed under the terms of the [*Mozilla Public License Version 2.0* (MPL)](LICENSE).

## Changes
This section only contains changes within the standards, which are respected within this library.

### ARINC 665-1
- Update file versions (Separate between media files, load files)
- FILES.LUM: Add filename and file length field
- Add optional batch files (.LUB)
- Add optional list of batch files (BATCHES.LUM)

### ARINC 665-2
- Update File Versions (Separate between media files, load files, and batch files)
- Load header file (.LUH): add `spare field` after file length and file format version to align following fields to 32 bit
- Batch file (.LUB): add `spare field` after file length and file format version to align following fields to 32 bit
- LOADS.LUM file: add `spare field` after file length and file format version to align following fields to 32 bit
- FILES.LUM file: add `spare field` after file length and file format version to align following fields to 32 bit
- BATCHES.LUM file: add `spare field` after file length and file format version to align following fields to 32 bit

### ARINC 665-3
- Update File Versions (Separate between media files, load files, and batch files)
- Load header file (.LUH)
  - Change *Spare* field to *Part Flags* field
  - Add *Load Check Value*
  - Add *Load Type Section* (ID + description)
  - Add *Target Hardware IDs with Positions* section
  - Add Data File *length in bytes* field
  - Add *Data and Support File Check Value* field
- FILES.LUM add *File Check Value*

### ARINC 665-4
Clarification but no changes of data formats.

### ARINC 665-5
Move Checksum and Check Value definition to ARINC 645.
No changes of data format.

## Python Implementation

This project now includes a Python implementation that provides equivalent functionality to the C++ library and applications. The Python version supports the same ARINC 665 standards (Supplements 2, 3, 4, 5) and provides a more accessible interface for Python developers.

### Python Dependencies
- Python 3.11 or higher
- No external dependencies required for basic functionality
- Optional: `lxml` for enhanced XML processing

### Python Applications

The following command-line applications are available:

#### arinc_665_ls.py
Lists ARINC 665 files and their contents from directories.
```bash
python3 python/apps/arinc_665_ls.py --directory /path/to/media/set
python3 python/apps/arinc_665_ls.py --file /path/to/FILES.LUM
```

#### arinc_665_print_xml.py
Loads and prints Media Set information from XML files.
```bash
python3 python/apps/arinc_665_print_xml.py --xml-file mediaset.xml
```

#### arinc_665_media_set_check.py
Validates ARINC 665 Media Sets.
```bash
python3 python/apps/arinc_665_media_set_check.py --medium-directory /path/to/medium1
```

#### arinc_665_print_media_set.py
Prints Media Set information from directories.
```bash
python3 python/apps/arinc_665_print_media_set.py --medium-directory /path/to/medium1
```

#### arinc_665_media_set_decompiler.py
Decompiles Media Sets to XML format.
```bash
python3 python/apps/arinc_665_media_set_decompiler.py --medium-directory /path/to/medium1 --output-file mediaset.xml
```

#### arinc_665_media_set_manager.py
Comprehensive command-line manager with multiple operations.
```bash
python3 python/apps/arinc_665_media_set_manager.py list --medium-directory /path/to/medium1
python3 python/apps/arinc_665_media_set_manager.py validate --medium-directory /path/to/medium1
python3 python/apps/arinc_665_media_set_manager.py export --medium-directory /path/to/medium1 --output-file mediaset.xml
python3 python/apps/arinc_665_media_set_manager.py import --xml-file mediaset.xml
```

### Python Library Usage

```python
from arinc_665.media.media_set import MediaSet
from arinc_665.utils.media_set_printer import MediaSetPrinter
from arinc_665.utils.xml_handler import XmlHandler

# Load a media set from directory
media_set = MediaSet.load_from_directory(Path("/path/to/media/set"))

# Print media set information
printer = MediaSetPrinter()
printer.print_media_set(media_set)

# Export to XML
xml_handler = XmlHandler()
xml_handler.export_to_xml(media_set, Path("output.xml"))

# Import from XML
imported_media_set = xml_handler.import_from_xml(Path("input.xml"))
```

### Installation

From the repository root:
```bash
cd python
pip install -e .
```

This will install the `arinc-665` package and make the command-line tools available as:
- `arinc-665-ls`
- `arinc-665-print-xml`
- `arinc-665-media-set-check`
- `arinc-665-print-media-set`
- `arinc-665-media-set-decompiler`
- `arinc-665-media-set-manager`

## References
- [ARINC Report 665-5](https://aviation-ia.sae-itc.com/standards/arinc665-5-665-5-loadable-software-standards):
  Loadable Software Standards;
  2019-08-13
- [ARINC Report 645-1](https://aviation-ia.sae-itc.com/standards/arinc645-1-645-1-common-terminology-functions-software-distribution-loading):
  Common Terminology and Functions for Software Distribution and Loading;
  2021-08-11
