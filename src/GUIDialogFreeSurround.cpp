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

#include "util/XMLUtils.h"
#include "p8-platform/util/util.h"

#include "GUIDialogFreeSurround.h"
#include "DSPProcessFreeSurround.h"

#include <kodi/ActionIDs.h>

#define BUTTON_OK                                   1
#define BUTTON_CANCEL                               2
#define BUTTON_DEFAULT                              5

#define DSP_SETTING_FREESURROUND_CIRCULAR_WRAP      41
#define DSP_SETTING_FREESURROUND_SHIFT              42
#define DSP_SETTING_FREESURROUND_DEPTH              43
#define DSP_SETTING_FREESURROUND_FOCUS              44
#define DSP_SETTING_FREESURROUND_FRONT_SEPARATION   45
#define DSP_SETTING_FREESURROUND_REAR_SEPARATION    46
#define DSP_SETTING_FREESURROUND_CENTER_IMAGE       47
#define DSP_SETTING_FREESURROUND_LFE                48
#define DSP_SETTING_FREESURROUND_LFE_LOW_CUTOFF     49
#define DSP_SETTING_FREESURROUND_LFE_HIGH_CUTOFF    50

CGUIDialogFreeSurround::CGUIDialogFreeSurround(unsigned int streamId)
  : CWindow("DialogMasterModeFreeSurround.xml", "skin.estuary", true),
    m_StreamId(streamId),
    m_CircularWrap(nullptr),
    m_Shift(nullptr),
    m_Depth(nullptr),
    m_Focus(nullptr),
    m_CenterImage(nullptr),
    m_FrontSeparation(nullptr),
    m_RearSeparation(nullptr),
    m_LFE(nullptr),
    m_LFE_LowCutoff(nullptr),
    m_LFE_HighCutoff(nullptr)
{
}

CGUIDialogFreeSurround::~CGUIDialogFreeSurround()
{
  delete m_CircularWrap;
  delete m_Shift;
  delete m_Depth;
  delete m_Focus;
  delete m_CenterImage;
  delete m_FrontSeparation;
  delete m_RearSeparation;
  delete m_LFE;
  delete m_LFE_LowCutoff;
  delete m_LFE_HighCutoff;
}

bool CGUIDialogFreeSurround::OnInit()
{
  LoadSettingsData();

  m_CircularWrap = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_CIRCULAR_WRAP);
  m_CircularWrap->SetIntRange(0, 360);
  m_CircularWrap->SetIntInterval(5);
  m_CircularWrap->SetIntValue((int)m_Settings.fCircularWrap);

  m_Shift = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_SHIFT);
  m_Shift->SetFloatRange(-1.0, +1.0);
  m_Shift->SetFloatValue(m_Settings.fShift);

  m_Depth = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_DEPTH);
  m_Depth->SetFloatRange(0, 4.0);
  m_Depth->SetFloatInterval(0.25);
  m_Depth->SetFloatValue(m_Settings.fDepth);

  m_Focus = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_FOCUS);
  m_Focus->SetFloatRange(-1.0, +1.0);
  m_Focus->SetFloatValue(m_Settings.fFocus);

  m_CenterImage = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_CENTER_IMAGE);
  m_CenterImage->SetFloatValue(m_Settings.fCenterImage);

  m_FrontSeparation = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_FRONT_SEPARATION);
  m_FrontSeparation->SetFloatValue(m_Settings.fFrontSeparation);

  m_RearSeparation = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_REAR_SEPARATION);
  m_RearSeparation->SetFloatValue(m_Settings.fRearSeparation);

  m_LFE = new kodi::gui::controls::CRadioButton(this, DSP_SETTING_FREESURROUND_LFE);
  m_LFE->SetSelected(m_Settings.bLFE);

  m_LFE_LowCutoff = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_LFE_LOW_CUTOFF);
  m_LFE_LowCutoff->SetIntRange(40.0f, 400.0f);
  m_LFE_LowCutoff->SetIntInterval(10.0f);
  m_LFE_LowCutoff->SetIntValue(m_Settings.fLowCutoff);

  m_LFE_HighCutoff = new kodi::gui::controls::CSettingsSlider(this, DSP_SETTING_FREESURROUND_LFE_HIGH_CUTOFF);
  m_LFE_HighCutoff->SetIntRange(60.0f, 1000.0f);
  m_LFE_HighCutoff->SetIntInterval(10.0f);
  m_LFE_HighCutoff->SetIntValue(m_Settings.fHighCutoff);

  return true;
}

bool CGUIDialogFreeSurround::OnClick(int controlId)
{
  CDSPProcess_FreeSurround *process = NULL;

  switch (controlId)
  {
    case DSP_SETTING_FREESURROUND_CIRCULAR_WRAP:
    {
      m_Settings.fCircularWrap = m_CircularWrap->GetFloatValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetCircularWrap(m_Settings.fCircularWrap);
      break;
    }
    case DSP_SETTING_FREESURROUND_SHIFT:
    {
      m_Settings.fShift = m_Shift->GetFloatValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetShift(m_Settings.fShift);
      break;
    }
    case DSP_SETTING_FREESURROUND_DEPTH:
    {
      m_Settings.fDepth = m_Depth->GetFloatValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetDepth(m_Settings.fDepth);
      break;
    }
    case DSP_SETTING_FREESURROUND_FOCUS:
    {
      m_Settings.fFocus = m_Focus->GetFloatValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetFocus(m_Settings.fFocus);
      break;
    }
    case DSP_SETTING_FREESURROUND_CENTER_IMAGE:
    {
      m_Settings.fCenterImage = m_CenterImage->GetFloatValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetCenterImage(m_Settings.fCenterImage);
      break;
    }
    case DSP_SETTING_FREESURROUND_FRONT_SEPARATION:
    {
      m_Settings.fFrontSeparation = m_FrontSeparation->GetFloatValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetFrontSeparation(m_Settings.fFrontSeparation);
      break;
    }
    case DSP_SETTING_FREESURROUND_REAR_SEPARATION:
    {
      m_Settings.fRearSeparation = m_RearSeparation->GetFloatValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetRearSeparation(m_Settings.fRearSeparation);
      break;
    }
    case DSP_SETTING_FREESURROUND_LFE:
    {
      m_Settings.bLFE = m_LFE->IsSelected();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetBassRedirection(m_Settings.bLFE);
      break;
    }
    case DSP_SETTING_FREESURROUND_LFE_LOW_CUTOFF:
    {
      m_Settings.fLowCutoff = m_LFE_LowCutoff->GetIntValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetLowCutoff(m_Settings.fLowCutoff);
      break;
    }
    case DSP_SETTING_FREESURROUND_LFE_HIGH_CUTOFF:
    {
      m_Settings.fHighCutoff = m_LFE_HighCutoff->GetIntValue();
      process = g_usedDSPs[m_StreamId];
      if (process)
        process->SetHighCutoff(m_Settings.fHighCutoff);
      break;
    }
    case BUTTON_OK:
      SaveSettingsData();
    case BUTTON_CANCEL:
    {
      if (controlId == BUTTON_CANCEL)
      {
        process = g_usedDSPs[m_StreamId];
        if (process)
          process->ResetSettings();
      }
      Close();
      break;
    }
    case BUTTON_DEFAULT:
    {
      CDSPSettings defaultSettings;
      m_Settings.fCircularWrap           = defaultSettings.m_Settings.fCircularWrap;
      m_Settings.fShift                  = defaultSettings.m_Settings.fShift;
      m_Settings.fDepth                  = defaultSettings.m_Settings.fDepth;
      m_Settings.fFocus                  = defaultSettings.m_Settings.fFocus;
      m_Settings.fCenterImage            = defaultSettings.m_Settings.fCenterImage;
      m_Settings.fFrontSeparation        = defaultSettings.m_Settings.fFrontSeparation;
      m_Settings.fRearSeparation         = defaultSettings.m_Settings.fRearSeparation;
      m_Settings.bLFE                    = defaultSettings.m_Settings.bLFE;
      m_Settings.fLowCutoff              = defaultSettings.m_Settings.fLowCutoff;
      m_Settings.fHighCutoff             = defaultSettings.m_Settings.fHighCutoff;

      m_CircularWrap->SetIntValue((int)m_Settings.fCircularWrap);
      m_Shift->SetFloatValue(m_Settings.fShift);
      m_Depth->SetFloatValue(m_Settings.fDepth);
      m_Focus->SetFloatValue(m_Settings.fFocus);
      m_CenterImage->SetFloatValue(m_Settings.fCenterImage);
      m_FrontSeparation->SetFloatValue(m_Settings.fFrontSeparation);
      m_RearSeparation->SetFloatValue(m_Settings.fRearSeparation);
      m_LFE->SetSelected(m_Settings.bLFE);
      m_LFE_LowCutoff->SetIntValue(m_Settings.fLowCutoff);
      m_LFE_HighCutoff->SetIntValue(m_Settings.fHighCutoff);

      process = g_usedDSPs[m_StreamId];
      if (process)
      {
        process->SetCircularWrap(m_Settings.fCircularWrap);
        process->SetShift(m_Settings.fShift);
        process->SetDepth(m_Settings.fDepth);
        process->SetFocus(m_Settings.fFocus);
        process->SetCenterImage(m_Settings.fCenterImage);
        process->SetFrontSeparation(m_Settings.fFrontSeparation);
        process->SetRearSeparation(m_Settings.fRearSeparation);
        process->SetBassRedirection(m_Settings.bLFE);
        process->SetLowCutoff(m_Settings.fLowCutoff);
        process->SetHighCutoff(m_Settings.fHighCutoff);
      }
      break;
    }
    default:
      break;
  }

  return true;
}

bool CGUIDialogFreeSurround::OnAction(int actionId)
{
  if (actionId == ACTION_PREVIOUS_MENU ||
      actionId == ACTION_NAV_BACK)
    return OnClick(BUTTON_CANCEL);
  else
    return false;
}
