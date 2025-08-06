#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 List Application

Utility to scan directories/files for ARINC 665 content and decode them.
Equivalent to the C++ arinc_665_ls application.
"""

import argparse
import sys
import logging
from pathlib import Path
from typing import List

# Add the parent directory to the path so we can import arinc_665
sys.path.insert(0, str(Path(__file__).parent.parent))

from arinc_665.utils.file_printer import FilePrinter
from arinc_665.exceptions import Arinc665Exception


def setup_logging(verbose: bool = False) -> None:
    """Set up logging configuration."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format='%(levelname)s: %(message)s'
    )


def list_directory_contents(directory: Path, recursive: bool = True) -> None:
    """
    List and analyze ARINC 665 files in a directory.
    
    Args:
        directory: Directory to scan
        recursive: Whether to scan recursively
    """
    printer = FilePrinter()
    
    print(f"Scanning directory: {directory}")
    print("=" * (len(str(directory)) + 19))
    
    # Look for standard ARINC 665 files
    arinc_files = []
    
    if recursive:
        # Recursively find ARINC 665 files
        for pattern in ["*.LUM", "*.LUH", "*.LUB"]:
            arinc_files.extend(directory.rglob(pattern))
    else:
        # Only look in the current directory
        for pattern in ["*.LUM", "*.LUH", "*.LUB"]:
            arinc_files.extend(directory.glob(pattern))
    
    # Also look for specific files
    standard_files = ["FILES.LUM", "LOADS.LUM", "BATCHES.LUM"]
    for filename in standard_files:
        file_path = directory / filename
        if file_path.exists() and file_path not in arinc_files:
            arinc_files.append(file_path)
    
    if not arinc_files:
        print("No ARINC 665 files found in the directory.")
        return
    
    # Sort files for consistent output
    arinc_files.sort()
    
    for file_path in arinc_files:
        print()  # Empty line between files
        printer.print_file_info(file_path)


def list_single_file(file_path: Path) -> None:
    """
    Analyze a single ARINC 665 file.
    
    Args:
        file_path: Path to the file
    """
    printer = FilePrinter()
    printer.print_file_info(file_path)


def main() -> int:
    """Main application entry point."""
    parser = argparse.ArgumentParser(
        description="Prints the ARINC 665 Media File information located in the given directory",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --directory /path/to/media/set
  %(prog)s --directory /path/to/media/set --no-recursive
  %(prog)s --file /path/to/FILES.LUM
        """.strip()
    )
    
    # Main arguments
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "--directory",
        type=Path,
        help="Start directory to scan for ARINC 665 files"
    )
    group.add_argument(
        "--file", 
        type=Path,
        help="Single ARINC 665 file to analyze"
    )
    
    # Optional arguments
    parser.add_argument(
        "--no-recursive",
        action="store_true",
        help="Do not scan subdirectories (only used with --directory)"
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Enable verbose output"
    )
    
    try:
        args = parser.parse_args()
        
        # Set up logging
        setup_logging(args.verbose)
        
        print("ARINC 665 list")
        print()
        
        if args.directory:
            if not args.directory.exists():
                print(f"Error: Directory does not exist: {args.directory}", file=sys.stderr)
                return 1
                
            if not args.directory.is_dir():
                print(f"Error: Path is not a directory: {args.directory}", file=sys.stderr)
                return 1
                
            list_directory_contents(args.directory, not args.no_recursive)
            
        elif args.file:
            if not args.file.exists():
                print(f"Error: File does not exist: {args.file}", file=sys.stderr)
                return 1
                
            list_single_file(args.file)
        
        return 0
        
    except KeyboardInterrupt:
        print("\nOperation cancelled by user.", file=sys.stderr)
        return 130
    except Arinc665Exception as e:
        print(f"ARINC 665 Error: {e}", file=sys.stderr)
        return 1
    except Exception as e:
        print(f"Unexpected error: {e}", file=sys.stderr)
        if args.verbose:
            import traceback
            traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())