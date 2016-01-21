[![Linux Build Status](https://travis-ci.org/kodi-adsp/adsp.freesurround.svg?branch=master)](https://travis-ci.org/kodi-adsp/adsp.freesurround)

### adsp.freesurround
-------------
*<p align="center">![alt tag](https://github.com/kodi-adsp/adsp.freesurround/blob/master/adsp.freesurround/icon.png)</p>*
*<p align="center">FreeSurround ADSP-Addon for KODI</p>*
-------------
*<p align="center">http://kodi.tv</p>*
-------------

-------------
*Is based on foo_dsp_fsurround from pro_optimizer on foobar2000 0.9.x.*
*URL:* http://www.hydrogenaud.io/forums/index.php?showtopic=52235

FreeSurround is meant to be the KODI equivalent of your hi-fi receiver's Dolby ProLogic II button (except that it uses a
different algorithm).

It's purpose is to decode surround information from your stereo music (including mp3, of course), and to produce
multichannel output from it. 

It works with any stereo music containing surround info in a coding scheme compatible with any the following decoders:

- Dolby Surround
- Dolby ProLogic
- Dolby ProLogic II
- SRS CircleSurround
- SRS CircleSurround II
- DTS Neo:6
- Lexicon Logic7
- Stereo Quadrophonic (SQ)
- Quadrophonic Stereo (QS)

-------------
To enable LFE channel output from add-on, must be after first start generated "ADSPFreeSurrAddonSettings.xml" changed by hand.
There are the following values (set to true and also the cut off frequencies can be changed):
```XML
        <bassredirection>false</bassredirection>
        <lowcutoff>40.000000</lowcutoff>
        <highcutoff>90.000000</highcutoff>
```
Thats normally part of DSP system post process thats why it is not selectable on skin!

-------------
### License
[GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007](https://github.com/kodi-adsp/adsp.freesurround/blob/master/LICENSE.md)
