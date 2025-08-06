# SPDX-License-Identifier: MPL-2.0
"""
Exception classes for ARINC 665 Python library.
"""

class Arinc665Exception(Exception):
    """Base exception class for ARINC 665 library errors."""
    pass


class InvalidFileFormatException(Arinc665Exception):
    """Raised when file format is invalid or unsupported."""
    pass


class InvalidMediaSetException(Arinc665Exception):
    """Raised when media set is invalid or corrupted."""
    pass


class FileAccessException(Arinc665Exception):
    """Raised when file cannot be accessed or read."""
    pass


class ValidationException(Arinc665Exception):
    """Raised when validation of media set fails."""
    pass


class XmlException(Arinc665Exception):
    """Raised when XML processing fails."""
    pass