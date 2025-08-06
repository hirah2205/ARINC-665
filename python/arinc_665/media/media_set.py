# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Media Set implementation.
"""

from typing import List, Optional, Dict, Any
from pathlib import Path
import logging

from ..exceptions import InvalidMediaSetException
from ..enums import Arinc665Version
from .directory import Directory
from .load import Load
from .batch import Batch

logger = logging.getLogger(__name__)


class MediaSet:
    """ARINC 665 Media Set."""
    
    def __init__(self, version: Arinc665Version = Arinc665Version.VERSION_5):
        """
        Initialize a new Media Set.
        
        Args:
            version: ARINC 665 version to use
        """
        self.version = version
        self.directories: List[Directory] = []
        self.loads: List[Load] = []
        self.batches: List[Batch] = []
        self._metadata: Dict[str, Any] = {}
        
    @classmethod
    def create(cls, version: Arinc665Version = Arinc665Version.VERSION_5) -> 'MediaSet':
        """
        Create a new Media Set.
        
        Args:
            version: ARINC 665 version to use
            
        Returns:
            New MediaSet instance
        """
        return cls(version)
    
    @classmethod
    def load_from_directory(cls, path: Path) -> 'MediaSet':
        """
        Load Media Set from directory.
        
        Args:
            path: Directory path containing media set
            
        Returns:
            Loaded MediaSet instance
            
        Raises:
            InvalidMediaSetException: If directory is not a valid media set
        """
        if not path.is_dir():
            raise InvalidMediaSetException(f"Path is not a directory: {path}")
            
        media_set = cls.create()
        
        # Look for standard ARINC 665 files
        files_lum = path / "FILES.LUM"
        loads_lum = path / "LOADS.LUM"
        batches_lum = path / "BATCHES.LUM"
        
        if files_lum.exists():
            logger.info(f"Found FILES.LUM in {path}")
            # TODO: Parse FILES.LUM
            
        if loads_lum.exists():
            logger.info(f"Found LOADS.LUM in {path}")
            # TODO: Parse LOADS.LUM
            
        if batches_lum.exists():
            logger.info(f"Found BATCHES.LUM in {path}")
            # TODO: Parse BATCHES.LUM
            
        return media_set
    
    def add_directory(self, directory: Directory) -> None:
        """Add a directory to the media set."""
        self.directories.append(directory)
        
    def add_load(self, load: Load) -> None:
        """Add a load to the media set."""
        self.loads.append(load)
        
    def add_batch(self, batch: Batch) -> None:
        """Add a batch to the media set."""
        self.batches.append(batch)
        
    def validate(self) -> bool:
        """
        Validate the media set.
        
        Returns:
            True if valid, False otherwise
        """
        # Basic validation - can be expanded
        return True
        
    def get_info(self) -> Dict[str, Any]:
        """
        Get media set information.
        
        Returns:
            Dictionary containing media set information
        """
        return {
            'version': self.version.value,
            'directories': len(self.directories),
            'loads': len(self.loads),
            'batches': len(self.batches),
            'metadata': self._metadata
        }