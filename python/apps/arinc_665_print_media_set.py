#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Print Media Set

Prints ARINC 665 Media Set from directories.
Equivalent to the C++ arinc_665_print_media_set application.
"""

import argparse
import sys
import logging
from pathlib import Path
from typing import List

# Add the parent directory to the path so we can import arinc_665
sys.path.insert(0, str(Path(__file__).parent.parent))

from arinc_665.media.media_set import MediaSet
from arinc_665.utils.media_set_printer import MediaSetPrinter
from arinc_665.exceptions import Arinc665Exception


def setup_logging(verbose: bool = False) -> None:
    """Set up logging configuration."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format='%(levelname)s: %(message)s'
    )


def print_media_set_from_directories(medium_directories: List[Path]) -> None:
    """
    Print ARINC 665 Media Set information from directories.
    
    Args:
        medium_directories: List of medium directories
    """
    printer = MediaSetPrinter()
    
    for i, directory in enumerate(medium_directories, 1):
        print(f"Medium {i}: {directory}")
        print("=" * (len(str(directory)) + 10))
        
        try:
            # Load media set from directory
            media_set = MediaSet.load_from_directory(directory)
            
            # Print media set information
            printer.print_media_set(media_set)
            
        except Exception as e:
            print(f"Error loading media set from {directory}: {e}")
        
        if i < len(medium_directories):
            print("\n" + "=" * 60 + "\n")  # Separator between media


def main() -> int:
    """Main application entry point."""
    parser = argparse.ArgumentParser(
        description="Prints ARINC 665 Media Set from directories",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --medium-directory /path/to/medium1
  %(prog)s --medium-directory /path/to/medium1 --medium-directory /path/to/medium2
  %(prog)s --medium-directory /path/to/medium1 --verbose
        """.strip()
    )
    
    # Main arguments
    parser.add_argument(
        "--medium-directory",
        type=Path,
        action="append",
        required=True,
        help="ARINC 665 medium source directory. For multiple media, repeat this parameter."
    )
    
    # Optional arguments
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Enable verbose output"
    )
    parser.add_argument(
        "--output-file", "-o",
        type=Path,
        help="Output file (default: stdout)"
    )
    
    try:
        args = parser.parse_args()
        
        # Set up logging
        setup_logging(args.verbose)
        
        # Validate medium directories
        for directory in args.medium_directory:
            if not directory.exists():
                print(f"Error: Medium directory does not exist: {directory}", file=sys.stderr)
                return 1
                
            if not directory.is_dir():
                print(f"Error: Path is not a directory: {directory}", file=sys.stderr)
                return 1
        
        # Redirect output if specified
        if args.output_file:
            try:
                with open(args.output_file, 'w') as f:
                    # Temporarily redirect stdout
                    original_stdout = sys.stdout
                    sys.stdout = f
                    
                    print("ARINC 665 Print Media Set")
                    print()
                    print_media_set_from_directories(args.medium_directory)
                    
                    # Restore stdout
                    sys.stdout = original_stdout
                    
                print(f"Output written to: {args.output_file}")
                
            except Exception as e:
                print(f"Error writing to output file {args.output_file}: {e}", file=sys.stderr)
                return 1
        else:
            print("ARINC 665 Print Media Set")
            print()
            print_media_set_from_directories(args.medium_directory)
        
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