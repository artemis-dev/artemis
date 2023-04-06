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

The artemis needs several root packages which may not be installed automatically in some operating suystem or distributions. Please check the existence of the packages or enable them.
* Minuit2 (root-minuit2)
* Geometry (root-geom)

ARTEMIS depends on [YAML-CPP](https://code.google.com/p/yaml-cpp/) 0.7.0 for loading steering and histgrum definition files. Please install it in advance as a shared object. (You have to cmake with -DYAML_BUILD_SHARED_LIBS=ON, -DBUILD_TESTING=OFF)
(sudo apt install libyaml-cpp-dev works on recent ubuntu)


1. Download this cmake version of **ARTEMIS** from [the release page](https://github.com/rin-yokoyama/artemis) with cmake branch 
1. configure ARTEMIS with cmake
```
git clone https://github.com/rin-yokoyama/artemis.git -b cmake
cd artemis
mkdir build
cd build
cmake .. (or ccmake ..)
```
* Options
  * -DCMAKE_INSTALL_PREFIX=[install path (default=./install)]
  * -DBUILD_GET=[ON/OFF (default=OFF)]
  * -DWITH_GET_DECODER=[path to GET decoder (required when BUILD_GET=ON)]
  * -DCMAKE_PREFIX_PATH=[path to yaml-cpp/openMPI if not found automatically] (yaml-cpp is required. You can also try setting yaml-cpp/lib64/pkgconfig to PKG_CONFIG_PATH in your environment. If openMPI is not found, it disables USE_MPI.)
3. make and make install
3. "source thisartemis.sh" in install/bin or add install/share/modulefiles to modulespath if you use environment modules then you can use "module load artemis".
3. If you want to build a project using artemis you can use find_package(artemis) in your cmake

Check https://github.com/rin-yokoyama/artemis-oedo as an example.

Licence
-------
Copyright 2013-2014 Shinsuke Ota <<ota@cns.s.u-tokyo.ac.jp>> and ARTEMIS develop team
