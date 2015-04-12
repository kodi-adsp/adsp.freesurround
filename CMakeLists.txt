project(adsp.freesurround)

cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(kodi REQUIRED)
find_package(kodiplatform REQUIRED)

include_directories(${kodiplatform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR})

set(FREESURROUND_SOURCES src/addon.cpp
                         src/GUIDialogFreeSurround.cpp
                         src/kissfft/kiss_fft.cpp
                         src/kissfft/kiss_fftr.cpp
                         src/DSPProcessFreeSurround.cpp
                         src/FreeSurroundSettings.cpp
                         src/ChannelMaps.cpp
                         src/FreeSurroundDecoder.cpp)

set(DEPLIBS ${kodiplatform_LIBRARIES})

build_addon(adsp.freesurround FREESURROUND DEPLIBS)

add_definitions(-DFREESURROUND_VERSION="${FREESURROUND_VERSION}")

include(CPack)
