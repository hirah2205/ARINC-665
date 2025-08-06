# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Load Header File (.LUH) implementation.
"""

from typing import Dict, Any
from pathlib import Path

from ..media.load import Load
from ..exceptions import InvalidFileFormatException


class LoadHeaderFile:
    """ARINC 665 Load Header File (.LUH)."""
    
    def __init__(self, load: Load):
        """Initialize with a Load object."""
        self.load = load
        
    @classmethod
    def load_from_file(cls, path: Path) -> 'LoadHeaderFile':
        """Load from .LUH file."""
        load = Load.from_luh_file(path)
        return cls(load)
    
    def get_info(self) -> Dict[str, Any]:
        """Get load header file information."""
        return self.load.get_info()