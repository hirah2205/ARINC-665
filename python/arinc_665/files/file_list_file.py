# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 File List File (FILES.LUM) implementation.
"""

from typing import List, Dict, Any
from pathlib import Path
import struct

from ..exceptions import InvalidFileFormatException
from ..enums import Arinc665Version


class FileListFile:
    """ARINC 665 File List File (FILES.LUM)."""
    
    def __init__(self, version: Arinc665Version = Arinc665Version.VERSION_5):
        """
        Initialize a file list file.
        
        Args:
            version: ARINC 665 version
        """
        self.version = version
        self.files: List[Dict[str, Any]] = []
        
    @classmethod
    def load_from_file(cls, path: Path) -> 'FileListFile':
        """
        Load from FILES.LUM file.
        
        Args:
            path: Path to FILES.LUM file
            
        Returns:
            FileListFile instance
            
        Raises:
            InvalidFileFormatException: If file format is invalid
        """
        if not path.exists():
            raise InvalidFileFormatException(f"FILES.LUM file does not exist: {path}")
            
        try:
            file_list = cls()
            
            with open(path, 'rb') as f:
                # Read file header (simplified implementation)
                header = f.read(32)
                if len(header) < 32:
                    raise InvalidFileFormatException("FILES.LUM file too short")
                
                # Parse basic header
                # This is a simplified implementation - real format would be more complex
                file_count = struct.unpack('<I', header[8:12])[0]
                
                # Read file entries
                for i in range(file_count):
                    entry_data = f.read(64)  # Simplified entry size
                    if len(entry_data) < 64:
                        break
                        
                    # Parse file entry (simplified)
                    filename = entry_data[:32].decode('ascii').rstrip('\x00')
                    file_size = struct.unpack('<I', entry_data[32:36])[0]
                    checksum = entry_data[36:52].hex()
                    
                    file_list.files.append({
                        'filename': filename,
                        'size': file_size,
                        'checksum': checksum
                    })
                    
            return file_list
            
        except Exception as e:
            raise InvalidFileFormatException(f"Failed to parse FILES.LUM file {path}: {e}")
    
    def add_file_entry(self, filename: str, size: int, checksum: str) -> None:
        """
        Add a file entry.
        
        Args:
            filename: File name
            size: File size in bytes
            checksum: File checksum
        """
        self.files.append({
            'filename': filename,
            'size': size,
            'checksum': checksum
        })
    
    def get_info(self) -> Dict[str, Any]:
        """
        Get file list information.
        
        Returns:
            Dictionary containing file list information
        """
        return {
            'version': self.version.value,
            'file_count': len(self.files),
            'total_size': sum(f['size'] for f in self.files),
            'files': self.files
        }