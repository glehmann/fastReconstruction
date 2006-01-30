#Change PROJECT_NAME to the name of your project
PROJECT(fastReconstruction)

# set the name of the input image used to run the test
SET(INPUT_IMAGE "")

#include some macros from another file...
INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)

FIND_PROGRAM(IMAGE_COMPARE ImageCompare)


#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
  ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
  ITKCommon
  ITKBasicFilters
  ITKIO
)

# option for wrapping
OPTION(BUILD_WRAPPERS "Wrap library" OFF)
IF(BUILD_WRAPPERS)
   SUBDIRS(Wrapping)
ENDIF(BUILD_WRAPPERS)
   
   

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project

SET(CurrentExe "perf2D")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "perf3D")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "test2Dmin")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "test2Dmax")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "test3Dmin")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "test3Dmax")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

#ADD_TEST(Run check ${CMAKE_SOURCE_DIR}/${INPUT_IMAGE} out.png)
#ADD_TEST(CompareImage ImageCompare out.png ${CMAKE_SOURCE_DIR}/test.png)

ADD_TEST(regMin2Da test2Dmin ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 2 0)
ADD_TEST(regMin2Da_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMin2Db test2Dmin ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 5 0)
ADD_TEST(regMin2Db_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMin2Dc test2Dmin ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 9 0)
ADD_TEST(regMin2Dc_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMin2Daf test2Dmin ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 2 1)
ADD_TEST(regMin2Daf_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMin2Dbf test2Dmin ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 5 1)
ADD_TEST(regMin2Dbf_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMin2Dcf test2Dmin ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 9 1)
ADD_TEST(regMin2Dcf_Compare ${IMAGE_COMPARE} new.png old.png)


ADD_TEST(regMax2Da test2Dmax ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 2 0)
ADD_TEST(regMax2Da_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMax2Db test2Dmax ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 5 0)
ADD_TEST(regMax2Db_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMax2Dc test2Dmax ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 9 0)
ADD_TEST(regMax2Dc_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMax2Daf test2Dmax ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 2 1)
ADD_TEST(regMax2Daf_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMax2Dbf test2Dmax ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 5 1)
ADD_TEST(regMax2Dbf_Compare ${IMAGE_COMPARE} new.png old.png)

ADD_TEST(regMax2Dcf test2Dmax ${CMAKE_SOURCE_DIR}/images/cthead1.png new.png old.png 9 1)
ADD_TEST(regMax2Dcf_Compare ${IMAGE_COMPARE} new.png old.png)

#3D versions
ADD_TEST(regMin3Da test3Dmin ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 2 0)
ADD_TEST(regMin3Da_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMin3Db test3Dmin ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 5 0)
ADD_TEST(regMin3Db_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMin3Dc test3Dmin ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 9 0)
ADD_TEST(regMin3Dc_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMin3Daf test3Dmin ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 2 1)
ADD_TEST(regMin3Daf_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMin3Dbf test3Dmin ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 5 1)
ADD_TEST(regMin3Dbf_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMin3Dcf test3Dmin ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 9 1)
ADD_TEST(regMin3Dcf_Compare ${IMAGE_COMPARE} new.img old.img)


ADD_TEST(regMax3Da test3Dmax ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 2 0)
ADD_TEST(regMax3Da_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMax3Db test3Dmax ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 5 0)
ADD_TEST(regMax3Db_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMax3Dc test3Dmax ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 9 0)
ADD_TEST(regMax3Dc_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMax3Daf test3Dmax ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 2 1)
ADD_TEST(regMax3Daf_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMax3Dbf test3Dmax ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 5 1)
ADD_TEST(regMax3Dbf_Compare ${IMAGE_COMPARE} new.img old.img)

ADD_TEST(regMax3Dcf test3Dmax ${CMAKE_SOURCE_DIR}/images/ESCells.img new.img old.img 9 1)
ADD_TEST(regMax3Dcf_Compare ${IMAGE_COMPARE} new.img old.img)


#ADD_TEST(perf2D perf2D ${CMAKE_SOURCE_DIR}/images/cthead1.img)
#ADD_TEST(perf3D perf3D ${CMAKE_SOURCE_DIR}/images/ESCells.hdr)
