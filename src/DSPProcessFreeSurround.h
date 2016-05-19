#pragma once
/*
 *      Copyright (C) 2014-2015 Team KODI
 *      http://kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#include <string>
#include <vector>
#include <map>

#include "kodi_adsp_types.h"
#include "FreeSurroundDecoder.h"
#include "FreeSurroundSettings.h"

#define SURROUND_BUFSIZE 8192

class CDSPProcess_FreeSurround
{
private:
  const unsigned int          m_StreamID;           /*!< @brief unique id of the audio stream packets */
  AE_DSP_SETTINGS             m_Settings;           /*!< @brief the active XBMC audio settings */
  int                         m_InputChannels;      /*!< @brief Current input channel format */
  unsigned int                m_SampleRate;         /*!< @brief Stream sample rate */
  sSettings                   m_Params;             /*!< @brief The actual settings from settings file */
  class CFreeSurroundDecoder *m_Decoder;            /*!< @brief the surround decoder */
  float                      *m_InbufArray[2];      /*!< @brief Input buffer for free surround decoder */
  unsigned int                m_ProcessBufferPos;   /*!< @brief amount in lt,rt */
  int                         m_ProcessedSize;      /*!< @brief amount processed */
  int                         m_LatencyFrames;      /*!< @brief number of frames of incurred latency */
  channel_setup               m_DecoderChannelSetup;/*!< @brief the free surround channel setup */
  bool                        m_LFEPresent;         /*!< @brief if true LFE signal generation is done inside decoder */

  void SetParams();
  void Deinitialize();

public:
  CDSPProcess_FreeSurround(unsigned int id);
  virtual ~CDSPProcess_FreeSurround();

  AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *settings, const AE_DSP_STREAM_PROPERTIES *pProperties);
  AE_DSP_ERROR StreamDestroy();
  AE_DSP_ERROR StreamInitialize(const AE_DSP_SETTINGS *settings);
  AE_DSP_ERROR StreamIsModeSupported(AE_DSP_MODE_TYPE type, unsigned int mode_id, int unique_db_mode_id);

  unsigned int StreamProcess(float **array_in, float **array_out, unsigned int samples);
  int          StreamGetOutChannels(unsigned long &out_channel_present_flags);
  float        StreamGetDelay();

  void SetCircularWrap(float degree);
  void SetShift(float value);
  void SetDepth(float depth);
  void SetFocus(float focus);
  void SetCenterImage(float value);
  void SetFrontSeparation(float separation);
  void SetRearSeparation(float separation);
  void SetBassRedirection(bool onOff);
  void SetLowCutoff(float value);
  void SetHighCutoff(float value);

  void ResetSettings();
};
