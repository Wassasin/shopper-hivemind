shopper-hivemind
================

Implementation of the Acquire Valued Shoppers Challenge in C++.

How to use
==========

```
Usage: ./shopper-hivemind [options]

General options:
  -h [ --help ]         display this message
  -c [ --cache ]        generate cache files,  if they do not exist yet
  -p [ --clear-cache ]  clears cache files (will ask for confirmation)
  -t [ --debug ]        use small debug subset, for code testing purposes
  -f [ --force ]        will never ask confirmation, answer [YES] to everything
  -d [ --datadir ] arg  specify in which directory to search for the data files
                        (default ./data)
```

Dependencies
============

`shopper-hivemind` can be compiled using at least `Clang 3.2` or higher.

Compilation has been tested with the following setups:
* Ubuntu 13.10, Clang 3.2
* Ubuntu 14.04, Clang 3.4

This software project uses the following packages:
* Boost with {system, filesystem, regex, program_options, iostreams}
* Msgpack
* LibSVM
* Vowpal Wabbit
* Qt5

To install the software required to compile `shopper-hivemind` on Ubuntu, use the following commands:

```
# apt-get update
# apt-get install libboost-dev libboost-system-dev libboost-filesystem-dev libboost-regex-dev libboost-program-options-dev libboost-iostreams-dev libmsgpack-dev libsvm-dev qtbase5-dev
```

How to build
============

`shopper-hivemind` can be built using both qmake and cmake. For qmake, just install QtCreator and load the .pro-file. For cmake, run:

```
$ ./cmake-linux
$ cd build
$ make
```

Data
====

To retrieve the source data, including the (partial) cached files, run

```
$ ./download-data
```

This will use up approximately 8.7GiB of disk space, so beware. When testing, try first to run with the `--debug` cli option. This will force the algorithm to use a random subset of the dataset.
