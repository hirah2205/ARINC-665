# ARINC 665 Python Implementation

This directory contains the Python implementation of the ARINC 665 Tool Suite, providing equivalent functionality to the C++ library for generating, reading, and handling ARINC 665 Media Sets.

## Features

- **Complete Python Library**: Full implementation of ARINC 665 Media Set handling
- **Command-Line Applications**: Python equivalents of all C++ applications
- **XML Support**: Import/export Media Sets to/from XML format
- **ARINC 665 Compliance**: Supports Supplements 2, 3, 4, and 5
- **Type Safety**: Comprehensive type hints throughout
- **Cross-Platform**: Works on Linux, macOS, and Windows

## Quick Start

### Installation

```bash
# Install from source
pip install -e .

# Or run directly without installation
python3 apps/arinc_665_ls.py --help
```

### Basic Usage

```bash
# List ARINC 665 files in a directory
arinc-665-ls --directory /path/to/media/set

# Validate a media set
arinc-665-media-set-check --medium-directory /path/to/medium

# Export media set to XML
arinc-665-media-set-decompiler --medium-directory /path/to/medium --output-file mediaset.xml

# Import and display XML media set
arinc-665-print-xml --xml-file mediaset.xml
```

## Library Structure

```
arinc_665/
├── __init__.py          # Main package
├── version.py           # Version information
├── exceptions.py        # Exception classes
├── enums.py            # Enumerations and constants
├── media/              # Media handling
│   ├── media_set.py    # MediaSet class
│   ├── directory.py    # Directory handling
│   ├── file.py         # File handling
│   ├── load.py         # Load handling
│   └── batch.py        # Batch handling
├── files/              # File format handling
│   ├── file_list_file.py    # FILES.LUM
│   ├── load_list_file.py    # LOADS.LUM
│   ├── batch_list_file.py   # BATCHES.LUM
│   ├── load_header_file.py  # .LUH files
│   └── batch_file.py        # .LUB files
└── utils/              # Utility modules
    ├── media_set_printer.py     # Printing utilities
    ├── media_set_validator.py   # Validation utilities
    ├── xml_handler.py           # XML import/export
    └── file_printer.py          # File information printing
```

## Applications

| Application | Description | C++ Equivalent |
|-------------|-------------|----------------|
| `arinc_665_ls.py` | List ARINC 665 files and content | `arinc_665_ls` |
| `arinc_665_print_xml.py` | Print Media Set from XML | `arinc_665_print_xml` |
| `arinc_665_media_set_check.py` | Validate Media Sets | `arinc_665_media_set_check` |
| `arinc_665_print_media_set.py` | Print Media Set from directories | `arinc_665_print_media_set` |
| `arinc_665_media_set_decompiler.py` | Decompile Media Sets to XML | `arinc_665_media_set_decompiler` |
| `arinc_665_media_set_manager.py` | Comprehensive Media Set manager | `arinc_665_media_set_manager` |

## Examples

### Programming Interface

```python
from pathlib import Path
from arinc_665.media.media_set import MediaSet
from arinc_665.utils.media_set_printer import MediaSetPrinter
from arinc_665.utils.xml_handler import XmlHandler

# Load a media set from a directory
media_set = MediaSet.load_from_directory(Path("/path/to/media"))

# Print information
printer = MediaSetPrinter()
printer.print_media_set(media_set)

# Validate the media set
from arinc_665.utils.media_set_validator import MediaSetValidator
validator = MediaSetValidator()
is_valid = validator.validate_media_set(media_set)

if not is_valid:
    errors, warnings = validator.get_validation_results()
    for error in errors:
        print(f"Error: {error}")

# Export to XML
xml_handler = XmlHandler()
xml_handler.export_to_xml(media_set, Path("output.xml"))

# Import from XML
imported_set = xml_handler.import_from_xml(Path("input.xml"))
```

### Command-Line Examples

```bash
# Comprehensive media set management
arinc-665-media-set-manager list --medium-directory /media/set1

# Validate multiple media
arinc-665-media-set-check \
  --medium-directory /media/set1 \
  --medium-directory /media/set2

# Convert media set to XML with verbose output
arinc-665-media-set-decompiler \
  --medium-directory /media/set1 \
  --output-file mediaset.xml \
  --verbose

# Print media set to file
arinc-665-print-media-set \
  --medium-directory /media/set1 \
  --output-file report.txt
```

## Development

### Requirements

- Python 3.11+
- No external runtime dependencies
- Development dependencies: pytest, black, mypy (optional)

### Testing

```bash
# Run basic functionality tests
python3 -c "import arinc_665; print('Import successful')"

# Test applications
python3 apps/arinc_665_ls.py --help
python3 apps/arinc_665_media_set_manager.py --help
```

### Code Style

The codebase follows Python best practices:
- Type hints throughout
- Comprehensive docstrings
- Error handling with custom exceptions
- Modular design with clear separation of concerns

## Migration from C++

This Python implementation provides equivalent functionality to the C++ version:

- **API Compatibility**: Similar class structure and method names
- **File Format Support**: Same ARINC 665 file formats supported
- **Command-Line Compatibility**: Same arguments and behavior
- **Feature Parity**: All major C++ features are implemented

### Key Differences

- **Language**: Python 3.11+ instead of C++
- **Dependencies**: No external libraries required (vs. Boost, Qt, etc.)
- **Installation**: Simple pip install vs. CMake build
- **Performance**: Python is slower but more accessible for scripting
- **Extensibility**: Easier to extend and modify in Python

## License

This project is licensed under the Mozilla Public License Version 2.0 (MPL-2.0).