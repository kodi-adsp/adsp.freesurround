project(adsp.freesurround)

cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(p8-platform REQUIRED)

include_directories(${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}/..) # Hack way with "/..", need bigger Kodi cmake rework to match right include ways (becomes done in future)

set(FREESURROUND_SOURCES src/addon.cpp
                         src/GUIDialogFreeSurround.cpp
                         src/kissfft/kiss_fft.c
                         src/kissfft/kiss_fftr.c
                         src/DSPProcessFreeSurround.cpp
                         src/FreeSurroundSettings.cpp
                         src/ChannelMaps.cpp
                         src/FreeSurroundDecoder.cpp)

set(FREESURROUND_HEADERS src/addon.h
                         src/ChannelMaps.h
                         src/DSPProcessFreeSurround.h
                         src/FreeSurroundDecoder.h
                         src/FreeSurroundSettings.h
                         src/GUIDialogFreeSurround.h
                         src/kissfft/_kiss_fft_guts.h
                         src/kissfft/kiss_fft.h
                         src/kissfft/kiss_fftnd.h
                         src/kissfft/kiss_fftndr.h
                         src/kissfft/kiss_fftr.h)

set(DEPLIBS ${kodiplatform_LIBRARIES}
            ${p8-platform_LIBRARIES})

build_addon(adsp.freesurround FREESURROUND DEPLIBS)

add_definitions(-DFREESURROUND_VERSION="${FREESURROUND_VERSION}")

include(CPack)
