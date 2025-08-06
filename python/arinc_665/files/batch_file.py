# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Batch File (.LUB) implementation.
"""

from typing import Dict, Any
from pathlib import Path

from ..media.batch import Batch
from ..exceptions import InvalidFileFormatException


class BatchFile:
    """ARINC 665 Batch File (.LUB)."""
    
    def __init__(self, batch: Batch):
        """Initialize with a Batch object."""
        self.batch = batch
        
    @classmethod
    def load_from_file(cls, path: Path) -> 'BatchFile':
        """Load from .LUB file."""
        batch = Batch.from_lub_file(path)
        return cls(batch)
    
    def get_info(self) -> Dict[str, Any]:
        """Get batch file information."""
        return self.batch.get_info()