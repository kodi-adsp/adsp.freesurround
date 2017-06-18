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

#include "p8-platform/util/StdString.h"
#include "addon.h"
#include "FreeSurroundSettings.h"

#include <kodi/gui/Window.h>
#include <kodi/gui/controls/SettingsSlider.h>
#include <kodi/gui/controls/RadioButton.h>

class CGUIDialogFreeSurround : public kodi::gui::CWindow, private CDSPSettings
{
public:
  CGUIDialogFreeSurround(unsigned int streamId);
  virtual ~CGUIDialogFreeSurround();

  virtual bool OnInit() override;
  virtual bool OnClick(int controlId) override;
  virtual bool OnAction(int actionId) override;

private:
  const unsigned int m_StreamId;

  kodi::gui::controls::CSettingsSlider *m_CircularWrap;
  kodi::gui::controls::CSettingsSlider *m_Shift;
  kodi::gui::controls::CSettingsSlider *m_Depth;
  kodi::gui::controls::CSettingsSlider *m_Focus;
  kodi::gui::controls::CSettingsSlider *m_CenterImage;
  kodi::gui::controls::CSettingsSlider *m_FrontSeparation;
  kodi::gui::controls::CSettingsSlider *m_RearSeparation;
  kodi::gui::controls::CRadioButton *m_LFE;
  kodi::gui::controls::CSettingsSlider *m_LFE_LowCutoff;
  kodi::gui::controls::CSettingsSlider *m_LFE_HighCutoff;
};
