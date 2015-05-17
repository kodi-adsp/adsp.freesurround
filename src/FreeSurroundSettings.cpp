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

#include "kodi/libXBMC_addon.h"
#include "kodi/libKODI_adsp.h"
#include "kodi/libKODI_guilib.h"

#include "kodi/util/XMLUtils.h"
#include "platform/util/util.h"
#include "platform/util/StdString.h"

#include "FreeSurroundSettings.h"

using namespace std;
using namespace ADDON;

std::string GetSettingsFile()
{
  string settingFile = g_strAddonPath;
  if (settingFile.at(settingFile.size() - 1) == '\\' ||
      settingFile.at(settingFile.size() - 1) == '/')
    settingFile.append("ADSPFreeSurrAddonSettings.xml");
  else
#ifdef TARGET_WINDOWS
    settingFile.append("\\ADSPFreeSurrAddonSettings.xml");
#else
    settingFile.append("/ADSPFreeSurrAddonSettings.xml");
#endif
  return settingFile;
}

CDSPSettings::CDSPSettings()
{
  m_Settings.fInputGain = 1.0f;
  m_Settings.fCircularWrap = 90.0f;
  m_Settings.fShift = 0.0f;
  m_Settings.fDepth = 1.0f;
  m_Settings.fCenterImage = 1.0f;
  m_Settings.fFocus = 0.0f;
  m_Settings.fFrontSeparation = 1.0;
  m_Settings.fRearSeparation = 1.0;
  m_Settings.bLFE = false;
  m_Settings.fLowCutoff = 40.0;
  m_Settings.fHighCutoff = 90.0;
}

bool CDSPSettings::LoadSettingsData(bool initial)
{
  TiXmlDocument xmlDoc;
  string strSettingsFile = GetSettingsFile();

  if (!xmlDoc.LoadFile(strSettingsFile))
  {
    if (initial)
    {
      if (!SaveSettingsData())
      {
        KODI->Log(LOG_ERROR, "failed to create initial settings data file at '%s')", strSettingsFile.c_str());
        return false;
      }
      return true;
    }
    else
      KODI->Log(LOG_ERROR, "invalid settings data (no/invalid data file found at '%s')", strSettingsFile.c_str());
    return false;
  }

  TiXmlElement *pRootElement = xmlDoc.RootElement();
  if (strcmp(pRootElement->Value(), "freesurround") != 0)
  {
    if (!initial)
      KODI->Log(LOG_ERROR, "invalid settings data (no <circlesurround2> tag found)");
    return false;
  }

  TiXmlElement *pElement = pRootElement->FirstChildElement("settings");
  if (pElement)
  {
    if (!XMLUtils::GetFloat(pElement, "inputgain", m_Settings.fInputGain))
      m_Settings.fInputGain = 0.70794576;
    if (!XMLUtils::GetFloat(pElement, "circularwrap", m_Settings.fCircularWrap))
      m_Settings.fCircularWrap = 90;
    if (!XMLUtils::GetFloat(pElement, "shift", m_Settings.fShift))
      m_Settings.fShift = 0.0f;
    if (!XMLUtils::GetFloat(pElement, "depth", m_Settings.fDepth))
      m_Settings.fDepth = 1.0f;
    if (!XMLUtils::GetFloat(pElement, "centerimage", m_Settings.fCenterImage))
      m_Settings.fCenterImage = 1.0f;
    if (!XMLUtils::GetFloat(pElement, "focus", m_Settings.fFocus))
      m_Settings.fFocus = 0.0f;
    if (!XMLUtils::GetFloat(pElement, "frontseparation", m_Settings.fFrontSeparation))
      m_Settings.fFrontSeparation = 1.0;
    if (!XMLUtils::GetFloat(pElement, "rearseparation", m_Settings.fRearSeparation))
      m_Settings.fRearSeparation = 1.0;
    if (!XMLUtils::GetBoolean(pElement, "bassredirection", m_Settings.bLFE))
      m_Settings.bLFE = false;
    if (!XMLUtils::GetFloat(pElement, "lowcutoff", m_Settings.fLowCutoff))
      m_Settings.fLowCutoff = 40.0;
    if (!XMLUtils::GetFloat(pElement, "highcutoff", m_Settings.fHighCutoff))
      m_Settings.fHighCutoff = 90.0;
  }

  return true;
}

bool CDSPSettings::SaveSettingsData()
{
  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("freesurround");
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == NULL)
    return false;

  TiXmlElement xmlCS2Setting("settings");
  TiXmlNode* pNode = pRoot->InsertEndChild(xmlCS2Setting);
  if (pNode)
  {
    XMLUtils::SetFloat(pNode, "inputgain", m_Settings.fInputGain);
    XMLUtils::SetFloat(pNode, "circularwrap", m_Settings.fCircularWrap);
    XMLUtils::SetFloat(pNode, "shift", m_Settings.fShift);
    XMLUtils::SetFloat(pNode, "depth", m_Settings.fDepth);
    XMLUtils::SetFloat(pNode, "centerimage", m_Settings.fCenterImage);
    XMLUtils::SetFloat(pNode, "focus", m_Settings.fFocus);
    XMLUtils::SetFloat(pNode, "frontseparation", m_Settings.fFrontSeparation);
    XMLUtils::SetFloat(pNode, "rearseparation", m_Settings.fRearSeparation);
    XMLUtils::SetBoolean(pNode, "bassredirection", m_Settings.bLFE);
    XMLUtils::SetFloat(pNode, "lowcutoff", m_Settings.fLowCutoff);
    XMLUtils::SetFloat(pNode, "highcutoff", m_Settings.fHighCutoff);
  }

  if (!xmlDoc.SaveFile(GetSettingsFile()))
  {
    KODI->Log(LOG_ERROR, "failed to write circle surround 2 settings data");
    return false;
  }

  return true;
}
