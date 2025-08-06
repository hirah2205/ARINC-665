# SPDX-License-Identifier: MPL-2.0
"""
Enumerations and constants for ARINC 665 library.
"""

from enum import Enum, IntEnum
from typing import Dict


class Arinc665Version(IntEnum):
    """Supported ARINC 665 versions."""
    VERSION_2 = 2
    VERSION_3 = 3
    VERSION_4 = 4
    VERSION_5 = 5


class FileType(Enum):
    """ARINC 665 file types."""
    LOAD_HEADER = "LUH"  # Load header file
    BATCH = "LUB"        # Batch file
    FILE_LIST = "FILES.LUM"      # File list file
    LOAD_LIST = "LOADS.LUM"      # Load list file
    BATCH_LIST = "BATCHES.LUM"   # Batch list file
    UNKNOWN = "UNKNOWN"


class LoadType(Enum):
    """Types of loads."""
    SOFTWARE = "SOFTWARE"
    DATA = "DATA"
    CONFIGURATION = "CONFIGURATION"


# File format constants
ARINC_665_MAGIC_NUMBER = b'\x41\x52\x49\x4e'  # "ARIN" in ASCII
FILE_FORMAT_VERSION_MAP: Dict[Arinc665Version, int] = {
    Arinc665Version.VERSION_2: 2,
    Arinc665Version.VERSION_3: 3,
    Arinc665Version.VERSION_4: 4,
    Arinc665Version.VERSION_5: 5,
}

# Standard file sizes and offsets
HEADER_SIZE = 32
CHECKSUM_SIZE = 4
MAX_FILENAME_LENGTH = 255
MAX_PART_NUMBER_LENGTH = 32