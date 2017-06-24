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

#include <vector>
#include <string>
#include "addon.h"
#include <kodi/Filesystem.h>
#include "p8-platform/util/util.h"
#include "p8-platform/util/StdString.h"
#include "GUIDialogFreeSurround.h"
#include "DSPProcessFreeSurround.h"

using namespace std;

#define ID_MASTER_PROCESS_FREE_SURROUND   1300

CDSPProcess_FreeSurround *g_usedDSPs[AE_DSP_STREAM_MAX_STREAMS];

#if defined(TARGET_WINDOWS)
#undef CreateDirectory
#endif

class CFreeSurroundAddon
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceAudioDSP
{
public:
  CFreeSurroundAddon();
  virtual ~CFreeSurroundAddon();

  virtual void GetCapabilities(AE_DSP_ADDON_CAPABILITIES& capabilities) override;
  virtual std::string GetDSPName() override { return "Free Surround Processor"; }
  virtual std::string GetDSPVersion() override { return FREESURROUND_VERSION; };
  virtual AE_DSP_ERROR MenuHook(const AE_DSP_MENUHOOK& menuhook, const AE_DSP_MENUHOOK_DATA& item) override;
  virtual AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS& addonSettings, const AE_DSP_STREAM_PROPERTIES& properties, ADDON_HANDLE handle) override;
  virtual AE_DSP_ERROR StreamDestroy(const ADDON_HANDLE handle) override;
  virtual AE_DSP_ERROR StreamInitialize(const ADDON_HANDLE handle, const AE_DSP_SETTINGS& addonSettings) override;
  virtual AE_DSP_ERROR StreamIsModeSupported(const ADDON_HANDLE handle, AE_DSP_MODE_TYPE type, unsigned int mode_id, int unique_db_mode_id) override;
  virtual float MasterProcessGetDelay(const ADDON_HANDLE handle) override;
  virtual unsigned int MasterProcess(const ADDON_HANDLE handle, const float** array_in, float** array_out, unsigned int samples) override;
  virtual int MasterProcessGetOutChannels(const ADDON_HANDLE handle, unsigned long& out_channel_present_flags) override;

private:
  AE_DSP_MENUHOOK m_MenuHook;
  struct AE_DSP_MODES::AE_DSP_MODE m_ModeInfoStruct;
};

CFreeSurroundAddon::CFreeSurroundAddon()
{
  // create addon user path
  if (!kodi::vfs::DirectoryExists(kodi::GetBaseUserPath()))
  {
    kodi::vfs::CreateDirectory(kodi::GetBaseUserPath());
  }

  for (int i = 0; i < AE_DSP_STREAM_MAX_STREAMS; ++i)
    g_usedDSPs[i] = nullptr;

  CDSPSettings settings;
  settings.LoadSettingsData(true);

  std::string imagePath;

  m_ModeInfoStruct.iUniqueDBModeId        = -1;         // set by RegisterMode
  m_ModeInfoStruct.iModeType              = AE_DSP_MODE_TYPE_MASTER_PROCESS;
  m_ModeInfoStruct.iModeNumber            = ID_MASTER_PROCESS_FREE_SURROUND;
  m_ModeInfoStruct.bHasSettingsDialog     = true;
  m_ModeInfoStruct.iModeDescription       = 30002;
  m_ModeInfoStruct.iModeHelp              = 30003;
  m_ModeInfoStruct.iModeName              = 30000;
  m_ModeInfoStruct.iModeSetupName         = 30001;
  m_ModeInfoStruct.iModeSupportTypeFlags  = AE_DSP_PRSNT_ASTREAM_BASIC | AE_DSP_PRSNT_ASTREAM_MUSIC | AE_DSP_PRSNT_ASTREAM_MOVIE;
  m_ModeInfoStruct.bIsDisabled            = false;

  strncpy(m_ModeInfoStruct.strModeName, "Free Surround", sizeof(m_ModeInfoStruct.strModeName) - 1);
  imagePath = kodi::GetAddonPath();
  imagePath += "/resources/skins/Confluence/media/adsp-freesurround.png";
  strncpy(m_ModeInfoStruct.strOwnModeImage, imagePath.c_str(), sizeof(m_ModeInfoStruct.strOwnModeImage) - 1);
  memset(m_ModeInfoStruct.strOverrideModeImage, 0, sizeof(m_ModeInfoStruct.strOwnModeImage)); // unused
  RegisterMode(&m_ModeInfoStruct);

  m_MenuHook.iHookId                      = ID_MASTER_PROCESS_FREE_SURROUND;
  m_MenuHook.category                     = AE_DSP_MENUHOOK_MASTER_PROCESS;
  m_MenuHook.iLocalizedStringId           = 30001;
  m_MenuHook.iRelevantModeId              = ID_MASTER_PROCESS_FREE_SURROUND;
  m_MenuHook.bNeedPlayback                = true;
  AddMenuHook(&m_MenuHook);
}

CFreeSurroundAddon::~CFreeSurroundAddon()
{
  for (int i = 0; i < AE_DSP_STREAM_MAX_STREAMS; ++i)
    SAFE_DELETE(g_usedDSPs[i]);
}

void CFreeSurroundAddon::GetCapabilities(AE_DSP_ADDON_CAPABILITIES& capabilities)
{
  capabilities.bSupportsInputProcess    = false;
  capabilities.bSupportsPreProcess      = false;
  capabilities.bSupportsInputResample   = false;
  capabilities.bSupportsMasterProcess   = true;
  capabilities.bSupportsPostProcess     = false;
  capabilities.bSupportsOutputResample  = false;
}

AE_DSP_ERROR CFreeSurroundAddon::MenuHook(const AE_DSP_MENUHOOK &menuhook, const AE_DSP_MENUHOOK_DATA &item)
{
  if (menuhook.iHookId == ID_MASTER_PROCESS_FREE_SURROUND)
  {
    CGUIDialogFreeSurround settings(item.data.iStreamId);
    settings.DoModal();
    return AE_DSP_ERROR_NO_ERROR;
  }
  return AE_DSP_ERROR_UNKNOWN;
}

AE_DSP_ERROR CFreeSurroundAddon::StreamCreate(const AE_DSP_SETTINGS& addonSettings, const AE_DSP_STREAM_PROPERTIES& pProperties, ADDON_HANDLE handle)
{
  CDSPProcess_FreeSurround *proc = new CDSPProcess_FreeSurround(addonSettings.iStreamID);
  AE_DSP_ERROR err = proc->StreamCreate(&addonSettings, &pProperties);
  if (err == AE_DSP_ERROR_NO_ERROR)
  {
    handle->dataIdentifier             = addonSettings.iStreamID;
    handle->callerAddress              = proc;
    g_usedDSPs[handle->dataIdentifier] = proc; //!< Still used in table to have identification on settings dialog
  }
  else
    delete proc;
  return err;
}

AE_DSP_ERROR CFreeSurroundAddon::StreamDestroy(const ADDON_HANDLE handle)
{
  AE_DSP_ERROR err = ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamDestroy();
  delete ((CDSPProcess_FreeSurround*)handle->callerAddress);
  g_usedDSPs[handle->dataIdentifier] = NULL;
  return err;
}

AE_DSP_ERROR CFreeSurroundAddon::StreamInitialize(const ADDON_HANDLE handle, const AE_DSP_SETTINGS& settings)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamInitialize(&settings);
}

AE_DSP_ERROR CFreeSurroundAddon::StreamIsModeSupported(const ADDON_HANDLE handle, AE_DSP_MODE_TYPE type, unsigned int mode_id, int unique_db_mode_id)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamIsModeSupported(type, mode_id, unique_db_mode_id);
}

float CFreeSurroundAddon::MasterProcessGetDelay(const ADDON_HANDLE handle)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamGetDelay();
}

unsigned int CFreeSurroundAddon::MasterProcess(const ADDON_HANDLE handle, const float **array_in, float **array_out, unsigned int samples)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamProcess(array_in, array_out, samples);
}

int CFreeSurroundAddon::MasterProcessGetOutChannels(const ADDON_HANDLE handle, unsigned long &out_channel_present_flags)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamGetOutChannels(out_channel_present_flags);
}

ADDONCREATOR(CFreeSurroundAddon);
