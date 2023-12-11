# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/ESP/esp-idf/esp-idf/components/bootloader/subproject"
  "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader"
  "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader-prefix"
  "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader-prefix/tmp"
  "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader-prefix/src/bootloader-stamp"
  "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader-prefix/src"
  "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/user/Robin/_APPS/Stem_coach_v3/sample_project/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
