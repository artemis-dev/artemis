#!/bin/sh

# Generates the HTML footer

echo '<html>'
echo '</body>'
echo '<div id="footer" style="background-color:#DDDDDD;">'
echo '<small>'
echo '<img class="footer" src="rootlogo_s.gif" alt="root"/></a>'
# Doxygen unconditionally adds a space in front of $DOXYGEN_ROOT_VERSION
echo 'ARTEMIS'$DOXYGEN_ROOT_VERSION' - Reference Guide Generated on $datetime using Doxygen '`doxygen --version`'.'
echo '</small>'
echo '</div>'
echo '</body>'
echo '</html>'