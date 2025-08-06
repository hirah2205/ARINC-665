#!/usr/bin/env python3
# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Media Set Validator

Validates ARINC 665 Media Sets.
Equivalent to the C++ arinc_665_media_set_check application.
"""

import argparse
import sys
import logging
from pathlib import Path
from typing import List

# Add the parent directory to the path so we can import arinc_665
sys.path.insert(0, str(Path(__file__).parent.parent))

from arinc_665.media.media_set import MediaSet
from arinc_665.utils.media_set_validator import MediaSetValidator
from arinc_665.exceptions import Arinc665Exception


def setup_logging(verbose: bool = False) -> None:
    """Set up logging configuration."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format='%(levelname)s: %(message)s'
    )


def validate_media_set_directories(medium_directories: List[Path]) -> bool:
    """
    Validate ARINC 665 Media Set from directories.
    
    Args:
        medium_directories: List of medium directories
        
    Returns:
        True if validation passes, False otherwise
    """
    validator = MediaSetValidator()
    all_valid = True
    
    for i, directory in enumerate(medium_directories, 1):
        print(f"Validating medium {i}: {directory}")
        print("-" * 50)
        
        try:
            # Load media set from directory
            media_set = MediaSet.load_from_directory(directory)
            
            # Validate the media set
            is_valid = validator.validate_media_set(media_set)
            
            if is_valid:
                print(f"✓ Medium {i} validation PASSED")
            else:
                print(f"✗ Medium {i} validation FAILED")
                all_valid = False
            
            # Print validation results
            errors, warnings = validator.get_validation_results()
            
            if errors:
                print(f"\nErrors ({len(errors)}):")
                for error in errors:
                    print(f"  ERROR: {error}")
            
            if warnings:
                print(f"\nWarnings ({len(warnings)}):")
                for warning in warnings:
                    print(f"  WARNING: {warning}")
            
            if not errors and not warnings:
                print("  No issues found.")
            
        except Exception as e:
            print(f"✗ Medium {i} validation FAILED: {e}")
            all_valid = False
        
        print()  # Empty line between media
    
    return all_valid


def main() -> int:
    """Main application entry point."""
    parser = argparse.ArgumentParser(
        description="Validates ARINC 665 Media Set",
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
        "--strict",
        action="store_true",
        help="Treat warnings as errors"
    )
    
    try:
        args = parser.parse_args()
        
        # Set up logging
        setup_logging(args.verbose)
        
        print("ARINC 665 Media Set Validator")
        print()
        
        # Validate medium directories
        for directory in args.medium_directory:
            if not directory.exists():
                print(f"Error: Medium directory does not exist: {directory}", file=sys.stderr)
                return 1
                
            if not directory.is_dir():
                print(f"Error: Path is not a directory: {directory}", file=sys.stderr)
                return 1
        
        # Perform validation
        validation_passed = validate_media_set_directories(args.medium_directory)
        
        # Print final result
        if validation_passed:
            print("Overall Result: ✓ VALIDATION PASSED")
            return 0
        else:
            print("Overall Result: ✗ VALIDATION FAILED")
            return 1
        
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