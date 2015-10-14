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

#include <cmath>
#include <vector>

#if defined(TARGET_WINDOWS)
  #include <algorithm>
#endif

#include "FreeSurroundDecoder.h"

const double pi      = 3.14159265358979323846;
const double epsilon = 0.000001;

#ifndef TARGET_WINDOWS
  #undef min
  #undef max
#endif // !TARGET_WINDOWS

// implementation of the shell class
CFreeSurroundDecoder::CFreeSurroundDecoder(channel_setup setup, unsigned blocksize, unsigned int samplerate)
  : m_ChannelSetup(setup),
    m_Channels((unsigned)chn_alloc[setup].size()),
    m_Samplerate(samplerate),
    m_BufferSize(blocksize),
    m_BufferSizeHalf(blocksize/2),
    m_FFTForward(kiss_fftr_alloc(blocksize,0,0,0)),
    m_FFTInverse(kiss_fftr_alloc(blocksize,1,0,0)),
    m_BufferEmpty(true)
{
  // allocate per-channel buffers
  m_InputBuffer[0] = (float*)calloc(2*m_BufferSize, sizeof(float));
  m_InputBuffer[1] = (float*)calloc(2*m_BufferSize, sizeof(float));

  memset(m_OutputBuffer, 0, sizeof(m_OutputBuffer));
  for (unsigned i = 0; i < m_Channels; ++i)
  {
    m_OutputBuffer[i] = (float*)calloc(2 * m_BufferSize, sizeof(float));
    m_Signal[i]       = (cplx*)calloc(m_BufferSize, sizeof(cplx));
  }

  // init the window function
  m_Window = (double*)malloc(m_BufferSize * sizeof(double));
  for (unsigned k=0; k < m_BufferSize; ++k)
    m_Window[k] = sqrt(0.5 * (1 - cos(2 * pi * k / m_BufferSize)) / m_BufferSize);

  m_Destination = (float*)calloc(m_BufferSize, sizeof(float));
  m_TotalLeft   = (float*)calloc(m_BufferSize, sizeof(float));
  m_TotalRight  = (float*)calloc(m_BufferSize, sizeof(float));
  m_FreqLeft    = (cplx*)calloc(m_BufferSize / 2 + 1, sizeof(cplx));
  m_FreqRight   = (cplx*)calloc(m_BufferSize / 2 + 1, sizeof(cplx));

  // set default parameters
  m_CircularWrap    = 90.0;
  m_Shift           = 0.0;
  m_Depth           = 1.0;
  m_Focus           = 0.0;
  m_CenterImage     = 1.0;
  m_FrontSeparation = 1.0;
  m_RearSeparation  = 1.0;
  m_LowCutoff       = 40.0 / m_Samplerate / 2.0 * m_BufferSizeHalf;
  m_HighCutoff      = 90.0 / m_Samplerate / 2.0 * m_BufferSizeHalf;
  m_UseLFE          = true;
}

CFreeSurroundDecoder::~CFreeSurroundDecoder()
{
  free(m_FFTForward);
  free(m_FFTInverse);

  free(m_InputBuffer[0]);
  free(m_InputBuffer[1]);
  free(m_Window);
  free(m_Destination);
  free(m_TotalLeft);
  free(m_TotalRight);
  free(m_FreqLeft);
  free(m_FreqRight);

  for (unsigned i = 0; i < m_Channels; ++i)
  {
    free(m_OutputBuffer[i]);
    free(m_Signal[i]);
  }
}

float **CFreeSurroundDecoder::getOutputBuffers()
{
  return m_OutputBuffer;
}

void CFreeSurroundDecoder::Decode(float **inputs)
{
  //! append incoming data to the end of the input buffer
  memcpy(&m_InputBuffer[0][m_BufferSizeHalf], &inputs[0][0], m_BufferSize*sizeof(float));
  memcpy(&m_InputBuffer[1][m_BufferSizeHalf], &inputs[1][0], m_BufferSize*sizeof(float));

  //! process first and second half, overlapped
  BufferedDecode(m_InputBuffer, 0);
  BufferedDecode(m_InputBuffer, m_BufferSizeHalf);

  //! shift last half of the input to the beginning (for overlapping with a future block)
  memcpy(&m_InputBuffer[0][0], &m_InputBuffer[0][m_BufferSize], m_BufferSizeHalf*sizeof(float));
  memcpy(&m_InputBuffer[1][0], &m_InputBuffer[1][m_BufferSize], m_BufferSizeHalf*sizeof(float));

  m_BufferEmpty = false;
}

void CFreeSurroundDecoder::Flush()
{
  for (unsigned i = 0; i < m_Channels; ++i)
    memset(m_OutputBuffer[i], 0, 2 * m_BufferSize*sizeof(float));

  m_BufferEmpty = true;
}

inline double CFreeSurroundDecoder::sqr(double x)
{
  return x * x;
}

inline double CFreeSurroundDecoder::amplitude(const cplx &x)
{
  return sqrt(sqr(x.real()) + sqr(x.imag()));
}

inline double CFreeSurroundDecoder::phase(const cplx &x)
{
  return atan2(x.imag(), x.real());
}

inline cplx CFreeSurroundDecoder::polar(double a, double p)
{
  return cplx(a * cos(p), a * sin(p));
}

inline double CFreeSurroundDecoder::Min(double a, double b)
{
  return a < b ? a : b;
}

inline double CFreeSurroundDecoder::Max(double a, double b)
{
  return a > b ? a : b;
}

inline double CFreeSurroundDecoder::clamp(double x)
{
  return Max(-1, Min(1, x));
}

inline double CFreeSurroundDecoder::sign(double x)
{
  return x < 0 ? -1 : (x > 0 ? 1 : 0);
}

inline double CFreeSurroundDecoder::EdgeDistance(double a)
{
  return Min(sqrt(1 + sqr(tan(a))), sqrt(1 + sqr(1 / tan(a))));
}

void CFreeSurroundDecoder::BufferedDecode(float *input[2], int ptr)
{
  //! demultiplex and apply window function
  for (unsigned k = 0; k < m_BufferSize; ++k)
  {
    m_TotalLeft[k]  = m_Window[k] * input[0][k+ptr];
    m_TotalRight[k] = m_Window[k] * input[1][k+ptr];
  }

  //! map into spectral domain
  kiss_fftr(m_FFTForward, m_TotalLeft,  (kiss_fft_cpx*)m_FreqLeft);
  kiss_fftr(m_FFTForward, m_TotalRight, (kiss_fft_cpx*)m_FreqRight);

  //! compute multichannel output signal in the spectral domain
  for (unsigned int f = 1; f < m_BufferSizeHalf; ++f)
  {
    //! get Lt/Rt amplitudes & phases
    double ampL   = amplitude(m_FreqLeft[f]);
    double ampR   = amplitude(m_FreqRight[f]);
    double phaseL = phase(m_FreqLeft[f]);
    double phaseR = phase(m_FreqRight[f]);

    //! calculate the amplitude & phase differences
    double ampDiff   = clamp((ampL+ampR < epsilon) ? 0.0 : (ampR-ampL) / (ampR+ampL));
    double phaseDiff = abs(phaseL - phaseR);
    if (phaseDiff > pi)
      phaseDiff = 2*pi - phaseDiff;

    double x, y;
    TransformDecode(ampDiff, phaseDiff, x, y);                              //!< decode into x/y soundfield position
    TransformCircularWrap(x, y, m_CircularWrap);                            //!< add wrap control
    y = clamp(y - m_Shift);                                                 //!< add shift control
    y = clamp(1 - (1-y)*m_Depth);                                           //!< add depth control
    TransformFocus(x,y,m_Focus);                                            //!< add focus control
    x = clamp(x * (m_FrontSeparation*(1+y)/2 + m_RearSeparation*(1-y)/2));  //!< add crossfeed control
    double amp_total = sqrt(ampL*ampL + ampR*ampR);                         //!< get total signal amplitude

    //! and total L/C/R signal phases
    double phase_of[] =
    {
      phaseL,
      atan2(m_FreqLeft[f].imag()+m_FreqRight[f].imag(), m_FreqLeft[f].real()+m_FreqRight[f].real()),
      phaseR
    };

    int p = MapToGrid(x), q=MapToGrid(y);                                   //!< compute 2d channel map indexes p/q and update x/y to fractional offsets in the map grid

    //! map position to channel volumes
    for (unsigned c = 0; c < m_Channels-1; ++c)
    {
      //! look up channel map at respective position (with bilinear interpolation) and build the signal
      std::vector<float*> &a = chn_alloc[m_ChannelSetup][c];

      m_Signal[c][f] = polar(amp_total*((1-x)*(1-y)*a[q][p] + x*(1-y)*a[q][p+1] + (1-x)*y*a[q+1][p] + x*y*a[q+1][p+1]), phase_of[1+(int)sign(chn_xsf[m_ChannelSetup][c])]);
    }

    //! optionally redirect bass
    if (m_UseLFE && f < m_HighCutoff)
    {
      //! level of LFE channel according to normalized frequency
      float lfe_level = f < m_LowCutoff ? 1 : 0.5*(1+cos(pi*(f-m_LowCutoff)/(m_HighCutoff-m_LowCutoff)));
      //! assign LFE channel
      m_Signal[m_Channels-1][f] = lfe_level * polar(amp_total,phase_of[1]);
      //! subtract the signal from the other channels
      for (unsigned c = 0; c < m_Channels-1; ++c)
        m_Signal[c][f] *= (1-lfe_level);
    }
  }

  for (unsigned channel = 0; channel < m_Channels; ++channel)
  {
    //! shift the last 2/3 to the first 2/3 of the output buffer
    memcpy(&m_OutputBuffer[channel][0], &m_OutputBuffer[channel][m_BufferSizeHalf], m_BufferSize*sizeof(double));
    //! and clear the rest
    memset(&m_OutputBuffer[channel][m_BufferSize], 0, m_BufferSizeHalf*sizeof(double));

    kiss_fftri(m_FFTInverse, (kiss_fft_cpx*)m_Signal[channel], &m_Destination[0]);

    //! add the result to the last 2/3 of the output buffer, windowed (and remultiplex)
    for (unsigned k = 0; k < m_BufferSize; ++k)
      m_OutputBuffer[channel][k+m_BufferSizeHalf] += m_Window[k]*m_Destination[k];
  }
}

int CFreeSurroundDecoder::MapToGrid(double &x)
{
  double gp = ((x+1)*0.5)*(grid_res-1);
  double i  = Min(grid_res-2,floor(gp));
  x = gp-i;
  return i;
}

void CFreeSurroundDecoder::TransformDecode(double a, double p, double &x, double &y)
{
  x = clamp(1.0047*a + 0.46804*a*p*p*p - 0.2042*a*p*p*p*p + 0.0080586*a*p*p*p*p*p*p*p - 0.0001526*a*p*p*p*p*p*p*p*p*p*p
    - 0.073512*a*a*a*p - 0.2499*a*a*a*p*p*p*p + 0.016932*a*a*a*p*p*p*p*p*p*p - 0.00027707*a*a*a*p*p*p*p*p*p*p*p*p*p
    + 0.048105*a*a*a*a*a*p*p*p*p*p*p*p - 0.0065947*a*a*a*a*a*p*p*p*p*p*p*p*p*p*p + 0.0016006*a*a*a*a*a*p*p*p*p*p*p*p*p*p*p*p
    - 0.0071132*a*a*a*a*a*a*a*p*p*p*p*p*p*p*p*p + 0.0022336*a*a*a*a*a*a*a*p*p*p*p*p*p*p*p*p*p*p
    - 0.0004804*a*a*a*a*a*a*a*p*p*p*p*p*p*p*p*p*p*p*p);
  y = clamp(0.98592 - 0.62237*p + 0.077875*p*p - 0.0026929*p*p*p*p*p + 0.4971*a*a*p - 0.00032124*a*a*p*p*p*p*p*p
    + 9.2491e-006*a*a*a*a*p*p*p*p*p*p*p*p*p*p + 0.051549*a*a*a*a*a*a*a*a + 1.0727e-014*a*a*a*a*a*a*a*a*a*a);
}

void CFreeSurroundDecoder::TransformCircularWrap(double &x, double &y, double refangle)
{
  if (refangle == 90)
    return;

  refangle = refangle*pi/180;
  double baseangle = 90*pi/180;
  // translate into edge-normalized polar coordinates
  double ang = atan2(x,y), len = sqrt(x*x+y*y);
  len = len / EdgeDistance(ang);
  // apply circular_wrap transform
  if (abs(ang) < baseangle/2)
    // angle falls within the front region (to be enlarged)
    ang *= refangle / baseangle;
  else
    // angle falls within the rear region (to be shrunken)
    ang = pi - (-(((refangle - 2*pi)*(pi - abs(ang))*sign(ang))/(2*pi - baseangle)));
  // translate back into soundfield position
  len = len * EdgeDistance(ang);
  x = clamp(sin(ang)*len);
  y = clamp(cos(ang)*len);
}

void CFreeSurroundDecoder::TransformFocus(double &x, double &y, double focus)
{
  if (focus == 0)
    return;
  // translate into edge-normalized polar coordinates
  double ang = atan2(x,y), len = clamp(sqrt(x*x+y*y)/EdgeDistance(ang));
  // apply focus
  len = focus > 0 ? 1-pow(1-len,1+focus*20) : pow(len,1-focus*20);
  // back-transform into euclidian soundfield position
  len = len * EdgeDistance(ang);
  x = clamp(sin(ang)*len);
  y = clamp(cos(ang)*len);
}

void CFreeSurroundDecoder::SetCircularWrap(float v)
{
  m_CircularWrap = v;
}

void CFreeSurroundDecoder::SetShift(float v)
{
  m_Shift = v;
}

void CFreeSurroundDecoder::SetDepth(float v)
{
  m_Depth = v;
}

void CFreeSurroundDecoder::SetFocus(float v)
{
  m_Focus = v;
}

void CFreeSurroundDecoder::SetCenterImage(float v)
{
  m_CenterImage = v;
}

void CFreeSurroundDecoder::SetFrontSeparation(float v)
{
  m_FrontSeparation = v;
}

void CFreeSurroundDecoder::SetRearSeparation(float v)
{
  m_RearSeparation = v;
}

void CFreeSurroundDecoder::SetLowCutoff(float v)
{
  m_LowCutoff = v/m_Samplerate/2.0*m_BufferSizeHalf;
}

void CFreeSurroundDecoder::SetHighCutoff(float v)
{
  m_HighCutoff = v/m_Samplerate/2.0*m_BufferSizeHalf;
}

void CFreeSurroundDecoder::SetBassRedirection(bool v)
{
  m_UseLFE = v;
}

unsigned CFreeSurroundDecoder::buffered()
{
  return m_BufferEmpty ? 0 : m_BufferSizeHalf;
}

unsigned CFreeSurroundDecoder::num_channels(channel_setup s)
{
  return chn_id[s].size();
}

AE_DSP_CHANNEL_PRESENT CFreeSurroundDecoder::channel_at(channel_setup s, unsigned i)
{
  return i < chn_id[s].size() ? chn_id[s][i] : AE_DSP_PRSNT_CH_UNDEFINED;
}
