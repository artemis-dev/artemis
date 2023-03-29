# Artemis Shared Codes

Concrete analysis codes for SHARAQ, CAT and other experiments.  The
codes in this repository is for internal collaboration since these codes
contains some research products.

## Requirement
* artemis (latest develop version)

art_analysis repository is recommended to use

## Install
* clone repository
* change directory to src directory
* make
* make install (recommended for ROOT6 to be installed into the same directory of artemis)
* load shared library named libuser.so at the beginning of artemislogon.C

## How to clone (from gitlab in CNS)

In order to cloning artemis share code via ssh, it is convenient to
setup the ssh config as described below.

```
Host gitlab.cns.s.u-tokyo.ac.jp                                                              
   Hostname www-intra.cns.s.u-tokyo.ac.jp

```

And then you can acces the repository via
```
git clone gitlab@gitlab.cns.s.u-tokyo.ac.jp:artemis/artemis-share.git
```


## Contribueters
* OTA, Shinsuke <ota@cns.s.u-tokyo.ac.jp>
* KAWASE, Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
* MASUOKA, Shoichiro <masuoka@cns.s.u-tokyo.ac.jp>


## History
2020-12-04 Remote repository for github with ROOT6 compatibility
2019-08-01 Remote repository was moved to gitlab (Ota)
2015-02-18 Add explanation of this file and other contents (Ota)
2014-06-22 File created (Kawase)
