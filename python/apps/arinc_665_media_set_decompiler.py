#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Media Set Decompiler

Decompiles ARINC 665 Media Sets to XML.
Equivalent to the C++ arinc_665_media_set_decompiler application.
"""

import argparse
import sys
import logging
from pathlib import Path
from typing import List

# Add the parent directory to the path so we can import arinc_665
sys.path.insert(0, str(Path(__file__).parent.parent))

from arinc_665.media.media_set import MediaSet
from arinc_665.utils.xml_handler import XmlHandler
from arinc_665.exceptions import Arinc665Exception


def setup_logging(verbose: bool = False) -> None:
    """Set up logging configuration."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format='%(levelname)s: %(message)s'
    )


def decompile_media_set_to_xml(medium_directories: List[Path], output_file: Path) -> None:
    """
    Decompile ARINC 665 Media Set to XML.
    
    Args:
        medium_directories: List of medium directories
        output_file: Output XML file path
    """
    xml_handler = XmlHandler()
    
    print(f"Decompiling {len(medium_directories)} medium(s) to XML...")
    
    # For multiple media, we'll create a combined media set
    # In a real implementation, this might need more sophisticated handling
    combined_media_set = MediaSet.create()
    
    for i, directory in enumerate(medium_directories, 1):
        print(f"Processing medium {i}: {directory}")
        
        try:
            # Load media set from directory
            media_set = MediaSet.load_from_directory(directory)
            
            # Merge into combined media set
            # This is a simplified merge - real implementation would be more complex
            combined_media_set.directories.extend(media_set.directories)
            combined_media_set.loads.extend(media_set.loads)
            combined_media_set.batches.extend(media_set.batches)
            
            print(f"  ✓ Loaded {len(media_set.loads)} loads, {len(media_set.batches)} batches")
            
        except Exception as e:
            print(f"  ✗ Error loading medium {i}: {e}")
            raise
    
    # Export to XML
    print(f"Exporting to XML file: {output_file}")
    xml_handler.export_to_xml(combined_media_set, output_file)
    print("✓ Export completed successfully")


def main() -> int:
    """Main application entry point."""
    parser = argparse.ArgumentParser(
        description="Decompiles ARINC 665 Media Sets to XML",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --medium-directory /path/to/medium1 --output-file mediaset.xml
  %(prog)s --medium-directory /path/to/medium1 --medium-directory /path/to/medium2 --output-file combined.xml
  %(prog)s --medium-directory /path/to/medium1 --output-file mediaset.xml --verbose
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
    parser.add_argument(
        "--output-file", "-o",
        type=Path,
        required=True,
        help="Output XML file path"
    )
    
    # Optional arguments
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Enable verbose output"
    )
    parser.add_argument(
        "--force", "-f",
        action="store_true",
        help="Overwrite output file if it exists"
    )
    
    try:
        args = parser.parse_args()
        
        # Set up logging
        setup_logging(args.verbose)
        
        print("ARINC 665 Media Set Decompiler")
        print()
        
        # Validate medium directories
        for directory in args.medium_directory:
            if not directory.exists():
                print(f"Error: Medium directory does not exist: {directory}", file=sys.stderr)
                return 1
                
            if not directory.is_dir():
                print(f"Error: Path is not a directory: {directory}", file=sys.stderr)
                return 1
        
        # Check output file
        if args.output_file.exists() and not args.force:
            print(f"Error: Output file already exists: {args.output_file}", file=sys.stderr)
            print("Use --force to overwrite the existing file.", file=sys.stderr)
            return 1
        
        # Ensure output directory exists
        args.output_file.parent.mkdir(parents=True, exist_ok=True)
        
        # Perform decompilation
        decompile_media_set_to_xml(args.medium_directory, args.output_file)
        
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