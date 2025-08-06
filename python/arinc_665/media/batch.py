# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Batch implementation.
"""

from typing import List, Optional, Dict, Any
from pathlib import Path

from ..exceptions import InvalidFileFormatException
from ..enums import Arinc665Version
from .load import Load


class Batch:
    """ARINC 665 Batch."""
    
    def __init__(self, name: str):
        """
        Initialize a batch.
        
        Args:
            name: Batch name
        """
        self.name = name
        self.version = Arinc665Version.VERSION_5
        self.description: str = ""
        self.loads: List[Load] = []
        self.execution_order: List[str] = []  # Part numbers in execution order
        self._metadata: Dict[str, Any] = {}
        
    @classmethod
    def from_lub_file(cls, path: Path) -> 'Batch':
        """
        Load from LUB (Load Upload Batch) file.
        
        Args:
            path: Path to LUB file
            
        Returns:
            Batch instance
            
        Raises:
            InvalidFileFormatException: If file format is invalid
        """
        if not path.exists():
            raise InvalidFileFormatException(f"LUB file does not exist: {path}")
            
        try:
            with open(path, 'rb') as f:
                # Read basic header
                header_data = f.read(32)  # Basic header size
                if len(header_data) < 32:
                    raise InvalidFileFormatException("LUB file too short")
                
                # Parse header (simplified - real format would be more complex)
                # This is a placeholder implementation
                batch_name = path.stem
                
                batch = cls(batch_name)
                batch.description = f"Batch from {path.name}"
                
                return batch
                
        except Exception as e:
            raise InvalidFileFormatException(f"Failed to parse LUB file {path}: {e}")
    
    def add_load(self, load: Load) -> None:
        """Add a load to this batch."""
        self.loads.append(load)
        
    def add_to_execution_order(self, part_number: str) -> None:
        """Add a load to the execution order."""
        if part_number not in self.execution_order:
            self.execution_order.append(part_number)
    
    def get_load_by_part_number(self, part_number: str) -> Optional[Load]:
        """
        Get a load by its part number.
        
        Args:
            part_number: Load part number
            
        Returns:
            Load instance or None if not found
        """
        for load in self.loads:
            if load.part_number == part_number:
                return load
        return None
    
    def validate_execution_order(self) -> bool:
        """
        Validate that all loads in execution order exist.
        
        Returns:
            True if valid, False otherwise
        """
        load_part_numbers = {load.part_number for load in self.loads}
        return all(pn in load_part_numbers for pn in self.execution_order)
    
    def get_info(self) -> Dict[str, Any]:
        """
        Get batch information.
        
        Returns:
            Dictionary containing batch information
        """
        return {
            'name': self.name,
            'version': self.version.value,
            'description': self.description,
            'load_count': len(self.loads),
            'execution_order': self.execution_order,
            'metadata': self._metadata
        }