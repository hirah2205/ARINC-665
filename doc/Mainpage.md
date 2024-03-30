# ARINC 665 Tool Suite Documentation {#mainpage}

This tool suite provides library and applications for generating, reading and 
handling ARINC 665 Media Sets.

This tool suite supports the generation and interpretation of media sets
according to ARINC 665-2, ARINC 665-3, ARINC 665-4, and ARINC 665-5.

A library is provided for:
- generating and parsing ARINC 665 files
- media set model, and
- utilities for generation and management of media sets.

An XML format and corresponding tools are provided to ease generation of media
sets.

For building graphical applications an Qt based library is provided which
consist of models, dialogs, and wizards to work with ARINC 665 media sets.

This tool suite contains the components:
 - Library with namespaces:
   - @ref Arinc665,
   - @ref Arinc665Commands, and 
   - @ref Arinc665Qt.
 - @subpage arinc_665_applications

Dependencies are:
 - @ref Arinc645 and @subpage arinc_645_applications,
 - @ref Commands and @subpage commands_applications,
 - @ref Helper, @ref HelperQt, and @subpage helper_applications,
 - @ref QtIconResources, and @subpage qt_icon_resources_applications, and
 - libxml++
