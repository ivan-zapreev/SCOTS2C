# **Introduction**

This project consists of several SCOTSv2.0 BDD controllers related software components:

1. The BDD controller interfaces for Wolfram Mathematica:
	
	1.1 WSTP interface
	
	1.2 LibraryLink interface
	
2. BDD controller related software:
	
	2.1 `scots_opt_det` - the BDD controller determinizer
	
	2.2 `scots_to_svg` - the BDD controller to SVG image converter
	
	2.3 `scots_split_det` - the BDD controller per-input value splitter

The former can be used in order to work with SCOTSv2.0 BDD controllers from Mathematica. The latter can be used to: *(i)* determinize BDD controllers, in order to reduce their size; *(ii)* visualize the BDD controller as a 2D image; *(iii)* split the controller into parts corresponding to different control input values.

# **Third party software**
Our software makes use of several open-source libraries that, for your convenience, are pre-packed under the project's `./ext/` folder.

**NOTE:** *The third party software is not guaranteed to be identical to the same versions provided on the official web sites. It has been modified for correctness and extended with new functionality.*

Most of the used third-party software is C++ header only libraries that do not require building or installation:

1. SCOTSv2.0 (<https://gitlab.lrz.de/matthias/SCOTSv0.2>) - the tool for generating symbolic controllers
2. svgDrawer (<https://github.com/openMVG-thirdparty/vectorGraphics>) - an easy SVG image C++ writer
3. TCLAP (<http://tclap.sourceforge.net/>) - a small, flexible library for defining and accessing command line arguments

There is yet one library which does require building and installation:

* CUDDv3.0.0 (<http://vlsi.colorado.edu/~fabio/>) - the Binary Decision Diagrams (BDD) management library

The instructions on building and installing the latter one (on two supported platforms) are given in the next section.

# **Preparing for building**

Before the software provided in this repository can build, make sure that you have the following standard tools installed:

1. GCC(<https://gcc.gnu.org/>) - the GNU compiler collection
2. CMAKE(<https://cmake.org/>) - the cross platform software building tool
3. Automake 1.14 (<http://ftp.gnu.org/gnu/automake/>) - the cross platform software building tool.

**NOTE:** If another automake version is used you will be getting a

```WARNING: 'aclocal-1.14' is missing on your system.```

message, preventing you to build CUDD. The rest of the section is devoted to building CUDD on Max OS X and Ubuntu platforms.

## Max OS X

-  Perform the following building/installation sequence:

```
export CUDDPATH=/opt/local/
cd ./ext/cudd-3.0.0
./configure --enable-shared --enable-obj \
--enable-dddmp --prefix=${CUDDPATH}
make
sudo make install
```

## Ubuntu

- Make sure that the development version of the UUID package "uuid-dev" is installed:

```sudo apt-get install uuid-dev```

-  Perform the following building/installation sequence:

```
export CUDDPATH=/usr/local
cd ./ext/cudd-3.0.0
./configure --enable-shared --enable-obj \
--enable-dddmp --prefix=${CUDDPATH}
make
sudo make install
```

-  Make sure that the `./ext/cudd-3.0.0/util/util.h` and `./ext/cudd-3.0.0/config.h` have been copied into `${CUDDPATH}/include` by running

```
find ${CUDDPATH}/include -name "util.h"
find ${CUDDPATH}/include -name "config.h"
```

-  If one of the header files was not copied then use an appropriate command
	
```
sudo cp ./ext/cudd-3.0.0/util/util.h ${CUDDPATH}/include
sudo cp ./ext/cudd-3.0.0/config.h ${CUDDPATH}/include
```

### What you need to build WSTP and LibrayLink interfaces:

For building *WSTP* and *LibraryLink* interfaces on required Wolfram Mathematica (<https://www.wolfram.com/mathematica/>) to be installed. Note that, our building scripts use `FindMathematica` CMake package (<https://github.com/sakra/FindMathematica/>) for finding the Mathematica distribution. The latter assumes that Mathematica is expected to be installed into a standard location. For more details see the package's manual <https://github.com/sakra/FindMathematica/blob/master/MANUAL.md>

# **Building the software**

Build the SCOTSv2.0 to Mathematica interface package from the project root by

```
mkdir build
cd build
cmake ..
make
```

Note that, if source code of the software has been changed (new source have been added) then one is required to re-run:

```
cmake ..
make
```

The Mathematica interface software comes in two versions: one version is WSTP based and the other uses LibraryLink. They are mostly identical in functionality with the exception of LibraryLink version being able to retrieve the set of all grid points at once. Further we shall first describe how the WSTP version is to be installed and used and then the same will be done for the LibraryLink version. The former is ran as a separate process and thus has not impact on the Mathematica Kernel, and benefits from all of the WSTP features. Yet the WSTP version is slower than the LibraryLink version which is however run inside the Mathematica Kernel and thus, in case of failures, may cause an interference.

# **Using the command line utilities**
The build command line utilities will be located inside the `./build/src/optdet/` folder. Each of them has a list of command line parameters and options that can be seen if the executable is run on the command line.

## `./scots_opt_det`
This software can be used to determinize BDD controllers provided by SCOTSv2.0 using a number of different deterministic algorithms as described and analyzed in **`[ZVM_ADHS_2018]`** and **`[ZVM_ArXiV_2018]`**.

```
$ ./scots_opt_det --help
USAGE:  ------------------------------------------------------------------ 
USAGE: |                  BDD Determinizer for SCOTSv2.0        :)\___/(: |
USAGE: |                       Software version 1.0             {(@)v(@)} |
USAGE: |                        DCSC, TU Delft, NL              {|~- -~|} |
USAGE: |            Copyright (C) Dr. Ivan S Zapreev, 2017-2018 {/^'^'^\} |
USAGE: |  ═════════════════════════════════════════════════════════m-m══  |
USAGE: |        This software is distributed under GPL 2.0 license        |
USAGE: |          (GPL stands for GNU General Public License)             |
USAGE: |          The product comes with ABSOLUTELY NO WARRANTY.          |
USAGE: |   This is a free software, you are welcome to redistribute it.   |
USAGE: |                     Running in 64 bit mode!                      |
USAGE: |                 Build on: May 14 2018 11:39:30                   |
USAGE:  ------------------------------------------------------------------ 

USAGE: 

   ./scots_opt_det  [-l <error|warn|usage|result|info|info1|info2|info3>]
                    -a <local|global|mixed|bdd-local|bdd-mixed> [-n] [-x]
                    [-g] [-c] [-e] [-r] -d <state-space dimensionality> -t
                    <target controller file name> -s <source controller
                    file name> [--] [--version] [-h]
Where: 

   -l <error|warn|usage|result|info|info1|info2|info3>,  --logging <error
      |warn|usage|result|info|info1|info2|info3>
     The log level to be used

   -a <local|global|mixed|bdd-local|bdd-mixed>,  --algorithm <local|global
      |mixed|bdd-local|bdd-mixed>
     (required)  Define the determinization algorithm

   -n,  --bdd-angled
     Compress using linear functions on the internal bdd state ids

   -x,  --bdd-constant
     Compress using constant functions on the internal bdd state ids

   -g,  --angled
     Compress using linear functions to optimize resulting BDD size

   -c,  --constant
     Compress using constant functions to optimize resulting BDD size

   -e,  --extend
     Extend to larger grid to optimize resulting BDD size

   -r,  --reorder
     Reorder variables to optimize resulting BDD size

   -d <state-space dimensionality>,  --state-dimension <state-space
      dimensionality>
     (required)  The number of state space dimensions

   -t <target controller file name>,  --target-controller <target
      controller file name>
     (required)  The SCOTSv2.0 BDD controller file name without (.scs/.bdd)

   -s <source controller file name>,  --source-controller <source
      controller file name>
     (required)  The SCOTSv2.0 BDD controller file name without (.scs/.bdd)

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```

## `./scots_split_det`
This software allows to do two things:

1. Extract the domain states of a BDD controller into a separate BDD file
2. Split the provided controller into a number of controllers, per input value.

The former removes all the inputs from the original controller and only preserves the domain states. The latter generates a number of BDD sub-controllers each of which contains states having one specific input.

```
$ ./scots_split_det --help
USAGE:  ------------------------------------------------------------------ 
USAGE: |                    BDD Splitter for SCOTSv2.0          :)\___/(: |
USAGE: |                       Software version 1.0             {(@)v(@)} |
USAGE: |                        DCSC, TU Delft, NL              {|~- -~|} |
USAGE: |            Copyright (C) Dr. Ivan S Zapreev, 2017-2018 {/^'^'^\} |
USAGE: |  ═════════════════════════════════════════════════════════m-m══  |
USAGE: |        This software is distributed under GPL 2.0 license        |
USAGE: |          (GPL stands for GNU General Public License)             |
USAGE: |          The product comes with ABSOLUTELY NO WARRANTY.          |
USAGE: |   This is a free software, you are welcome to redistribute it.   |
USAGE: |                     Running in 64 bit mode!                      |
USAGE: |                 Build on: May 14 2018 11:39:30                   |
USAGE:  ------------------------------------------------------------------ 

USAGE: 

   ./scots_split_det  [-l <error|warn|usage|result|info|info1|info2|info3>]
                      [-p] [-i] -d <state-space dimensionality> -t <target
                      controller file name> -s <source controller file
                      name> [--] [--version] [-h]
Where: 

   -l <error|warn|usage|result|info|info1|info2|info3>,  --logging <error
      |warn|usage|result|info|info1|info2|info3>
     The log level to be used

   -p,  --support
     Request the reordered  controller support BDD

   -i,  --input
     Request the splitting the controller per input

   -d <state-space dimensionality>,  --state-dimension <state-space
      dimensionality>
     (required)  The number of state space dimensions

   -t <target controller file name>,  --target-controller <target
      controller file name>
     (required)  The SCOTSv2.0 BDD controller file name without (.scs/.bdd)

   -s <source controller file name>,  --source-controller <source
      controller file name>
     (required)  The SCOTSv2.0 BDD controller file name without (.scs/.bdd)

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```

## `./scots_to_svg`
This software allows to convert a BDD controller into a 2D SVG image. The horizontal axis thereof corresponds to the state index and the vertical axis to an input index. Note that, one can use regular SCOTSv2.0 or BDD-induced indexes. The plot is a discrete multi-valued function of the controller.

```
$ ./scots_to_svg --help
USAGE:  ------------------------------------------------------------------ 
USAGE: |               SCOTSv2.0 controller to SVG converter    :)\___/(: |
USAGE: |                       Software version 1.0             {(@)v(@)} |
USAGE: |                        DCSC, TU Delft, NL              {|~- -~|} |
USAGE: |            Copyright (C) Dr. Ivan S Zapreev, 2017-2018 {/^'^'^\} |
USAGE: |  ═════════════════════════════════════════════════════════m-m══  |
USAGE: |        This software is distributed under GPL 2.0 license        |
USAGE: |          (GPL stands for GNU General Public License)             |
USAGE: |          The product comes with ABSOLUTELY NO WARRANTY.          |
USAGE: |   This is a free software, you are welcome to redistribute it.   |
USAGE: |                     Running in 64 bit mode!                      |
USAGE: |                 Build on: May 14 2018 11:39:30                   |
USAGE:  ------------------------------------------------------------------ 

USAGE: 

   ./scots_to_svg  [-l <error|warn|usage|result|info|info1|info2|info3>]
                   [-b] -d <state-space dimensionality> -t <target
                   controller file name> -s <source controller file name>
                   [--] [--version] [-h]
Where: 

   -l <error|warn|usage|result|info|info1|info2|info3>,  --logging <error
      |warn|usage|result|info|info1|info2|info3>
     The log level to be used

   -b,  --bdd
     Request the bdd ids plotting instead of scots abstract ids

   -d <state-space dimensionality>,  --state-dimension <state-space
      dimensionality>
     (required)  The number of state space dimensions

   -t <target controller file name>,  --target-controller <target
      controller file name>
     (required)  The SCOTSv2.0 BDD controller file name without (.scs/.bdd)

   -s <source controller file name>,  --source-controller <source
      controller file name>
     (required)  The SCOTSv2.0 BDD controller file name without (.scs/.bdd)

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```

## `./scots_opt_lis`

**WARNING:** Is an experimental piece that at the moment does not work, please ignore!

# **Installing the WSTP software**

Install the `build/src/wstp/scots2ext` WSTP application through the Notebook in Mathematica.

   1. Load through interface:

```   
SetDirectory["<PROJECT_LOCATION>/mscots2bdd/build/src/wstp"]
link = Install["./scots2ext"]
LinkPatterns[link]
```
      	
   2. Run from command line and connect through interface.
	In command-line console run

```	
$./build/src/wstp/scots2ext
```

	In the application's prompt type:

```		
myPort
```
		
	Within Mathematica use
	
```
link = Install["myPort", LinkMode -> Connect]
```
		
   In both cases to uninstall or disconnect use

```   
Uninstall[link]
```

Install the `src/scots2.m` package through the Mathematica Menu:

```
File>Install...
```
    
Choose the install name to be `scots2`.

# **Using the WSTP software**

1. Load the SCOTSv2.0 controller (*.scs) by calling
```
SCOTSv2`LoadBDDController[]
```
   or
```
Global`LoadSCOTSv2BDD["<PATH_TO_CONTROLLER>/controller"]
```
   Note that the `.scs` file extension is omitted.

   At the moment only the BDD controllers are supported. See the pre-generated example controllers from SCOTSv2.0 located in:
```
./data/models
```

2. To see the operations available via the link use `LinkPatterns[link]`

3. Get the controller's number of dimensions ``Global`GetDim[]``

4. Get the list of the discretization parameters ``Global`GetEta[]``

5. Get the list of the lower left coordinate of the grid ``Global`GetLowerLeft[]``

6. Get the list of the upper right coordinate of the grid ``Global`GetUpperRight[]``

7. Get the input/control values for the given state space point ``Global`Restriction[x_List]``  The argument here is the list of real values, the number of values shall be equal to the number of dimensions in the state space. The result is the list of real values of the control signals.  In case the control signal is multi-dimensional then the list is made plain, e.g. for the list of 2D control signals :
      ``{(u^1_1, u^1_2), (u^2_1, u^2_2), (u^3_1, u^3_2)}``
   one shall get the plain list:
      ``{u^1_1, u^1_2, u^2_1, u^2_2, u^3_1, u^3_2}``

8. Uninstall the application by
       ``Uninstall[link]``

# **Installing the LibraryLink software**

Load the `build/src/liblink/scots2int.dylink` (Mac OS X) or `build/src/liblink/scots2int.so` (Linux) dynamic library functions into Mathematica kernel by following the next steps:

```
SetDirectory["<PATH_TO_PROJECT_LOCATION>/mscots2bdd/build/src/liblink"]
LoadCtrl = LibraryFunctionLoad["./scots2int", "load_controller_bdd", {UTF8String}, Integer]
GetGridDim = LibraryFunctionLoad["./scots2int", "get_dim", {}, mint]
GetGridEtha = LibraryFunctionLoad["./scots2int", "get_eta", {}, {Real, 1}]
GetLowerLeft = LibraryFunctionLoad["./scots2int", "get_lower_left", {}, {Real, 1}]
GetUpperRight = LibraryFunctionLoad["./scots2int", "get_upper_right", {}, {Real, 1}]
GetGridPoints = LibraryFunctionLoad["./scots2int", "get_grid_points", {}, {Real, 1}]
GetGridRestrict = LibraryFunctionLoad["./scots2int", "restriction", {{Real, 1}}, {Real, 1}]
```

Note that, the LoadCtrl (`load_controller_bdd`) function returns the number of `DdNodes` in the BDD which represents the its size.

In all cases to un-load a library function one needs to use `LibraryFunctionUnload`. In order to unload the entire library one needs using `LibraryUnload`. However the latter does not always work, at least not on all platforms. Therefore, is a new version of the library is to be loaded one needs to re-start Mathematica first to let the previous version be unloaded.

# **Using the LibraryLink software**

The example usage of the LibraryLink software is given in the `./data/liblink_example.nb` Please make sure that the absolute paths to the generated shared library and the bdd controller are updated. The example is made and tested on Mac OS X and Linux.

# **Literature**

- **`[ZVM_ADHS_2018]`** - Ivan S. Zapreev, Cees Verdier, Manuel Mazo Jr. "Optimal Symbolic Controllers Determinization for BDD storage", IFAC Conference on Analysis and Design of Hybrid Systems (ADHS), July 2018
- **`[ZVM_ArXiV_2018]`** - Ivan S. Zapreev, Cees Verdier, Manuel Mazo Jr. "Optimal Symbolic Controllers Determinization for BDD storage", arXiv, 2018, <https://arxiv.org/abs/1803.07369>