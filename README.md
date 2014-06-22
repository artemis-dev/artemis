ARTEMIS
=======

**A RooT Extesion with Modular processor for Instant Swiching**

ARTEMIS is an analysis framework with modular processors, which enable us to easily switch the detail of analysis, like snipets.



Features
--------

* Modular processors and steering files
* Generic parameter loader
* Parentesisless command line user interface
* Easy implimentation of user defined commands
* Multiple tree projection scheme

Following features are not implemented, but we will make them in the future release.

* Graphical User Interfaces for Loop operation
* Editor for steering file and histgrum definition file


Installation
------------

ARTEMIS depends on [YAML-CPP](https://code.google.com/p/yaml-cpp/) for loading steering and histgrum definition files. Please install it in advance.

1. Download latest **ARTEMIS** from [the release page](https://github.com/artemis-dev/artemis) or develop version
1. run autoreconf with "--install" option
1. configure ARTEMIS with "--with-yaml-cpp=[install directory of YAML-CPP]"
1. make and make install


Licence
-------
Copyright 2013-2014 Shinsuke Ota <<ota@cns.s.u-tokyo.ac.jp>> and ARTEMIS develop team
