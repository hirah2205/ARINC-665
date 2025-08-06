# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 File implementation.
"""

from typing import Optional, TYPE_CHECKING
from pathlib import Path
import hashlib

from ..exceptions import FileAccessException

if TYPE_CHECKING:
    from .directory import Directory


class File:
    """ARINC 665 File."""
    
    def __init__(self, name: str, size: int = 0, checksum: Optional[str] = None):
        """
        Initialize a file.
        
        Args:
            name: File name
            size: File size in bytes
            checksum: File checksum (optional)
        """
        self.name = name
        self.size = size
        self.checksum = checksum
        self.parent_directory: Optional['Directory'] = None
        self._data: Optional[bytes] = None
        
    @classmethod
    def from_path(cls, path: Path) -> 'File':
        """
        Create a File from a filesystem path.
        
        Args:
            path: Path to the file
            
        Returns:
            File instance
            
        Raises:
            FileAccessException: If file cannot be read
        """
        if not path.exists():
            raise FileAccessException(f"File does not exist: {path}")
            
        if not path.is_file():
            raise FileAccessException(f"Path is not a file: {path}")
            
        try:
            size = path.stat().st_size
            with open(path, 'rb') as f:
                data = f.read()
                checksum = hashlib.md5(data).hexdigest()
                
            file_obj = cls(path.name, size, checksum)
            file_obj._data = data
            return file_obj
            
        except Exception as e:
            raise FileAccessException(f"Failed to read file {path}: {e}")
    
    def get_data(self) -> bytes:
        """
        Get file data.
        
        Returns:
            File data as bytes
            
        Raises:
            FileAccessException: If data is not available
        """
        if self._data is None:
            raise FileAccessException(f"No data available for file {self.name}")
        return self._data
    
    def set_data(self, data: bytes) -> None:
        """
        Set file data.
        
        Args:
            data: File data
        """
        self._data = data
        self.size = len(data)
        self.checksum = hashlib.md5(data).hexdigest()
    
    def get_full_path(self) -> str:
        """Get the full path of this file."""
        if self.parent_directory is None:
            return self.name
        return f"{self.parent_directory.get_path()}/{self.name}"
    
    def verify_checksum(self) -> bool:
        """
        Verify file checksum.
        
        Returns:
            True if checksum is valid, False otherwise
        """
        if self.checksum is None or self._data is None:
            return False
        
        actual_checksum = hashlib.md5(self._data).hexdigest()
        return actual_checksum == self.checksum