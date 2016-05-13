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

#include "libXBMC_addon.h"
#include "libKODI_adsp.h"
#include "libKODI_guilib.h"

class CDSPProcess_FreeSurround;

extern std::string                   g_strUserPath;
extern std::string                   g_strAddonPath;
extern CDSPProcess_FreeSurround     *g_usedDSPs[AE_DSP_STREAM_MAX_STREAMS];
extern ADDON::CHelper_libXBMC_addon *KODI;
extern CHelper_libKODI_guilib       *GUI;
extern CHelper_libKODI_adsp         *ADSP;
