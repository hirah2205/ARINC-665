# SPDX-License-Identifier: MPL-2.0
"""
Media Set Printer utility for ARINC 665.
"""

from typing import TextIO
import sys

from ..media.media_set import MediaSet


class MediaSetPrinter:
    """Utility for printing Media Set information."""
    
    def __init__(self, output: TextIO = sys.stdout):
        """
        Initialize printer.
        
        Args:
            output: Output stream (default: stdout)
        """
        self.output = output
        
    def print_media_set(self, media_set: MediaSet) -> None:
        """
        Print media set information.
        
        Args:
            media_set: MediaSet to print
        """
        info = media_set.get_info()
        
        self.output.write("ARINC 665 Media Set Information\n")
        self.output.write("=" * 40 + "\n")
        self.output.write(f"Version: ARINC 665-{info['version']}\n")
        self.output.write(f"Directories: {info['directories']}\n")
        self.output.write(f"Loads: {info['loads']}\n")
        self.output.write(f"Batches: {info['batches']}\n")
        
        if media_set.loads:
            self.output.write("\nLoads:\n")
            for i, load in enumerate(media_set.loads, 1):
                load_info = load.get_info()
                self.output.write(f"  {i}. {load_info['part_number']}\n")
                self.output.write(f"     Type: {load_info['load_type']}\n")
                self.output.write(f"     Files: {load_info['file_count']}\n")
                self.output.write(f"     Size: {load_info['total_size']} bytes\n")
                
        if media_set.batches:
            self.output.write("\nBatches:\n")
            for i, batch in enumerate(media_set.batches, 1):
                batch_info = batch.get_info()
                self.output.write(f"  {i}. {batch_info['name']}\n")
                self.output.write(f"     Loads: {batch_info['load_count']}\n")
                
        if media_set.directories:
            self.output.write("\nDirectories:\n")
            for directory in media_set.directories:
                self._print_directory(directory, indent=2)
                
    def _print_directory(self, directory, indent: int = 0) -> None:
        """Print directory structure recursively."""
        spaces = " " * indent
        self.output.write(f"{spaces}{directory.name}/\n")
        
        for file in directory.files:
            self.output.write(f"{spaces}  {file.name} ({file.size} bytes)\n")
            
        for subdir in directory.subdirectories:
            self._print_directory(subdir, indent + 2)