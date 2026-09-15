AFF4 CPP Light v2.0
-------------

Copyright Schatz Forensic Pty Ltd, 2017-2018

Released under the LGPL v3.0+


## About this LTRData fork

This is an LTRData fork of [aff4/aff4-cpp-lite](https://github.com/aff4/aff4-cpp-lite), a lightweight C++ library with a C API for reading AFF4 forensic image containers. The local work focuses on Windows compiler compatibility and integration through a DevIO DLL.

### Branches

| Branch | Source state |
| --- | --- |
| [`master`](https://github.com/LTRData/aff4-cpp-lite/tree/master) (default) | July 2019 source, including LTRData's Windows changes and upstream merges. `configure.ac` declares version 2.0.6. |
| [`LTRData.aff4-cpp-lite-initial`](https://github.com/LTRData/aff4-cpp-lite/tree/LTRData.aff4-cpp-lite-initial) | Adds the March 2020 `msvcr120 compatibility` commit, including a size conversion, shared property-sheet imports for unit tests, and rebuilt Windows artifacts. |

These are historical snapshots; the fork should not be assumed to track current upstream. The v2.0 introduction and Visual Studio 2015 instructions below are inherited context.

### Components and local changes

| Component | Purpose |
| --- | --- |
| [`src/`](src) | AFF4 reader library, with C++ interfaces and the [C API](src/aff4-c.h). |
| [`examples/`](examples) | `aff4-info`, `aff4-extract` and `aff4-digest` example programs. |
| [`win32/libaff4_devio/`](win32/libaff4_devio) | LTRData's read-only DevIO adapter, exporting open/read/close callbacks, image-count and sector-size queries. |
| [`win32/xwaysPlugin/`](win32/xwaysPlugin) | X-Ways image plugin project. |
| [`tests/`](tests) | Library tests; Windows test projects are also included in the solution. |

The Windows changes introduce a configurable `NOEXCEPT` macro, compiler/runtime adjustments and ARM/ARM64 project configurations. The shared [Windows property sheet](win32/libaff4/libaff4props.props) defines `NO_OPENSSL`: with that configuration, **`aff4-digest` does not calculate or print the SHA-1 digest** described in the inherited example list.

### Building this fork

Clone this repository explicitly:

```sh
git clone --branch master https://github.com/LTRData/aff4-cpp-lite.git
cd aff4-cpp-lite
```

Use `--branch LTRData.aff4-cpp-lite-initial` instead to include the later compatibility commit.

For Windows, start with [`win32/libaff4.sln`](win32/libaff4.sln). The core library and DevIO projects select `v120_xp` for Win32/x64, `v120` for ARM and `v141` for ARM64, with Windows SDK 10.0.17763.0. The checked-in settings therefore require more than the Visual Studio 2015 installation described below. Review the selected projects and configuration before building, including native NuGet dependencies.

The DevIO project additionally imports ImDisk/DevIO property sheets from fixed local paths and includes external `devio.h` and `devio_types.h` headers. Supply those dependencies and adapt the imports for your checkout. [`win32/INSTALL.txt`](win32/INSTALL.txt) retains useful dependency background, but its statement that all dependencies are included does not cover this adapter.

For a Unix-like Git checkout, run [`autogen.sh`](autogen.sh) to generate `configure` before following the inherited installation steps. This requires the Autotools toolchain and pkg-config in addition to the listed library dependencies. The historical build instructions and committed Windows binaries do not establish compatibility with current toolchains or operating systems.

For licensing, retain the upstream LGPL v3.0+ notice below and consult [LICENSE.txt](LICENSE.txt), [COPYING](COPYING) and the bundled dependencies' own notices.

## Inherited upstream README

The following requirements, examples and v2.0 background are preserved from upstream; apply the fork-specific qualifications above when using them.

---

Requirements:
-------------

* C++11 compiler (gcc/clang/msvc++)
* zlib
* snappy
* liblz4
* libraptor2
* cppunit (tests only)
* openssl (tests/examples only)
* doxygen (documentation).

General Installation (*nix):
----------------------------------

With all dependencies installed, typically, a user can:

```
$ ./configure
$ make
# make install
```

And libaff4 will be installed into /usr/local/

To override installation location, use the --prefix option, and compiler
selection may be made using environment variables as passed to ./configure.

General Installation (macOS):
----------------------------------

With all dependencies installed, typically, a user can:

```
$ ./configure CC=clang CXX=clang++ CXXFLAGS="-std=c++11 -stdlib=libc++ -O2 -g0" LDFLAGS="-stdlib=libc++ -L/opt/local/lib" --prefix=/opt/local
$ make
# make install
```

And libaff4 will be installed into /opt/local/

To override installation location, use the --prefix option, and compiler
selection may be made using environment variables as passed to ./configure.

General Installation (Windows):
-------------------------------

Solution files for Visual Studio 2015 are located in /win32.

See /win32/INSTALL.txt for full details if needing to rebuild dependencies.

## Notes:

This source repository includes the following projects
for building on Windows (via Visual Studio 2015):

* snappy 1.1.7   - http://google.github.io/snappy/
* zlib 1.2.11    - http://zlib.net
* raptor2 2.0.15 - http://librdf.org/

These projects are distributed in accordance with their relevant license.

Unit Tests:
-----------

Unit tests can be built and run with:

```
$ make check
$ make test
```

Note: Typically, ``make check`` will build and run tests, however running of 
separated out to assist with integration with IDEs.

Example Applications:
---------------------

Three example applications are contained in ./examples. These are:

* aff4-info - Display the Image metadata cotained in the container to console.
* aff4-digest - Open the given container, and create a SHA1 digest of the first 
in the container.
* aff4-extract - Open the given container, and extract the first image as a RAW/dd.

Notes:
------

v2.0.0 is a near complete rewrite of the v1.0.0 of the library, and exhibits
a completely new API model. We did this so we have a simple, easy to read C++ implementation.
 
