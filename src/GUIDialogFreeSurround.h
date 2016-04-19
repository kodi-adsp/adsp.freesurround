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

#include "addon.h"
#include "FreeSurroundSettings.h"

#include <kodi/api2/gui/Window.hpp>
#include <kodi/api2/gui/ControlSettingsSlider.hpp>

class CGUIDialogFreeSurround : private CDSPSettings, public KodiAPI::GUI::CWindow
{
public:
  CGUIDialogFreeSurround(unsigned int streamId);

private:
  virtual bool OnClick(int controlId) override;
  virtual bool OnFocus(int controlId) override;
  virtual bool OnInit() override;
  virtual bool OnAction(int actionId) override;

  const unsigned int m_StreamId;

  KodiAPI::GUI::CControlSettingsSlider  *m_CircularWrap;
  KodiAPI::GUI::CControlSettingsSlider  *m_Shift;
  KodiAPI::GUI::CControlSettingsSlider  *m_Depth;
  KodiAPI::GUI::CControlSettingsSlider  *m_Focus;
  KodiAPI::GUI::CControlSettingsSlider  *m_CenterImage;
  KodiAPI::GUI::CControlSettingsSlider  *m_FrontSeparation;
  KodiAPI::GUI::CControlSettingsSlider  *m_RearSeparation;
};
