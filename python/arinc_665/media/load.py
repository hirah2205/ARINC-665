# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Load implementation.
"""

from typing import List, Optional, Dict, Any
from pathlib import Path
import struct

from ..exceptions import InvalidFileFormatException
from ..enums import LoadType, Arinc665Version
from .file import File


class Load:
    """ARINC 665 Load."""
    
    def __init__(self, part_number: str, load_type: LoadType = LoadType.SOFTWARE):
        """
        Initialize a load.
        
        Args:
            part_number: Load part number
            load_type: Type of load
        """
        self.part_number = part_number
        self.load_type = load_type
        self.version = Arinc665Version.VERSION_5
        self.description: str = ""
        self.files: List[File] = []
        self.target_hardware_ids: List[str] = []
        self.load_check_value: Optional[int] = None
        self._metadata: Dict[str, Any] = {}
        
    @classmethod
    def from_luh_file(cls, path: Path) -> 'Load':
        """
        Load from LUH (Load Upload Header) file.
        
        Args:
            path: Path to LUH file
            
        Returns:
            Load instance
            
        Raises:
            InvalidFileFormatException: If file format is invalid
        """
        if not path.exists():
            raise InvalidFileFormatException(f"LUH file does not exist: {path}")
            
        try:
            with open(path, 'rb') as f:
                # Read basic header
                header_data = f.read(32)  # Basic header size
                if len(header_data) < 32:
                    raise InvalidFileFormatException("LUH file too short")
                
                # Parse header (simplified - real format would be more complex)
                # This is a placeholder implementation
                part_number = header_data[4:36].decode('ascii').rstrip('\x00')
                
                load = cls(part_number)
                load.description = f"Load from {path.name}"
                
                return load
                
        except Exception as e:
            raise InvalidFileFormatException(f"Failed to parse LUH file {path}: {e}")
    
    def add_file(self, file: File) -> None:
        """Add a file to this load."""
        self.files.append(file)
        
    def add_target_hardware_id(self, hardware_id: str) -> None:
        """Add a target hardware ID."""
        self.target_hardware_ids.append(hardware_id)
        
    def calculate_load_check_value(self) -> int:
        """
        Calculate load check value.
        
        Returns:
            Calculated check value
        """
        # Simplified implementation
        total_size = sum(file.size for file in self.files)
        return total_size & 0xFFFFFFFF
    
    def get_info(self) -> Dict[str, Any]:
        """
        Get load information.
        
        Returns:
            Dictionary containing load information
        """
        return {
            'part_number': self.part_number,
            'load_type': self.load_type.value,
            'version': self.version.value,
            'description': self.description,
            'file_count': len(self.files),
            'total_size': sum(file.size for file in self.files),
            'target_hardware_ids': self.target_hardware_ids,
            'load_check_value': self.load_check_value,
            'metadata': self._metadata
        }