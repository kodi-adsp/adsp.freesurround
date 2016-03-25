#### CI Testing
* Tarvis-CI for OS X, iOS, Linux [![Build Status](https://travis-ci.org/kodi-adsp/adsp.freesurround.svg?branch=master)](https://travis-ci.org/kodi-adsp/adsp.freesurround)
* AppVeyor for Windows [![Build status](https://ci.appveyor.com/api/projects/status/4r006cum794fwlsb/branch/master?svg=true)](https://ci.appveyor.com/project/james4kodi-adsp/adsp-freesurround/branch/master)
* Code analyses for Linux [![Coverity Scan Build Status](https://scan.coverity.com/projects/5120/badge.svg)](https://scan.coverity.com/projects/5120)

# FreeSurround AudioDSP add-on for [Kodi] (http://kodi.tv)
*<p align="center">![alt tag](https://github.com/kodi-adsp/adsp.freesurround/blob/master/adsp.freesurround/icon.png)</p>*

*FreeSurround is based on [foo_dsp_fsurround](http://www.hydrogenaud.io/forums/index.php?showtopic=52235) from pro_optimizer for foobar2000 0.9.x.*

This add-on is the equivalent of your hi-fi receiver's Dolby ProLogic II button (except that it uses a
different algorithm) for Kodi.

It's purpose is to decode surround information from your stereo music (including mp3) to produce
multichannel output.

It works with any stereo music containing surround information like the following compatible  coding schemes:

- Dolby Surround
- Dolby ProLogic
- Dolby ProLogic II
- SRS CircleSurround
- SRS CircleSurround II
- DTS Neo:6
- Lexicon Logic7
- Stereo Quadrophonic (SQ)
- Quadrophonic Stereo (QS)

## Change LFE output settings
Currently you have to edit the generated "ADSPFreeSurrAddonSettings.xml" changed by hand to edit the cut off frequencies of the LFE channel.
Set `bassredirection` to `true` and change the settings. E.g.
```XML
        <bassredirection>true</bassredirection>
        <lowcutoff>40.000000</lowcutoff>
        <highcutoff>90.000000</highcutoff>
```
Normally it's a part of the AudioDSP post processing system that's why it is not directly editable from a Kodi window!

## Build instructions for Kodi's binary add-ons

When building the addon you have to use the correct branch depending on which version of Kodi you're building against. 
For example, if you're building the `Jarvis` branch of Kodi you should checkout the `Jarvis` branch of this repository. 
Addon releases are also tagged regularly.

For further instructions replace `{addon-id}` and `{org}` with the repository- and the Github organisation-name.
E.g. `https://github.com/kodi-adsp/adsp.basic.git` results in `{addon-id}=adsp.basic` and `{org}=kodi-adsp`

With the following instructions the add-on files will be placed in `../../xbmc/addons`. Consequently this adds a system add-on, which can be used direclty if you build Kodi from source.

### Linux

1. `git clone https://github.com/xbmc/xbmc.git`
2. `git clone https://github.com/{org}/{addon-id}.git`
3. `cd {addon-id} && mkdir build && cd build`
4. `cmake -DADDONS_TO_BUILD={addon-id} -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../xbmc/addons -DPACKAGE_ZIP=1 ../../xbmc/project/cmake/addons`
5. `make`

### Windows

Use the visual studio command line or run `call "%VS120COMNTOOLS%..\..\VC\bin\vcvars32.bat"`

1. `git clone https://github.com/xbmc/xbmc.git`
2. `git clone https://github.com/{org}/{addon-id}.git`
3. `cd {addon-id} && mkdir build && cd build`
4. `cmake -G "NMake Makefiles" -DADDONS_TO_BUILD={addon-id} -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../xbmc/addons -DPACKAGE_ZIP=1 ../../xbmc/project/cmake/addons`
5. `cmake --build "%cd%" --target "{addon-id}"`


## Useful links

* [Kodi's PVR user support] (http://forum.kodi.tv/forumdisplay.php?fid=167)
* [General binary add-on development] (http://forum.kodi.tv/forumdisplay.php?fid=26)
* [Kodi Core development support] (http://forum.kodi.tv/forumdisplay.php?fid=93)
* [Kodi AudioDSP development support] (http://forum.kodi.tv/forumdisplay.php?fid=235)
* [Kodi Screensavers and Visualisations development support] (http://forum.kodi.tv/forumdisplay.php?fid=38)
* [Kodi PVR development support] (http://forum.kodi.tv/forumdisplay.php?fid=136)

-------------
#### License
[GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007](https://github.com/kodi-adsp/adsp.freesurround/blob/master/LICENSE.md)
