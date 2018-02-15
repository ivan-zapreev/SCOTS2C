#Introduction

#Installation of third party software

In order to run the software one requires Mathematica (<https://www.wolfram.com/mathematica/>) being installed. In addition one will require:

1. GCC(<https://gcc.gnu.org/>) - the GNU compiler collection
2. CMAKE(<https://cmake.org/>) - the cross platform software building tool
3. Automake 1.14.1 (<http://ftp.gnu.org/gnu/automake/>) - the cross platform software building tool

The building and installation instructions for the listed tools come with the tools.

Note that, SCOTSv2.0 (<https://gitlab.lrz.de/matthias/SCOTSv0.2>) and CUDDv3.0.0 (<http://vlsi.colorado.edu/~fabio/>) are already integrated into the project under ./ext. SCOTSv2.0 is a header-only C++ library so it does not require building or installation. The supplied CUDDv3.0.0 is already fixed  a way required by SCOTSv2.0, see Section 3.1 of the SCOTSv2.0 manual. However it steel needs to be build and installed. 

On any platform CUDD requires the UUID package, to be more precise the development version thereof, on Ubuntu known as "uuid-dev". This library can be installed for instance by running "sudo apt-get install uuid-dev". Below we list two instruction sets for building and installing CUDD on two platforms:

##Max OS X

  -  On MAC OS X we use the following sequence:

```
	export CUDDPATH=/opt/local/
	cd ./ext/cudd-3.0.0
	./configure --enable-shared --enable-obj \
	--enable-dddmp --prefix=${CUDDPATH}
	make
	sudo make install
```
##Ubuntu

  -  On Ubuntu we use the following sequence:

```
	export CUDDPATH=/usr/local
	cd ./ext/cudd-3.0.0
	./configure --enable-shared --enable-obj \
	--enable-dddmp --prefix=${CUDDPATH}
	make
	sudo make install
```
  -  Make sure that the "./ext/cudd-3.0.0/util/util.h" and "./ext/cudd-3.0.0/config.h" have been copied into "${CUDDPATH}/include" by running

```
	find ${CUDDPATH}/include -name "util.h"
	find ${CUDDPATH}/include -name "config.h"
```

  -  If one of the header files was not copied then use an appropriate command
	
```
	sudo cp ./ext/cudd-3.0.0/util/util.h ${CUDDPATH}/include
	sudo cp ./ext/cudd-3.0.0/config.h ${CUDDPATH}/include
```

Note that, the tooling uses FindMathematica CMake package (<https://github.com/sakra/FindMathematica/>) for finding the Mathematica distribution. It means that Mathematica is expected to be installed into a standard location. For more details see the package's manual <https://github.com/sakra/FindMathematica/blob/master/MANUAL.md>

#Building the software
Build the SCOTSv2.0 to Mathematica interface package from the project root by

	mkdir build
	cd build
	cmake ..
	make

Note that, if source code of the software has been changed then one is required to re-run:

	cmake ..
	make

The software comes in two versions, one version is WSTP based and the other uses LibraryLink. They are mostly identical in functionality with the exception of LibraryLink version being able to retrieve the set of all grid points at once. Further we shall first describe how the WSTP version is to be installed and used and then the same will be done for the LibraryLink version. The former is ran as a separate process and thus has not impact on the Mathematica Kernel, and benefits from all of the WSTP features. Yet the WSTP version is slower than the LibraryLink version which is however run inside the Mathematica Kernel and thus, in case of failures, may cause an interference.

#Installing the WSTP software
Install the `build/src/wstp/scots2ext` WSTP application through the Notebook in Mathematica.

   1. Load through interface:
   
      	SetDirectory["<PROJECT_LOCATION>/mscots2bdd/build/src/wstp"]
      	link = Install["./scots2ext"]
      	LinkPatterns[link]
      	
   2. Run from command line and connect through interface.
	In command-line console run
	
		$./build/src/wstp/scots2ext
	In the application's prompt type:
		
		myPort
		
	Within Mathematica use
	
		link = Install["myPort", LinkMode -> Connect]
		
   In both cases to uninstall or disconnect use
   
		Uninstall[link]

Install the `src/scots2.m` package through the Mathematica Menu:

       File>Install...
       
Choose the install name to be `scots2`.

#Using the WSTP software

1. Load the SCOTSv2.0 controller (*.scs) by calling
       SCOTSv2`LoadBDDController[]
   or
       Global`LoadSCOTSv2BDD["<PATH_TO_CONTROLLER>/controller"]
   Note that the ".scs" file extension is ommited.

   At the moment only the BDD controllers are supported. See the pre-generated example controllers from SCOTSv2.0 located in:
   ./data/models

2. To see the operations available via the link use
	LinkPatterns[link]

3. Get the controller's number of dimensions
       Global`GetDim[]

4. Get the list of the discretization parameters
       Global`GetEta[]

5. Get the list of the lower left coordinate of the grid
       Global`GetLowerLeft[]

6. Get the list of the upper right coordinate of the grid
       Global`GetUpperRight[]

7. Get the input/control values for the given state space point
       Global`Restriction[x_List]
   The argument here is the list of real values, the number of values shall
   be equal to the number of dimensions in the state space.
   The result is the list of real values of the control signals.
   In case the control signal is multi-dimensional then the list is
   made plain, e.g. for the list of 2D control signals :
      {(u^1_1, u^1_2), (u^2_1, u^2_2), (u^3_1, u^3_2)}
   one shall get the plain list:
      {u^1_1, u^1_2, u^2_1, u^2_2, u^3_1, u^3_2}

8. Uninstall the application by
       Uninstall[link]

#Installing the LibraryLink software

Load the build/src/liblink/scots2int.dylink (Mac OS X) or build/src/liblink/scots2int.so (Linux) dynamic library functions into Mathematica kernel by following the next steps:

	SetDirectory["<PATH_TO_PROJECT_LOCATION>/mscots2bdd/build/src/liblink"]	
	LoadCtrl = LibraryFunctionLoad["./scots2int", "load_controller_bdd", {UTF8String}, Integer]
	GetGridDim = LibraryFunctionLoad["./scots2int", "get_dim", {}, mint]
	GetGridEtha = LibraryFunctionLoad["./scots2int", "get_eta", {}, {Real, 1}]
	GetLowerLeft = LibraryFunctionLoad["./scots2int", "get_lower_left", {}, {Real, 1}]
	GetUpperRight = LibraryFunctionLoad["./scots2int", "get_upper_right", {}, {Real, 1}]
	GetGridPoints = LibraryFunctionLoad["./scots2int", "get_grid_points", {}, {Real, 1}]
	GetGridRestrict = LibraryFunctionLoad["./scots2int", "restriction", {{Real, 1}}, {Real, 1}]

Note that, the LoadCtrl (load_controller_bdd) function returns the number of DdNodes in the BDD which represents the its size.

In all cases to un-load a library function one needs to use LibraryFunctionUnload. In order to unload the entire library one needs using LibraryUnload. However the latter does not always work, at least not on all platforms. Therefore, is a new version of the library is to be loaded one needs to re-start Mathematica first to let the previous version be unloaded.

#Using the LibraryLink software

The example usage of the LibraryLink software is given in the ./data/liblink_example.nb Please make sure that the absolute paths to the generated shared library and the bdd controller are updated. The example is made and tested on Mac OS X and Linux.
