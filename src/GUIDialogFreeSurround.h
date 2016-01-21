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

class CGUIDialogFreeSurround : private CDSPSettings
{
public:
  CGUIDialogFreeSurround(unsigned int streamId);
  virtual ~CGUIDialogFreeSurround();

  bool Show();
  void Close();
  void DoModal();

private:
  bool OnClick(int controlId);
  bool OnFocus(int controlId);
  bool OnInit();
  bool OnAction(int actionId);

  static bool OnClickCB(GUIHANDLE cbhdl, int controlId);
  static bool OnFocusCB(GUIHANDLE cbhdl, int controlId);
  static bool OnInitCB(GUIHANDLE cbhdl);
  static bool OnActionCB(GUIHANDLE cbhdl, int actionId);

  const unsigned int               m_StreamId;

  CAddonGUISettingsSliderControl  *m_CircularWrap;
  CAddonGUISettingsSliderControl  *m_Shift;
  CAddonGUISettingsSliderControl  *m_Depth;
  CAddonGUISettingsSliderControl  *m_Focus;
  CAddonGUISettingsSliderControl  *m_CenterImage;
  CAddonGUISettingsSliderControl  *m_FrontSeparation;
  CAddonGUISettingsSliderControl  *m_RearSeparation;

  CAddonGUIWindow                 *m_window;
};
