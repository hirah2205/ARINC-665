# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Load List File (LOADS.LUM) implementation.
"""

from typing import List, Dict, Any
from pathlib import Path
import struct

from ..exceptions import InvalidFileFormatException
from ..enums import Arinc665Version


class LoadListFile:
    """ARINC 665 Load List File (LOADS.LUM)."""
    
    def __init__(self, version: Arinc665Version = Arinc665Version.VERSION_5):
        """Initialize a load list file."""
        self.version = version
        self.loads: List[Dict[str, Any]] = []
        
    @classmethod
    def load_from_file(cls, path: Path) -> 'LoadListFile':
        """Load from LOADS.LUM file."""
        if not path.exists():
            raise InvalidFileFormatException(f"LOADS.LUM file does not exist: {path}")
            
        try:
            load_list = cls()
            
            with open(path, 'rb') as f:
                # Simplified implementation
                header = f.read(32)
                if len(header) < 32:
                    raise InvalidFileFormatException("LOADS.LUM file too short")
                
                load_count = struct.unpack('<I', header[8:12])[0]
                
                for i in range(load_count):
                    entry_data = f.read(64)
                    if len(entry_data) < 64:
                        break
                        
                    part_number = entry_data[:32].decode('ascii').rstrip('\x00')
                    filename = entry_data[32:64].decode('ascii').rstrip('\x00')
                    
                    load_list.loads.append({
                        'part_number': part_number,
                        'filename': filename
                    })
                    
            return load_list
            
        except Exception as e:
            raise InvalidFileFormatException(f"Failed to parse LOADS.LUM file {path}: {e}")
    
    def add_load_entry(self, part_number: str, filename: str) -> None:
        """Add a load entry."""
        self.loads.append({
            'part_number': part_number,
            'filename': filename
        })
    
    def get_info(self) -> Dict[str, Any]:
        """Get load list information."""
        return {
            'version': self.version.value,
            'load_count': len(self.loads),
            'loads': self.loads
        }