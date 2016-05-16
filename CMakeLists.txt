project(adsp.freesurround)

cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(p8-platform REQUIRED)

include_directories(${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR})

set(FREESURROUND_SOURCES src/addon.cpp
                         src/GUIDialogFreeSurround.cpp
                         src/kissfft/kiss_fft.c
                         src/kissfft/kiss_fftr.c
                         src/DSPProcessFreeSurround.cpp
                         src/FreeSurroundSettings.cpp
                         src/ChannelMaps.cpp
                         src/FreeSurroundDecoder.cpp)

set(DEPLIBS ${kodiplatform_LIBRARIES}
            ${p8-platform_LIBRARIES})

build_addon(adsp.freesurround FREESURROUND DEPLIBS)

add_definitions(-DFREESURROUND_VERSION="${FREESURROUND_VERSION}")

include(CPack)
