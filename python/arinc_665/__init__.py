# SPDX-License-Identifier: MPL-2.0
"""
ARINC 665 Python Library

This library provides functionality for generating, reading and handling ARINC 665 Media Sets.
Supports ARINC 665 Supplements 2, 3, 4, and 5.
"""

from .version import VERSION, __version__
from .exceptions import Arinc665Exception

__all__ = [
    "VERSION",
    "__version__", 
    "Arinc665Exception",
]