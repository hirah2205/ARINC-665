#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Media Set Manager

Command-line manager for ARINC 665 Media Sets.
Equivalent to the C++ arinc_665_media_set_manager application.
"""

import argparse
import sys
import logging
from pathlib import Path
from typing import List, Optional

# Add the parent directory to the path so we can import arinc_665
sys.path.insert(0, str(Path(__file__).parent.parent))

from arinc_665.media.media_set import MediaSet
from arinc_665.utils.media_set_printer import MediaSetPrinter
from arinc_665.utils.media_set_validator import MediaSetValidator
from arinc_665.utils.xml_handler import XmlHandler
from arinc_665.exceptions import Arinc665Exception


def setup_logging(verbose: bool = False) -> None:
    """Set up logging configuration."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format='%(levelname)s: %(message)s'
    )


def list_media_set(medium_directories: List[Path]) -> None:
    """List media set contents."""
    printer = MediaSetPrinter()
    
    for i, directory in enumerate(medium_directories, 1):
        print(f"Medium {i}: {directory}")
        print("=" * (len(str(directory)) + 10))
        
        try:
            media_set = MediaSet.load_from_directory(directory)
            printer.print_media_set(media_set)
            
        except Exception as e:
            print(f"Error loading media set: {e}")
        
        if i < len(medium_directories):
            print("\n" + "=" * 60 + "\n")


def validate_media_set(medium_directories: List[Path]) -> bool:
    """Validate media set."""
    validator = MediaSetValidator()
    all_valid = True
    
    for i, directory in enumerate(medium_directories, 1):
        print(f"Validating medium {i}: {directory}")
        
        try:
            media_set = MediaSet.load_from_directory(directory)
            is_valid = validator.validate_media_set(media_set)
            
            if is_valid:
                print(f"✓ Medium {i} validation PASSED")
            else:
                print(f"✗ Medium {i} validation FAILED")
                all_valid = False
            
            validator.print_validation_results()
            
        except Exception as e:
            print(f"✗ Error validating medium {i}: {e}")
            all_valid = False
    
    return all_valid


def export_to_xml(medium_directories: List[Path], output_file: Path) -> None:
    """Export media set to XML."""
    xml_handler = XmlHandler()
    
    # Create combined media set
    combined_media_set = MediaSet.create()
    
    for directory in medium_directories:
        media_set = MediaSet.load_from_directory(directory)
        combined_media_set.directories.extend(media_set.directories)
        combined_media_set.loads.extend(media_set.loads)
        combined_media_set.batches.extend(media_set.batches)
    
    xml_handler.export_to_xml(combined_media_set, output_file)
    print(f"Exported to XML: {output_file}")


def import_from_xml(xml_file: Path) -> None:
    """Import and display media set from XML."""
    xml_handler = XmlHandler()
    media_set = xml_handler.import_from_xml(xml_file)
    
    printer = MediaSetPrinter()
    printer.print_media_set(media_set)


def main() -> int:
    """Main application entry point."""
    parser = argparse.ArgumentParser(
        description="Command-line manager for ARINC 665 Media Sets",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Commands:
  list      - List media set contents
  validate  - Validate media set
  export    - Export media set to XML
  import    - Import media set from XML

Examples:
  %(prog)s list --medium-directory /path/to/medium1
  %(prog)s validate --medium-directory /path/to/medium1 --medium-directory /path/to/medium2
  %(prog)s export --medium-directory /path/to/medium1 --output-file mediaset.xml
  %(prog)s import --xml-file mediaset.xml
        """.strip()
    )
    
    # Subcommands
    subparsers = parser.add_subparsers(dest='command', help='Available commands')
    
    # List command
    list_parser = subparsers.add_parser('list', help='List media set contents')
    list_parser.add_argument(
        "--medium-directory",
        type=Path,
        action="append",
        required=True,
        help="ARINC 665 medium source directory. For multiple media, repeat this parameter."
    )
    
    # Validate command
    validate_parser = subparsers.add_parser('validate', help='Validate media set')
    validate_parser.add_argument(
        "--medium-directory",
        type=Path,
        action="append",
        required=True,
        help="ARINC 665 medium source directory. For multiple media, repeat this parameter."
    )
    
    # Export command
    export_parser = subparsers.add_parser('export', help='Export media set to XML')
    export_parser.add_argument(
        "--medium-directory",
        type=Path,
        action="append",
        required=True,
        help="ARINC 665 medium source directory. For multiple media, repeat this parameter."
    )
    export_parser.add_argument(
        "--output-file", "-o",
        type=Path,
        required=True,
        help="Output XML file path"
    )
    export_parser.add_argument(
        "--force", "-f",
        action="store_true",
        help="Overwrite output file if it exists"
    )
    
    # Import command
    import_parser = subparsers.add_parser('import', help='Import media set from XML')
    import_parser.add_argument(
        "--xml-file",
        type=Path,
        required=True,
        help="XML file containing Media Set definition"
    )
    
    # Global options
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Enable verbose output"
    )
    
    try:
        args = parser.parse_args()
        
        if not args.command:
            parser.print_help()
            return 1
        
        # Set up logging
        setup_logging(args.verbose)
        
        print("ARINC 665 Media Set Manager")
        print()
        
        # Execute command
        if args.command == 'list':
            # Validate directories
            for directory in args.medium_directory:
                if not directory.exists() or not directory.is_dir():
                    print(f"Error: Invalid directory: {directory}", file=sys.stderr)
                    return 1
            
            list_media_set(args.medium_directory)
            
        elif args.command == 'validate':
            # Validate directories
            for directory in args.medium_directory:
                if not directory.exists() or not directory.is_dir():
                    print(f"Error: Invalid directory: {directory}", file=sys.stderr)
                    return 1
            
            is_valid = validate_media_set(args.medium_directory)
            return 0 if is_valid else 1
            
        elif args.command == 'export':
            # Validate directories
            for directory in args.medium_directory:
                if not directory.exists() or not directory.is_dir():
                    print(f"Error: Invalid directory: {directory}", file=sys.stderr)
                    return 1
            
            # Check output file
            if args.output_file.exists() and not args.force:
                print(f"Error: Output file already exists: {args.output_file}", file=sys.stderr)
                print("Use --force to overwrite.", file=sys.stderr)
                return 1
            
            args.output_file.parent.mkdir(parents=True, exist_ok=True)
            export_to_xml(args.medium_directory, args.output_file)
            
        elif args.command == 'import':
            # Validate XML file
            if not args.xml_file.exists() or not args.xml_file.is_file():
                print(f"Error: Invalid XML file: {args.xml_file}", file=sys.stderr)
                return 1
            
            import_from_xml(args.xml_file)
        
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