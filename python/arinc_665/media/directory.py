# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Directory implementation.
"""

from typing import List, Optional
from pathlib import Path

from .file import File


class Directory:
    """ARINC 665 Directory."""
    
    def __init__(self, name: str, parent: Optional['Directory'] = None):
        """
        Initialize a directory.
        
        Args:
            name: Directory name
            parent: Parent directory (None for root)
        """
        self.name = name
        self.parent = parent
        self.subdirectories: List['Directory'] = []
        self.files: List[File] = []
        
    def add_subdirectory(self, directory: 'Directory') -> None:
        """Add a subdirectory."""
        directory.parent = self
        self.subdirectories.append(directory)
        
    def add_file(self, file: File) -> None:
        """Add a file."""
        file.parent_directory = self
        self.files.append(file)
        
    def get_path(self) -> str:
        """Get the full path of this directory."""
        if self.parent is None:
            return self.name
        return f"{self.parent.get_path()}/{self.name}"
        
    def find_file(self, name: str) -> Optional[File]:
        """Find a file by name in this directory."""
        for file in self.files:
            if file.name == name:
                return file
        return None
        
    def list_all_files(self) -> List[File]:
        """List all files recursively."""
        all_files = self.files.copy()
        for subdir in self.subdirectories:
            all_files.extend(subdir.list_all_files())
        return all_files