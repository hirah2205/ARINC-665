# SPDX-License-Identifier: MPL-2.0
"""
XML Handler utility for ARINC 665.
"""

import xml.etree.ElementTree as ET
from typing import Dict, Any
from pathlib import Path

from ..media.media_set import MediaSet
from ..exceptions import XmlException


class XmlHandler:
    """Utility for XML import/export of Media Sets."""
    
    def __init__(self):
        """Initialize XML handler."""
        pass
        
    def export_to_xml(self, media_set: MediaSet, output_path: Path) -> None:
        """
        Export media set to XML.
        
        Args:
            media_set: MediaSet to export
            output_path: Output XML file path
            
        Raises:
            XmlException: If export fails
        """
        try:
            root = ET.Element("MediaSet")
            root.set("version", str(media_set.version.value))
            
            # Export loads
            if media_set.loads:
                loads_elem = ET.SubElement(root, "Loads")
                for load in media_set.loads:
                    load_elem = ET.SubElement(loads_elem, "Load")
                    load_elem.set("partNumber", load.part_number)
                    load_elem.set("type", load.load_type.value)
                    
                    if load.description:
                        desc_elem = ET.SubElement(load_elem, "Description")
                        desc_elem.text = load.description
                        
                    if load.files:
                        files_elem = ET.SubElement(load_elem, "Files")
                        for file in load.files:
                            file_elem = ET.SubElement(files_elem, "File")
                            file_elem.set("name", file.name)
                            file_elem.set("size", str(file.size))
                            if file.checksum:
                                file_elem.set("checksum", file.checksum)
            
            # Export batches
            if media_set.batches:
                batches_elem = ET.SubElement(root, "Batches")
                for batch in media_set.batches:
                    batch_elem = ET.SubElement(batches_elem, "Batch")
                    batch_elem.set("name", batch.name)
                    
                    if batch.description:
                        desc_elem = ET.SubElement(batch_elem, "Description")
                        desc_elem.text = batch.description
                        
                    if batch.execution_order:
                        order_elem = ET.SubElement(batch_elem, "ExecutionOrder")
                        for part_number in batch.execution_order:
                            load_ref_elem = ET.SubElement(order_elem, "LoadRef")
                            load_ref_elem.set("partNumber", part_number)
            
            # Write XML to file
            tree = ET.ElementTree(root)
            ET.indent(tree, space="  ", level=0)  # Pretty formatting
            tree.write(output_path, encoding='utf-8', xml_declaration=True)
            
        except Exception as e:
            raise XmlException(f"Failed to export media set to XML: {e}")
    
    def import_from_xml(self, xml_path: Path) -> MediaSet:
        """
        Import media set from XML.
        
        Args:
            xml_path: XML file path
            
        Returns:
            Imported MediaSet
            
        Raises:
            XmlException: If import fails
        """
        try:
            if not xml_path.exists():
                raise XmlException(f"XML file does not exist: {xml_path}")
                
            tree = ET.parse(xml_path)
            root = tree.getroot()
            
            if root.tag != "MediaSet":
                raise XmlException("Invalid XML: root element must be 'MediaSet'")
                
            # Create media set
            version_str = root.get("version", "5")
            from ..enums import Arinc665Version
            version = Arinc665Version(int(version_str))
            media_set = MediaSet.create(version)
            
            # Import loads
            loads_elem = root.find("Loads")
            if loads_elem is not None:
                for load_elem in loads_elem.findall("Load"):
                    part_number = load_elem.get("partNumber", "")
                    load_type_str = load_elem.get("type", "SOFTWARE")
                    
                    from ..enums import LoadType
                    load_type = LoadType(load_type_str)
                    
                    from ..media.load import Load
                    load = Load(part_number, load_type)
                    
                    desc_elem = load_elem.find("Description")
                    if desc_elem is not None and desc_elem.text:
                        load.description = desc_elem.text
                    
                    media_set.add_load(load)
            
            # Import batches
            batches_elem = root.find("Batches")
            if batches_elem is not None:
                for batch_elem in batches_elem.findall("Batch"):
                    batch_name = batch_elem.get("name", "")
                    
                    from ..media.batch import Batch
                    batch = Batch(batch_name)
                    
                    desc_elem = batch_elem.find("Description")
                    if desc_elem is not None and desc_elem.text:
                        batch.description = desc_elem.text
                    
                    order_elem = batch_elem.find("ExecutionOrder")
                    if order_elem is not None:
                        for load_ref_elem in order_elem.findall("LoadRef"):
                            part_number = load_ref_elem.get("partNumber", "")
                            batch.add_to_execution_order(part_number)
                    
                    media_set.add_batch(batch)
            
            return media_set
            
        except Exception as e:
            raise XmlException(f"Failed to import media set from XML: {e}")
    
    def print_xml_info(self, xml_path: Path) -> None:
        """
        Print XML file information.
        
        Args:
            xml_path: XML file path
        """
        try:
            media_set = self.import_from_xml(xml_path)
            from .media_set_printer import MediaSetPrinter
            printer = MediaSetPrinter()
            printer.print_media_set(media_set)
            
        except Exception as e:
            print(f"Error reading XML file {xml_path}: {e}")