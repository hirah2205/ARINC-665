# SPDX-License-Identifier: MPL-2.0
"""
File Printer utility for ARINC 665.
"""

from typing import TextIO
import sys
from pathlib import Path

from ..files.file_list_file import FileListFile
from ..files.load_list_file import LoadListFile
from ..files.batch_list_file import BatchListFile
from ..files.load_header_file import LoadHeaderFile
from ..files.batch_file import BatchFile
from ..exceptions import Arinc665Exception


class FilePrinter:
    """Utility for printing ARINC 665 file information."""
    
    def __init__(self, output: TextIO = sys.stdout):
        """
        Initialize printer.
        
        Args:
            output: Output stream (default: stdout)
        """
        self.output = output
        
    def print_file_info(self, file_path: Path) -> None:
        """
        Print information about an ARINC 665 file.
        
        Args:
            file_path: Path to the file
        """
        try:
            if not file_path.exists():
                self.output.write(f"File does not exist: {file_path}\n")
                return
                
            self.output.write(f"File: {file_path}\n")
            self.output.write("=" * (len(str(file_path)) + 6) + "\n")
            
            # Determine file type and print accordingly
            filename = file_path.name.upper()
            
            if filename == "FILES.LUM":
                self._print_file_list_file(file_path)
            elif filename == "LOADS.LUM":
                self._print_load_list_file(file_path)
            elif filename == "BATCHES.LUM":
                self._print_batch_list_file(file_path)
            elif filename.endswith(".LUH"):
                self._print_load_header_file(file_path)
            elif filename.endswith(".LUB"):
                self._print_batch_file(file_path)
            else:
                self._print_generic_file_info(file_path)
                
        except Exception as e:
            self.output.write(f"Error processing file {file_path}: {e}\n")
    
    def _print_file_list_file(self, file_path: Path) -> None:
        """Print FILES.LUM information."""
        try:
            file_list = FileListFile.load_from_file(file_path)
            info = file_list.get_info()
            
            self.output.write(f"Type: File List File (FILES.LUM)\n")
            self.output.write(f"Version: ARINC 665-{info['version']}\n")
            self.output.write(f"File Count: {info['file_count']}\n")
            self.output.write(f"Total Size: {info['total_size']} bytes\n")
            
            if info['files']:
                self.output.write("\nFiles:\n")
                for i, file_info in enumerate(info['files'], 1):
                    self.output.write(f"  {i}. {file_info['filename']}\n")
                    self.output.write(f"     Size: {file_info['size']} bytes\n")
                    self.output.write(f"     Checksum: {file_info['checksum']}\n")
                    
        except Arinc665Exception as e:
            self.output.write(f"Error reading FILES.LUM: {e}\n")
    
    def _print_load_list_file(self, file_path: Path) -> None:
        """Print LOADS.LUM information."""
        try:
            load_list = LoadListFile.load_from_file(file_path)
            info = load_list.get_info()
            
            self.output.write(f"Type: Load List File (LOADS.LUM)\n")
            self.output.write(f"Version: ARINC 665-{info['version']}\n")
            self.output.write(f"Load Count: {info['load_count']}\n")
            
            if info['loads']:
                self.output.write("\nLoads:\n")
                for i, load_info in enumerate(info['loads'], 1):
                    self.output.write(f"  {i}. {load_info['part_number']}\n")
                    self.output.write(f"     File: {load_info['filename']}\n")
                    
        except Arinc665Exception as e:
            self.output.write(f"Error reading LOADS.LUM: {e}\n")
    
    def _print_batch_list_file(self, file_path: Path) -> None:
        """Print BATCHES.LUM information."""
        try:
            batch_list = BatchListFile.load_from_file(file_path)
            info = batch_list.get_info()
            
            self.output.write(f"Type: Batch List File (BATCHES.LUM)\n")
            self.output.write(f"Version: ARINC 665-{info['version']}\n")
            self.output.write(f"Batch Count: {info['batch_count']}\n")
            
            if info['batches']:
                self.output.write("\nBatches:\n")
                for i, batch_info in enumerate(info['batches'], 1):
                    self.output.write(f"  {i}. {batch_info['batch_name']}\n")
                    self.output.write(f"     File: {batch_info['filename']}\n")
                    
        except Arinc665Exception as e:
            self.output.write(f"Error reading BATCHES.LUM: {e}\n")
    
    def _print_load_header_file(self, file_path: Path) -> None:
        """Print .LUH file information."""
        try:
            load_header = LoadHeaderFile.load_from_file(file_path)
            info = load_header.get_info()
            
            self.output.write(f"Type: Load Header File (.LUH)\n")
            self.output.write(f"Part Number: {info['part_number']}\n")
            self.output.write(f"Load Type: {info['load_type']}\n")
            self.output.write(f"Version: ARINC 665-{info['version']}\n")
            self.output.write(f"Description: {info['description']}\n")
            self.output.write(f"File Count: {info['file_count']}\n")
            self.output.write(f"Total Size: {info['total_size']} bytes\n")
            
        except Arinc665Exception as e:
            self.output.write(f"Error reading .LUH file: {e}\n")
    
    def _print_batch_file(self, file_path: Path) -> None:
        """Print .LUB file information."""
        try:
            batch_file = BatchFile.load_from_file(file_path)
            info = batch_file.get_info()
            
            self.output.write(f"Type: Batch File (.LUB)\n")
            self.output.write(f"Name: {info['name']}\n")
            self.output.write(f"Version: ARINC 665-{info['version']}\n")
            self.output.write(f"Description: {info['description']}\n")
            self.output.write(f"Load Count: {info['load_count']}\n")
            
        except Arinc665Exception as e:
            self.output.write(f"Error reading .LUB file: {e}\n")
    
    def _print_generic_file_info(self, file_path: Path) -> None:
        """Print generic file information."""
        stat = file_path.stat()
        self.output.write(f"Type: Generic File\n")
        self.output.write(f"Size: {stat.st_size} bytes\n")
        self.output.write(f"Modified: {stat.st_mtime}\n")