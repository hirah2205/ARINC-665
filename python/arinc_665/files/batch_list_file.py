# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Batch List File (BATCHES.LUM) implementation.
"""

from typing import List, Dict, Any
from pathlib import Path
import struct

from ..exceptions import InvalidFileFormatException
from ..enums import Arinc665Version


class BatchListFile:
    """ARINC 665 Batch List File (BATCHES.LUM)."""
    
    def __init__(self, version: Arinc665Version = Arinc665Version.VERSION_5):
        """Initialize a batch list file."""
        self.version = version
        self.batches: List[Dict[str, Any]] = []
        
    @classmethod
    def load_from_file(cls, path: Path) -> 'BatchListFile':
        """Load from BATCHES.LUM file."""
        if not path.exists():
            raise InvalidFileFormatException(f"BATCHES.LUM file does not exist: {path}")
            
        try:
            batch_list = cls()
            
            with open(path, 'rb') as f:
                # Simplified implementation
                header = f.read(32)
                if len(header) < 32:
                    raise InvalidFileFormatException("BATCHES.LUM file too short")
                
                batch_count = struct.unpack('<I', header[8:12])[0]
                
                for i in range(batch_count):
                    entry_data = f.read(64)
                    if len(entry_data) < 64:
                        break
                        
                    batch_name = entry_data[:32].decode('ascii').rstrip('\x00')
                    filename = entry_data[32:64].decode('ascii').rstrip('\x00')
                    
                    batch_list.batches.append({
                        'batch_name': batch_name,
                        'filename': filename
                    })
                    
            return batch_list
            
        except Exception as e:
            raise InvalidFileFormatException(f"Failed to parse BATCHES.LUM file {path}: {e}")
    
    def add_batch_entry(self, batch_name: str, filename: str) -> None:
        """Add a batch entry."""
        self.batches.append({
            'batch_name': batch_name,
            'filename': filename
        })
    
    def get_info(self) -> Dict[str, Any]:
        """Get batch list information."""
        return {
            'version': self.version.value,
            'batch_count': len(self.batches),
            'batches': self.batches
        }