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

#include <complex>

#include "kissfft/kiss_fftr.h"
#include "ChannelMaps.h"

/**
* The supported output channel setups.
* A channel setup is defined by the set of channels that are present. Here is a graphic
* of the cs_5point1 setup: http://en.wikipedia.org/wiki/File:5_1_channels_(surround_sound)_label.svg
*/
typedef enum {
  cs_stereo = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_LFE,
  cs_3stereo = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_LFE,
  cs_5stereo = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_LFE,
  cs_4point1 = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_LFE,
  cs_5point1 = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_LFE,
  cs_6point1 = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_BC | AE_DSP_PRSNT_CH_LFE,
  cs_7point1 = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_LFE,
  cs_7point1_panorama = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR |
               AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_LFE,
  cs_7point1_tricenter = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR |
               AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_LFE,
  cs_8point1 = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR |
               AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BC | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_LFE,
  cs_9point1_densepanorama = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR |
               AE_DSP_PRSNT_CH_TFL | AE_DSP_PRSNT_CH_TFR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_LFE,
  cs_9point1_wrap = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR |
               AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_LFE,
  cs_11point1_densewrap = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR |
               AE_DSP_PRSNT_CH_TFL | AE_DSP_PRSNT_CH_TFR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR |
               AE_DSP_PRSNT_CH_TBL | AE_DSP_PRSNT_CH_TBR | AE_DSP_PRSNT_CH_LFE,
  cs_13point1_totalwrap = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR |
               AE_DSP_PRSNT_CH_TFL | AE_DSP_PRSNT_CH_TFR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR |
               AE_DSP_PRSNT_CH_TBL | AE_DSP_PRSNT_CH_TBR | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_LFE,
  cs_16point1 = AE_DSP_PRSNT_CH_FL | AE_DSP_PRSNT_CH_FLOC | AE_DSP_PRSNT_CH_FC | AE_DSP_PRSNT_CH_FROC | AE_DSP_PRSNT_CH_FR |
               AE_DSP_PRSNT_CH_TFL | AE_DSP_PRSNT_CH_TFR | AE_DSP_PRSNT_CH_SL | AE_DSP_PRSNT_CH_SR | AE_DSP_PRSNT_CH_TBL |
               AE_DSP_PRSNT_CH_TBR | AE_DSP_PRSNT_CH_BL | AE_DSP_PRSNT_CH_BLOC | AE_DSP_PRSNT_CH_BC | AE_DSP_PRSNT_CH_BROC | AE_DSP_PRSNT_CH_BR | AE_DSP_PRSNT_CH_LFE,
  cs_legacy = 0 // same channels as cs_5point1 but different upmixing transform; does not support the focus control
} channel_setup;

typedef std::complex<kiss_fft_scalar> cplx;

/**
* The FreeSurround decoder.
*/
class CFreeSurroundDecoder
{
public:
  /**
   * Create an instance of the decoder.
   * @param setup The output channel setup -- determines the number of output channels
    *         and their place in the sound field.
    * @param blocksize Granularity at which data is processed by the decode() function.
   *           Must be a power of two and should correspond to ca. 10ms worth of single-channel
   *           samples (default is 4096 for 44.1Khz data). Do not make it shorter or longer
   *           than 5ms to 20ms since the granularity at which locations are decoded
   *           changes with this.
   */
  CFreeSurroundDecoder(channel_setup setup=cs_5point1, unsigned blocksize=4096, unsigned int samplerate=48000);
  ~CFreeSurroundDecoder();

  float ** getOutputBuffers();

  /**
   * Decode a chunk of stereo sound. The output is delayed by half of the blocksize.
   * This function is the only one needed for straightforward decoding.
   */
  void Decode(float **inputs);

  /**
   * Flush the internal buffer.
   */
  void Flush();


  /** --- soundfield transformations
   *  These functions allow to set up geometric transformations of the sound field after it has been decoded.
   *  The sound field is best pictured as a 2-dimensional square with the listener in its
   *  center which can be shifted or stretched in various ways before it is sent to the
   *  speakers. The order in which these transformations are applied is as listed below.
   */
  //@{
  /**
   * Allows to wrap the soundfield around the listener in a circular manner.
   * Determines the angle of the frontal sound stage relative to the listener, in degrees.
   * A setting of 90° corresponds to standard surround decoding, 180° stretches the front stage from
   * ear to ear, 270° wraps it around most of the head. The side and rear content of the sound
   * field is compressed accordingly behind the listerer. (default: 90, range: [0°..360°])
   */
  void SetCircularWrap(float v);

  /**
   * Allows to shift the soundfield forward or backward.
   * Value range: [-1.0..+1.0]. 0 is no offset, positive values move the sound
   * forward, negative values move it backwards. (default: 0)
   */
  void SetShift(float v);

  /**
   * Allows to scale the soundfield backwards.
   * Value range: [0.0..+5.0] -- 0 is all compressed to the front, 1 is no change, 5 is scaled 5x backwards (default: 1)
   */
  void SetDepth(float v);

  /**
   * Allows to control the localization (i.e., focality) of sources.
    * Value range: [-1.0..+1.0] -- 0 means unchanged, positive means more localized, negative means more ambient (default: 0)
   */
  void SetFocus(float v);
  //@}

  /** --- rendering parameters
   *  These parameters control how the sound field is mapped onto speakers.
   */
  //@{
  /**
   * Set the presence of the front center channel(s).
   * Value range: [0.0..1.0] -- fully present at 1.0, fully replaced by left/right at 0.0 (default: 1).
   * The default of 1.0 results in spec-conformant decoding ("movie mode") while a value of 0.7 is
   * better suited for music reproduction (which is usually mixed without a center channel).
   */
  void SetCenterImage(float v);

  /**
   * Set the front stereo separation.
   * Value range: [0.0..inf] -- 1.0 is default, 0.0 is mono.
   */
  void SetFrontSeparation(float v);

  /**
   * Set the rear stereo separation.
   * Value range: [0.0..inf] -- 1.0 is default, 0.0 is mono.
   */
  void SetRearSeparation(float v);
  //@}

  /** --- bass redirection (to LFE)
   */
  //@{
  /**
   * Enable/disable LFE channel (default: false = disabled)
   */
  void SetBassRedirection(bool v);

  /**
   * Set the lower end of the transition band, in Hz/Nyquist (default: 40/22050).
   */
  void SetLowCutoff(float v);

  /**
   * Set the upper end of the transition band, in Hz/Nyquist (default: 90/22050).
   */
  void SetHighCutoff(float v);
  //@}

  /** --- info
   */
  //@{
  /**
   * Number of samples currently held in the buffer.
   */
  unsigned buffered();

  /**
    * Number of channels in the given setup.
   */
  static unsigned num_channels(channel_setup s);

  /**
   * Channel id of the i'th channel in the given setup.
   */
  static AE_DSP_CHANNEL_PRESENT channel_at(channel_setup s, unsigned i);
  //@}

private:
  //! constants
  channel_setup m_ChannelSetup;      //!< the channel setup
  unsigned int  m_Channels;          //!< number of samples per input/output block, number of output channels
  unsigned int  m_Samplerate;
  unsigned int  m_BufferSize;
  unsigned int  m_BufferSizeHalf;    //!< half block size precalculated

  //! parameters
  float         m_CircularWrap;      //!< angle of the front soundstage around the listener (90°=default)
  float         m_Shift;             //!< m_FFTForward/backward offset of the sounage
  float         m_Depth;             //!< backward extension of the soundstage
  float         m_Focus;             //!< localization of the sound events
  float         m_CenterImage;       //!< presence of the center speaker
  float         m_FrontSeparation;   //!< front stereo separation
  float         m_RearSeparation;    //!< rear stereo separation
  float         m_LowCutoff;         //!< LFE cutoff frequencies
  float         m_HighCutoff;        //!<  "     "        "
  bool          m_UseLFE;            //!< whether to use the LFE channel

  //! FFT data structures
  float        *m_Destination;
  float        *m_TotalLeft;         //!< left total, (source arrays), time-domain destination buffer array
  float        *m_TotalRight;        //!< right total,   "
  cplx         *m_FreqLeft;          //!< left total in frequency domain
  cplx         *m_FreqRight;         //!< right total in frequency domain
  kiss_fftr_cfg m_FFTForward;        //!< FFT buffers
  kiss_fftr_cfg m_FFTInverse;        //!<  "     "

   //! buffers
  bool          m_BufferEmpty;       //!< whether the buffer is currently empty or dirty
  float        *m_InputBuffer[2];    //!< for passing back to driver
  float        *m_OutputBuffer[17];  //!<
  cplx         *m_Signal[17];        //!< the signal to be constructed in every channel, in the frequency domain
  double       *m_Window;            //!< the window function, precomputed

  //! helper functions
  static inline double sqr(double x);
  static inline double amplitude(const cplx &x);
  static inline double phase(const cplx &x);
  static inline cplx polar(double a, double p);
  static inline double Min(double a, double b);
  static inline double Max(double a, double b);
  static inline double clamp(double x);
  static inline double sign(double x);

  //!> get the distance of the soundfield edge, along a given angle
  static inline double EdgeDistance(double a);

  //!> decode a block of data and overlap-add it into outbuf
  void BufferedDecode(float *input[2], int ptr = 0);

  //!> get the index (and fractional offset!) in a piecewise-linear channel allocation grid
  int MapToGrid(double &x);

  //!> transform amp/phase difference space into x/y soundfield space
  void TransformDecode(double a, double p, double &x, double &y);

  //!> apply a circular_wrap transformation to some position
  void TransformCircularWrap(double &x, double &y, double refangle);

  //!> apply a focus transformation to some position
  void TransformFocus(double &x, double &y, double focus);
};
