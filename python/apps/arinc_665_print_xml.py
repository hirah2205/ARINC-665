#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Media Set XML Printer Application

Loads XML and prints Media Set information to console.
Equivalent to the C++ arinc_665_print_xml application.
"""

import argparse
import sys
import logging
from pathlib import Path

# Add the parent directory to the path so we can import arinc_665
sys.path.insert(0, str(Path(__file__).parent.parent))

from arinc_665.utils.xml_handler import XmlHandler
from arinc_665.utils.media_set_printer import MediaSetPrinter
from arinc_665.exceptions import Arinc665Exception, XmlException


def setup_logging(verbose: bool = False) -> None:
    """Set up logging configuration."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format='%(levelname)s: %(message)s'
    )


def print_xml_media_set(xml_file: Path) -> None:
    """
    Load and print media set from XML file.
    
    Args:
        xml_file: Path to XML file
    """
    xml_handler = XmlHandler()
    
    try:
        print(f"Loading XML file: {xml_file}")
        print()
        
        # Import media set from XML
        media_set = xml_handler.import_from_xml(xml_file)
        
        # Print media set information
        printer = MediaSetPrinter()
        printer.print_media_set(media_set)
        
    except XmlException as e:
        print(f"XML Error: {e}", file=sys.stderr)
        raise
    except Exception as e:
        print(f"Error processing XML file: {e}", file=sys.stderr)
        raise


def main() -> int:
    """Main application entry point."""
    parser = argparse.ArgumentParser(
        description="Loads XML and prints Media Set information to console",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --xml-file /path/to/mediaset.xml
  %(prog)s --xml-file /path/to/mediaset.xml --verbose
        """.strip()
    )
    
    # Main arguments
    parser.add_argument(
        "--xml-file",
        type=Path,
        required=True,
        help="XML file containing Media Set definition"
    )
    
    # Optional arguments
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Enable verbose output"
    )
    
    try:
        args = parser.parse_args()
        
        # Set up logging
        setup_logging(args.verbose)
        
        print("ARINC 665 Media Set XML Printer")
        print()
        
        # Validate XML file
        if not args.xml_file.exists():
            print(f"Error: XML file does not exist: {args.xml_file}", file=sys.stderr)
            return 1
            
        if not args.xml_file.is_file():
            print(f"Error: Path is not a file: {args.xml_file}", file=sys.stderr)
            return 1
        
        # Check file extension
        if args.xml_file.suffix.lower() not in ['.xml']:
            print(f"Warning: File does not have .xml extension: {args.xml_file}", file=sys.stderr)
        
        # Process XML file
        print_xml_media_set(args.xml_file)
        
        return 0
        
    except KeyboardInterrupt:
        print("\nOperation cancelled by user.", file=sys.stderr)
        return 130
    except Arinc665Exception as e:
        print(f"ARINC 665 Error: {e}", file=sys.stderr)
        return 1
    except Exception as e:
        print(f"Unexpected error: {e}", file=sys.stderr)
        if hasattr(args, 'verbose') and args.verbose:
            import traceback
            traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())