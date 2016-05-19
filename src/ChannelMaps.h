#pragma once
/*
 *      Copyright (C) 2014-2015 Team KODI
 *      Copyright (C) 2007-2010 Christian Kothe
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

/*
 * passed upon fsurround available under
 * http://www.hydrogenaud.io/forums/index.php?showtopic=52235
 * written by pro_optimizer
 */

#include <vector>
#include <map>

#include "kodi_adsp_types.h"

/*
 * Identifier translation from FreeSurround to KODI ADSP
 *
 * ci_front_left          = AE_DSP_PRSNT_CH_FL
 * ci_front_center_left   = AE_DSP_PRSNT_CH_FLOC
 * ci_front_center        = AE_DSP_PRSNT_CH_FC
 * ci_front_center_right  = AE_DSP_PRSNT_CH_FROC
 * ci_front_right         = AE_DSP_PRSNT_CH_FR
 * ci_side_front_left     = AE_DSP_PRSNT_CH_TFL
 * ci_side_front_right    = AE_DSP_PRSNT_CH_TFR
 * ci_side_center_left    = AE_DSP_PRSNT_CH_SL
 * ci_side_center_right   = AE_DSP_PRSNT_CH_SR
 * ci_side_back_left      = AE_DSP_PRSNT_CH_TBL
 * ci_side_back_right     = AE_DSP_PRSNT_CH_TBR
 * ci_back_left           = AE_DSP_PRSNT_CH_BL
 * ci_back_center_left    = AE_DSP_PRSNT_CH_BLOC
 * ci_back_center         = AE_DSP_PRSNT_CH_BC
 * ci_back_center_right   = AE_DSP_PRSNT_CH_BROC
 * ci_back_right          = AE_DSP_PRSNT_CH_BR
 * ci_lfe                 = AE_DSP_PRSNT_CH_LFE
 */

const int grid_res = 21; // resolution of the lookup grid

// channel allocation maps (per setup)
typedef std::vector<std::vector<float*> > alloc_lut;
extern std::map<unsigned,alloc_lut> chn_alloc;
// channel metadata maps (per setup)
extern std::map<unsigned,std::vector<float> > chn_angle;
extern std::map<unsigned,std::vector<float> > chn_xsf;
extern std::map<unsigned,std::vector<float> > chn_ysf;
extern std::map<unsigned,std::vector<AE_DSP_CHANNEL_PRESENT> > chn_id;
extern bool init_maps();
