# SPDX-License-Identifier: MPL-2.0
"""
Media Set Validator utility for ARINC 665.
"""

from typing import List, Tuple
from pathlib import Path

from ..media.media_set import MediaSet
from ..exceptions import ValidationException


class MediaSetValidator:
    """Utility for validating Media Sets."""
    
    def __init__(self):
        """Initialize validator."""
        self.errors: List[str] = []
        self.warnings: List[str] = []
        
    def validate_media_set(self, media_set: MediaSet) -> bool:
        """
        Validate a media set.
        
        Args:
            media_set: MediaSet to validate
            
        Returns:
            True if valid, False otherwise
        """
        self.errors.clear()
        self.warnings.clear()
        
        # Basic validation
        if not media_set.loads and not media_set.batches:
            self.errors.append("Media set contains no loads or batches")
            
        # Validate loads
        for load in media_set.loads:
            self._validate_load(load)
            
        # Validate batches  
        for batch in media_set.batches:
            self._validate_batch(batch)
            
        # Validate directories
        for directory in media_set.directories:
            self._validate_directory(directory)
            
        return len(self.errors) == 0
    
    def _validate_load(self, load) -> None:
        """Validate a single load."""
        if not load.part_number:
            self.errors.append(f"Load has empty part number")
            
        if not load.files:
            self.warnings.append(f"Load {load.part_number} contains no files")
            
        # Validate file integrity
        for file in load.files:
            if not file.verify_checksum():
                self.errors.append(f"File {file.name} in load {load.part_number} has invalid checksum")
                
    def _validate_batch(self, batch) -> None:
        """Validate a single batch."""
        if not batch.name:
            self.errors.append("Batch has empty name")
            
        if not batch.loads:
            self.warnings.append(f"Batch {batch.name} contains no loads")
            
        if not batch.validate_execution_order():
            self.errors.append(f"Batch {batch.name} has invalid execution order")
            
    def _validate_directory(self, directory) -> None:
        """Validate a directory structure."""
        if not directory.name:
            self.errors.append("Directory has empty name")
            
        # Recursively validate subdirectories
        for subdir in directory.subdirectories:
            self._validate_directory(subdir)
    
    def get_validation_results(self) -> Tuple[List[str], List[str]]:
        """
        Get validation results.
        
        Returns:
            Tuple of (errors, warnings)
        """
        return self.errors.copy(), self.warnings.copy()
    
    def print_validation_results(self) -> None:
        """Print validation results to console."""
        if self.errors:
            print("Validation Errors:")
            for error in self.errors:
                print(f"  ERROR: {error}")
                
        if self.warnings:
            print("Validation Warnings:")
            for warning in self.warnings:
                print(f"  WARNING: {warning}")
                
        if not self.errors and not self.warnings:
            print("Validation passed: No errors or warnings found.")