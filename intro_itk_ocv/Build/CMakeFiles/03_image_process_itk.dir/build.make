# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/Build"

# Include any dependencies generated for this target.
include CMakeFiles/03_image_process_itk.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/03_image_process_itk.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/03_image_process_itk.dir/flags.make

CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o: CMakeFiles/03_image_process_itk.dir/flags.make
CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o: ../03_image_process_itk.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/Build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o -c "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/03_image_process_itk.cxx"

CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/03_image_process_itk.cxx" > CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.i

CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/03_image_process_itk.cxx" -o CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.s

CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.requires:

.PHONY : CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.requires

CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.provides: CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.requires
	$(MAKE) -f CMakeFiles/03_image_process_itk.dir/build.make CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.provides.build
.PHONY : CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.provides

CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.provides.build: CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o


# Object files for target 03_image_process_itk
03_image_process_itk_OBJECTS = \
"CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o"

# External object files for target 03_image_process_itk
03_image_process_itk_EXTERNAL_OBJECTS =

03_image_process_itk: CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o
03_image_process_itk: CMakeFiles/03_image_process_itk.dir/build.make
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libdouble-conversion.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libz.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libjpeg.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libpng.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libtiff.so
03_image_process_itk: /usr/lib/libi2d.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmjpeg.so.12.3.6.2
03_image_process_itk: /usr/lib/libijg8.so.12.3.6.2
03_image_process_itk: /usr/lib/libijg12.so.12.3.6.2
03_image_process_itk: /usr/lib/libijg16.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmjpls.so.12.3.6.2
03_image_process_itk: /usr/lib/libcmr.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmwlm.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmpstat.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmrt.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmseg.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmtract.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmpmap.so.12.3.6.2
03_image_process_itk: /usr/lib/libITKDeprecated-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOLSM-4.12.so.1
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libexpat.so
03_image_process_itk: /usr/lib/libITKIOMesh-4.12.so.1
03_image_process_itk: /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5_cpp.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libpthread.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libsz.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libdl.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libm.so
03_image_process_itk: /usr/lib/libITKIOCSV-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOHDF5-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOMRC-4.12.so.1
03_image_process_itk: /usr/lib/libITKOptimizersv4-4.12.so.1
03_image_process_itk: /usr/lib/libITKReview-4.12.so.1
03_image_process_itk: /usr/lib/libITKVideoIO-4.12.so.1
03_image_process_itk: /usr/lib/libdcmtls.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmsr.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmimage.so.12.3.6.2
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libjpeg.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libpng.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libtiff.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libxml2.so
03_image_process_itk: /usr/lib/libdcmdsig.so.12.3.6.2
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libssl.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libcrypto.so
03_image_process_itk: /usr/lib/libdcmqrdb.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmnet.so.12.3.6.2
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libwrap.so
03_image_process_itk: /usr/lib/libdcmimgle.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmfg.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmiod.so.12.3.6.2
03_image_process_itk: /usr/lib/libdcmdata.so.12.3.6.2
03_image_process_itk: /usr/lib/liboflog.so.12.3.6.2
03_image_process_itk: /usr/lib/libofstd.so.12.3.6.2
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libicuuc.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libicudata.so
03_image_process_itk: /usr/lib/libITKDICOMParser-4.12.so.1
03_image_process_itk: /usr/lib/libITKgiftiio-4.12.so.1
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libexpat.so
03_image_process_itk: /usr/lib/libITKIOBMP-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOGDCM-4.12.so.1
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libgdcmMSFF.so.2.8.4
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libgdcmDICT.so.2.8.4
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libgdcmIOD.so.2.8.4
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libgdcmDSED.so.2.8.4
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libgdcmCommon.so.2.8.4
03_image_process_itk: /usr/lib/libITKIOGIPL-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOJPEG-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOMeta-4.12.so.1
03_image_process_itk: /usr/lib/libITKIONIFTI-4.12.so.1
03_image_process_itk: /usr/lib/libITKIONRRD-4.12.so.1
03_image_process_itk: /usr/lib/libITKNrrdIO-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOPNG-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOTIFF-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOVTK-4.12.so.1
03_image_process_itk: /usr/lib/libITKLabelMap-4.12.so.1
03_image_process_itk: /usr/lib/libITKQuadEdgeMesh-4.12.so.1
03_image_process_itk: /usr/lib/libITKPolynomials-4.12.so.1
03_image_process_itk: /usr/lib/libITKBiasCorrection-4.12.so.1
03_image_process_itk: /usr/lib/libITKBioCell-4.12.so.1
03_image_process_itk: /usr/lib/libITKFFT-4.12.so.1
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libfftw3.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libfftw3_threads.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libfftw3f.so
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libfftw3f_threads.so
03_image_process_itk: /usr/lib/libITKIOBioRad-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOStimulate-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOSpatialObjects-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOXML-4.12.so.1
03_image_process_itk: /usr/lib/libITKFEM-4.12.so.1
03_image_process_itk: /usr/lib/libITKMetaIO-4.12.so.1
03_image_process_itk: /usr/lib/x86_64-linux-gnu/libz.so
03_image_process_itk: /usr/lib/libITKOptimizers-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOGE-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOSiemens-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOIPL-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOTransformHDF5-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOTransformInsightLegacy-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOTransformMatlab-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOTransformBase-4.12.so.1
03_image_process_itk: /usr/lib/libITKTransformFactory-4.12.so.1
03_image_process_itk: /usr/lib/libITKKLMRegionGrowing-4.12.so.1
03_image_process_itk: /usr/lib/libITKVTK-4.12.so.1
03_image_process_itk: /usr/lib/libITKWatersheds-4.12.so.1
03_image_process_itk: /usr/lib/libITKStatistics-4.12.so.1
03_image_process_itk: /usr/lib/libitkNetlibSlatec-4.12.so.1
03_image_process_itk: /usr/lib/libITKSpatialObjects-4.12.so.1
03_image_process_itk: /usr/lib/libITKMesh-4.12.so.1
03_image_process_itk: /usr/lib/libITKTransform-4.12.so.1
03_image_process_itk: /usr/lib/libITKPath-4.12.so.1
03_image_process_itk: /usr/lib/libitkopenjpeg-4.12.so.1
03_image_process_itk: /usr/lib/libITKIOImageBase-4.12.so.1
03_image_process_itk: /usr/lib/libITKVideoCore-4.12.so.1
03_image_process_itk: /usr/lib/libITKCommon-4.12.so.1
03_image_process_itk: /usr/lib/libitksys-4.12.so.1
03_image_process_itk: /usr/lib/libITKVNLInstantiation-4.12.so.1
03_image_process_itk: /usr/lib/libitkvnl_algo-4.12.so.1
03_image_process_itk: /usr/lib/libitkvnl-4.12.so.1
03_image_process_itk: /usr/lib/libitkv3p_netlib-4.12.so.1
03_image_process_itk: /usr/lib/libitknetlib-4.12.so.1
03_image_process_itk: /usr/lib/libitkvcl-4.12.so.1
03_image_process_itk: CMakeFiles/03_image_process_itk.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/Build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 03_image_process_itk"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/03_image_process_itk.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/03_image_process_itk.dir/build: 03_image_process_itk

.PHONY : CMakeFiles/03_image_process_itk.dir/build

CMakeFiles/03_image_process_itk.dir/requires: CMakeFiles/03_image_process_itk.dir/03_image_process_itk.cxx.o.requires

.PHONY : CMakeFiles/03_image_process_itk.dir/requires

CMakeFiles/03_image_process_itk.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/03_image_process_itk.dir/cmake_clean.cmake
.PHONY : CMakeFiles/03_image_process_itk.dir/clean

CMakeFiles/03_image_process_itk.dir/depend:
	cd "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/Build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv" "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv" "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/Build" "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/Build" "/home/sistemas/Escritorio/Taller 2/intro_itk_ocv/Build/CMakeFiles/03_image_process_itk.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/03_image_process_itk.dir/depend

