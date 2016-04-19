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

#include "kodi/util/XMLUtils.h"
#include "p8-platform/util/util.h"

#include "GUIDialogFreeSurround.h"
#include "DSPProcessFreeSurround.h"

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

CGUIDialogFreeSurround::CGUIDialogFreeSurround(unsigned int streamId)
  : KodiAPI::GUI::CWindow("DialogMasterModeFreeSurround.xml", "Confluence", false, true),
    m_StreamId(streamId),
    m_CircularWrap(nullptr),
    m_Shift(nullptr),
    m_Depth(nullptr),
    m_Focus(nullptr),
    m_CenterImage(nullptr),
    m_FrontSeparation(nullptr),
    m_RearSeparation(nullptr)
{
}

bool CGUIDialogFreeSurround::OnInit()
{
  LoadSettingsData();

  m_CircularWrap = new KodiAPI::GUI::CControlSettingsSlider(this, DSP_SETTING_FREESURROUND_CIRCULAR_WRAP);
  m_CircularWrap->SetIntRange(0, 360);
  m_CircularWrap->SetIntInterval(5);
  m_CircularWrap->SetIntValue((int)m_Settings.fCircularWrap);

  m_Shift = new KodiAPI::GUI::CControlSettingsSlider(this, DSP_SETTING_FREESURROUND_SHIFT);
  m_Shift->SetFloatRange(-1.0, +1.0);
  m_Shift->SetFloatValue(m_Settings.fShift);

  m_Depth = new KodiAPI::GUI::CControlSettingsSlider(this, DSP_SETTING_FREESURROUND_DEPTH);
  m_Depth->SetFloatRange(0, 4.0);
  m_Depth->SetFloatInterval(0.25);
  m_Depth->SetFloatValue(m_Settings.fDepth);

  m_Focus = new KodiAPI::GUI::CControlSettingsSlider(this, DSP_SETTING_FREESURROUND_FOCUS);
  m_Focus->SetFloatRange(-1.0, +1.0);
  m_Focus->SetFloatValue(m_Settings.fFocus);

  m_CenterImage = new KodiAPI::GUI::CControlSettingsSlider(this, DSP_SETTING_FREESURROUND_CENTER_IMAGE);
  m_CenterImage->SetFloatValue(m_Settings.fCenterImage);

  m_FrontSeparation = new KodiAPI::GUI::CControlSettingsSlider(this, DSP_SETTING_FREESURROUND_FRONT_SEPARATION);
  m_FrontSeparation->SetFloatValue(m_Settings.fFrontSeparation);

  m_RearSeparation = new KodiAPI::GUI::CControlSettingsSlider(this, DSP_SETTING_FREESURROUND_REAR_SEPARATION);
  m_RearSeparation->SetFloatValue(m_Settings.fRearSeparation);

  return true;
}

bool CGUIDialogFreeSurround::OnClick(int controlId)
{
  CDSPProcess_FreeSurround *process = nullptr;

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
      delete m_CircularWrap;
      delete m_Shift;
      delete m_Depth;
      delete m_Focus;
      delete m_CenterImage;
      delete m_FrontSeparation;
      delete m_RearSeparation;
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

      m_CircularWrap->SetIntValue((int)m_Settings.fCircularWrap);
      m_Shift->SetFloatValue(m_Settings.fShift);
      m_Depth->SetFloatValue(m_Settings.fDepth);
      m_Focus->SetFloatValue(m_Settings.fFocus);
      m_CenterImage->SetFloatValue(m_Settings.fCenterImage);
      m_FrontSeparation->SetFloatValue(m_Settings.fFrontSeparation);
      m_RearSeparation->SetFloatValue(m_Settings.fRearSeparation);

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
      }
      break;
    }
    default:
      break;
  }

  return true;
}

bool CGUIDialogFreeSurround::OnFocus(int controlId)
{
  return true;
}

bool CGUIDialogFreeSurround::OnAction(int actionId)
{
  if (actionId == ADDON_ACTION_PREVIOUS_MENU ||
      actionId == ADDON_ACTION_NAV_BACK)
    return OnClick(BUTTON_CANCEL);
  else
    return false;
}
