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
#include "kodi_adsp_dll.h"
#include "p8-platform/util/util.h"
#include "p8-platform/util/StdString.h"
#include "GUIDialogFreeSurround.h"
#include "DSPProcessFreeSurround.h"

using namespace std;
using namespace ADDON;

#ifdef TARGET_WINDOWS
#define snprintf _snprintf
#endif

#if defined(TARGET_WINDOWS)
  #undef CreateDirectory
#endif

#define ID_MASTER_PROCESS_FREE_SURROUND   1300


/* User adjustable settings are saved here.
 * Default values are defined inside addon.h
 * and exported to the other source files.
 */
std::string               g_strUserPath   = "";
std::string               g_strAddonPath  = "";
CHelper_libXBMC_addon    *KODI            = NULL;
CHelper_libKODI_adsp     *ADSP            = NULL;
CHelper_libKODI_guilib   *GUI             = NULL;
ADDON_STATUS              m_CurStatus     = ADDON_STATUS_UNKNOWN;
AE_DSP_MENUHOOK           m_MenuHook;
CDSPProcess_FreeSurround *g_usedDSPs[AE_DSP_STREAM_MAX_STREAMS];
struct AE_DSP_MODES::AE_DSP_MODE m_ModeInfoStruct;

extern "C" {

void ADDON_ReadSettings(void)
{
}

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!hdl || !props)
    return ADDON_STATUS_UNKNOWN;

  AE_DSP_PROPERTIES* adspprops = (AE_DSP_PROPERTIES*)props;

  KODI = new CHelper_libXBMC_addon;
  if (!KODI->RegisterMe(hdl))
  {
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  GUI = new CHelper_libKODI_guilib;
  if (!GUI->RegisterMe(hdl))
  {
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  ADSP = new CHelper_libKODI_adsp;
  if (!ADSP->RegisterMe(hdl))
  {
    SAFE_DELETE(ADSP);
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  KODI->Log(LOG_DEBUG, "%s - Creating the Audio DSP free surround add-on", __FUNCTION__);

  m_CurStatus     = ADDON_STATUS_UNKNOWN;
  g_strUserPath   = adspprops->strUserPath;
  g_strAddonPath  = adspprops->strAddonPath;

  // create addon user path
  if (!KODI->DirectoryExists(g_strUserPath.c_str()))
  {
    KODI->CreateDirectory(g_strUserPath.c_str());
  }

  for (int i = 0; i < AE_DSP_STREAM_MAX_STREAMS; ++i)
    g_usedDSPs[i] = NULL;

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
  imagePath = g_strAddonPath;
  imagePath += "/resources/skins/Confluence/media/adsp-freesurround.png";
  strncpy(m_ModeInfoStruct.strOwnModeImage, imagePath.c_str(), sizeof(m_ModeInfoStruct.strOwnModeImage) - 1);
  memset(m_ModeInfoStruct.strOverrideModeImage, 0, sizeof(m_ModeInfoStruct.strOwnModeImage)); // unused
  ADSP->RegisterMode(&m_ModeInfoStruct);

  m_MenuHook.iHookId                      = ID_MASTER_PROCESS_FREE_SURROUND;
  m_MenuHook.category                     = AE_DSP_MENUHOOK_MASTER_PROCESS;
  m_MenuHook.iLocalizedStringId           = 30001;
  m_MenuHook.iRelevantModeId              = ID_MASTER_PROCESS_FREE_SURROUND;
  m_MenuHook.bNeedPlayback                = true;
  ADSP->AddMenuHook(&m_MenuHook);

  m_CurStatus = ADDON_STATUS_OK;

  return m_CurStatus;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
  for (int i = 0; i < AE_DSP_STREAM_MAX_STREAMS; ++i)
    SAFE_DELETE(g_usedDSPs[i]);

  SAFE_DELETE(ADSP);
  SAFE_DELETE(GUI);
  SAFE_DELETE(KODI);

  m_CurStatus = ADDON_STATUS_UNKNOWN;
}

bool ADDON_HasSettings()
{
  return true;
}

const char* GetAudioDSPAPIVersion(void)
{
  return KODI_AE_DSP_API_VERSION;
}

const char* GetMinimumAudioDSPAPIVersion(void)
{
  return KODI_AE_DSP_MIN_API_VERSION;
}

const char* GetGUIAPIVersion(void)
{
  return KODI_GUILIB_API_VERSION;
}

const char* GetMinimumGUIAPIVersion(void)
{
  return KODI_GUILIB_MIN_API_VERSION;
}

AE_DSP_ERROR GetAddonCapabilities(AE_DSP_ADDON_CAPABILITIES* pCapabilities)
{
  pCapabilities->bSupportsInputProcess    = false;
  pCapabilities->bSupportsPreProcess      = false;
  pCapabilities->bSupportsInputResample   = false;
  pCapabilities->bSupportsMasterProcess   = true;
  pCapabilities->bSupportsPostProcess     = false;
  pCapabilities->bSupportsOutputResample  = false;

  return AE_DSP_ERROR_NO_ERROR;
}

const char *GetDSPName(void)
{
  static const char *strBackendName = "Free Surround Processor";
  return strBackendName;
}

const char *GetDSPVersion(void)
{
  static const char *strDSPVersion = FREESURROUND_VERSION;
  return strDSPVersion;
}

AE_DSP_ERROR CallMenuHook(const AE_DSP_MENUHOOK &menuhook, const AE_DSP_MENUHOOK_DATA &item)
{
  if (menuhook.iHookId == ID_MASTER_PROCESS_FREE_SURROUND)
  {
    CGUIDialogFreeSurround settings(item.data.iStreamId);
    settings.DoModal();
    return AE_DSP_ERROR_NO_ERROR;
  }
  return AE_DSP_ERROR_UNKNOWN;
}

AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *addonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties, ADDON_HANDLE handle)
{
  CDSPProcess_FreeSurround *proc = new CDSPProcess_FreeSurround(addonSettings->iStreamID);
  AE_DSP_ERROR err = proc->StreamCreate(addonSettings, pProperties);
  if (err == AE_DSP_ERROR_NO_ERROR)
  {
    handle->dataIdentifier             = addonSettings->iStreamID;
    handle->callerAddress              = proc;
    g_usedDSPs[handle->dataIdentifier] = proc; //!< Still used in table to have identification on settings dialog
  }
  else
    delete proc;
  return err;
}

AE_DSP_ERROR StreamDestroy(const ADDON_HANDLE handle)
{
  AE_DSP_ERROR err = ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamDestroy();
  delete ((CDSPProcess_FreeSurround*)handle->callerAddress);
  g_usedDSPs[handle->dataIdentifier] = NULL;
  return err;
}

AE_DSP_ERROR StreamInitialize(const ADDON_HANDLE handle, const AE_DSP_SETTINGS *settings)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamInitialize(settings);
}

AE_DSP_ERROR StreamIsModeSupported(const ADDON_HANDLE handle, AE_DSP_MODE_TYPE type, unsigned int mode_id, int unique_db_mode_id)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamIsModeSupported(type, mode_id, unique_db_mode_id);
}

float MasterProcessGetDelay(const ADDON_HANDLE handle)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamGetDelay();
}

unsigned int MasterProcess(const ADDON_HANDLE handle, float **array_in, float **array_out, unsigned int samples)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamProcess(array_in, array_out, samples);
}

int MasterProcessGetOutChannels(const ADDON_HANDLE handle, unsigned long &out_channel_present_flags)
{
  return ((CDSPProcess_FreeSurround*)handle->callerAddress)->StreamGetOutChannels(out_channel_present_flags);
}

/*!
 * Unused DSP addon functions
 */
unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet) { return 0; }
ADDON_STATUS ADDON_SetSetting(const char *settingName, const void *settingValue) { return ADDON_STATUS_OK; }
void ADDON_Stop() {}
void ADDON_FreeSettings() {}
void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data) {}
bool InputProcess(const ADDON_HANDLE handle, const float **array_in, unsigned int samples) { return true; }
unsigned int InputResampleProcessNeededSamplesize(const ADDON_HANDLE handle) { return 0; }
int InputResampleSampleRate(const ADDON_HANDLE handle) { return 0; }
float InputResampleGetDelay(const ADDON_HANDLE handle) { return 0.0f; }
unsigned int InputResampleProcess(const ADDON_HANDLE handle, float **array_in, float **array_out, unsigned int samples) { return 0; }
unsigned int PreProcessNeededSamplesize(const ADDON_HANDLE handle, unsigned int mode_id) { return 0; }
float PreProcessGetDelay(const ADDON_HANDLE handle, unsigned int mode_id) { return 0.0f; }
unsigned int PreProcess(const ADDON_HANDLE handle, unsigned int mode_id, float **array_in, float **array_out, unsigned int samples) { return 0; }
AE_DSP_ERROR MasterProcessSetMode(const ADDON_HANDLE handle, AE_DSP_STREAMTYPE type, unsigned int client_mode_id, int unique_db_mode_id) { return AE_DSP_ERROR_NO_ERROR; }
unsigned int MasterProcessNeededSamplesize(const ADDON_HANDLE handle) { return 0; }
const char *MasterProcessGetStreamInfoString(const ADDON_HANDLE handle) { return ""; }
unsigned int PostProcessNeededSamplesize(const ADDON_HANDLE handle, unsigned int mode_id) { return 0; }
float PostProcessGetDelay(const ADDON_HANDLE handle, unsigned int mode_id) { return 0.0f; }
unsigned int PostProcess(const ADDON_HANDLE handle, unsigned int mode_id, float **array_in, float **array_out, unsigned int samples) { return 0; }
unsigned int OutputResampleProcessNeededSamplesize(const ADDON_HANDLE handle) { return 0; }
int OutputResampleSampleRate(const ADDON_HANDLE handle) { return 0; }
float OutputResampleGetDelay(const ADDON_HANDLE handle) { return 0.0f; }
unsigned int OutputResampleProcess(const ADDON_HANDLE handle, float **array_in, float **array_out, unsigned int samples) { return 0; }

}
