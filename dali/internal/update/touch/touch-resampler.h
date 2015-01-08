#ifndef __DALI_INTERNAL_TOUCH_RESAMPLER_H__
#define __DALI_INTERNAL_TOUCH_RESAMPLER_H__

/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

namespace Dali
{

namespace Integration
{

struct TouchData;
struct TouchEvent;

} // namespace Integration

namespace Internal
{

class TouchPointsTracker;

/**
 * The TouchResampler is responsible for receiving and regulating raw input touches, generating
 * TouchEvents at a regulated frequency suitable for the update/render cycle.
 *
 * Usage:
 *
 * // Touch Thread...
 * OnTouchEvent(const Dali::Integration::TouchData& rawTouchData)
 * {
 *   mResampler.SendTouchData( rawTouchData );
 * }
 *
 * // Update Thread...
 * OnUpdate(unsigned int timestamp)
 * {
 *   mResampler.Update();
 *
 *   while(mResampler.GetNextTouchEvent( touchEvent, timestamp ))
 *   {
 *     // send resampled touchEvent on to Application Thread.
 *   }
 * }
 *
 *
 * Feed touch Events in at any frequency, and new touch Events will come out at the desired
 * frequency of the caller.
 */
class TouchResampler
{
public:

  /**
   * Create a touch Resampler
   */
  static TouchResampler* New();

  /**
   * Virtual destructor
   */
  virtual ~TouchResampler();

  /**
   * Whenever there is new raw touch data it should be sent into the touch-resampler.
   * This will queue the data, and it will be processed on every update
   * (at the video refresh frequency e.g. 60Hz)
   * @param[in] touch The latest touch data
   */
  void SendTouchData( const Dali::Integration::TouchData& touch );

  /**
   * Update should be called on every update frame (i.e. at video refresh frequency)
   * This will identify all the touch incidents that occured since the past call.
   */
  void Update();

  /**
   * Calling GetNextTouchEvent after an Update will retrieve each TouchEvent that
   * was generated by Update.
   * @param[in] touchEvent Reference to a TouchEvent structure to be populated with
   * a single TouchEvent.
   * @param[in] time The time of interest in touch space-time that the caller
   * is interested in.
   */
  bool GetNextTouchEvent( Dali::Integration::TouchEvent& touchEvent, unsigned int time );

  /**
   * Returns whether an update is required
   * @note this changes the internal mUpdateRequired flag to false upon calling.
   */
  bool NeedsUpdate();

private:

  /**
   * Protected constructor.
   */
  TouchResampler();

  // Undefined
  TouchResampler(const TouchResampler&);

  // Undefined
  TouchResampler& operator=(const TouchResampler&);

private:

  bool mUpdateRequired;
  Dali::Integration::TouchData* mTouchesQueue;                    ///< Circular buffer.
  volatile unsigned int mTouchesRead;                             ///< Circular buffer read offset.
  volatile unsigned int mTouchesWrite;                            ///< Circular buffer write offset.
  TouchPointsTracker* mTouchPointsTracker;

};

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_TOUCH_RESAMPLER_H__
