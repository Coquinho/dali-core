/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
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

// CLASS HEADER
#include <dali/internal/event/events/long-press-gesture/long-press-gesture-recognizer.h>

// EXTERNAL INCLUDES
#include <cmath>

#include <dali/devel-api/events/touch-point.h>
#include <dali/public-api/math/vector2.h>

#include <dali/internal/event/common/thread-local-storage.h>
#include <dali/internal/event/events/gesture-requests.h>
#include <dali/integration-api/events/touch-event-integ.h>
#include <dali/integration-api/platform-abstraction.h>

namespace Dali
{

namespace Internal
{

namespace
{
// TODO: Set these according to DPI
const float MAXIMUM_MOTION_ALLOWED = 60.0f;

} // unnamed namespace

LongPressGestureRecognizer::LongPressGestureRecognizer(Observer& observer, Vector2 screenSize, const LongPressGestureRequest& request, uint32_t minimumHoldingTime )
: GestureRecognizer( screenSize, Dali::Gesture::LongPress ),
  mObserver( observer ),
  mState( Clear ),
  mMinimumTouchesRequired( request.minTouches ),
  mMaximumTouchesRequired( request.maxTouches ),
  mTouchTime( 0 ),
  mTimerId( 0 ),
  mMinimumHoldingTime( minimumHoldingTime )
{
}

LongPressGestureRecognizer::~LongPressGestureRecognizer()
{
  if( mTimerId != 0 && ThreadLocalStorage::Created() )
  {
    Dali::Integration::PlatformAbstraction& platformAbstraction = ThreadLocalStorage::Get().GetPlatformAbstraction();
    platformAbstraction.CancelTimer(mTimerId);
  }
}

void LongPressGestureRecognizer::SendEvent(const Integration::TouchEvent& event)
{
  unsigned int pointCount( event.GetPointCount() );
  Dali::Integration::PlatformAbstraction& platformAbstraction = ThreadLocalStorage::Get().GetPlatformAbstraction();
  GestureRecognizerPtr ptr(this); // To keep us from being destroyed during the life-time of this method

  switch (mState)
  {
    // Clear: Wait till one point touches the screen before starting timer.
    case Clear:
    {
      const Integration::Point& point = event.points[0];

      if ( point.GetState() == PointState::DOWN )
      {
        mTouchPositions.clear();
        mTouchPositions[point.GetDeviceId()] = point.GetScreenPosition();

        mTouchTime = event.time;

        if( mTimerId != 0 )
        {
          platformAbstraction.CancelTimer(mTimerId);
        }
        mTimerId = platformAbstraction.StartTimer( mMinimumHoldingTime, MakeCallback( this, &LongPressGestureRecognizer::TimerCallback ) );

        // A long press gesture may be possible, tell Core about this and change state to Touched.
        mState = Touched;
        EmitGesture( Dali::Gesture::Possible );
      }

      break;
    }

    // Touched: Monitor movement and addition/removal of points.
    case Touched:
    {
      if (pointCount > mMaximumTouchesRequired)
      {
        // A long press did not occur, tell Core that it was cancelled and change state to Failed.
        EmitGesture( Dali::Gesture::Cancelled );
        mTouchPositions.clear();
        platformAbstraction.CancelTimer(mTimerId);
        mTimerId = 0;
        mState = Failed;
        break;
      }

      bool endLoop(false);

      for ( Integration::PointContainerConstIterator iter = event.points.begin(), endIter = event.points.end();
           iter != endIter && !endLoop; ++iter)
      {
        switch( iter->GetState() )
        {
          // add point.
          case PointState::DOWN:
          {
            mTouchPositions[iter->GetDeviceId()] = iter->GetScreenPosition();
            break;
          }

          // remove point.
          case PointState::UP:
          case PointState::INTERRUPTED:
          {
            // System has interrupted us, long press is not possible, inform Core
            EmitGesture( Dali::Gesture::Cancelled );
            mTouchPositions.clear();
            platformAbstraction.CancelTimer(mTimerId);
            mTimerId = 0;
            mState = ( pointCount == 1 ) ? Clear : Failed; // Change state to Clear if only one point, Failed otherwise.
            endLoop = true;
            break;
          }

          case PointState::MOTION:
          {
            const Vector2 touchPosition( mTouchPositions[iter->GetDeviceId()] - iter->GetScreenPosition() );
            float distanceSquared = touchPosition.LengthSquared();

            if (distanceSquared > ( MAXIMUM_MOTION_ALLOWED * MAXIMUM_MOTION_ALLOWED ) )
            {
              // We have moved more than the allowable motion for a long press gesture. Inform Core and change state to Failed.
              EmitGesture( Dali::Gesture::Cancelled );
              platformAbstraction.CancelTimer(mTimerId);
              mTimerId = 0;
              mState = Failed;
              endLoop = true;
            }
            break;
          }

          case PointState::STATIONARY:
          case PointState::LEAVE:
          {
            break;
          }
        }
      }
      break;
    }

    // Failed/Finished: Monitor the touches, waiting for all touches to be released.
    case Failed:
    case Finished:
    {
      // eventually the final touch point will be removed, marking the end of this gesture.
      if ( pointCount == 1 )
      {
        PointState::Type primaryPointState = event.points[0].GetState();

        if ( (primaryPointState == PointState::UP) || (primaryPointState == PointState::INTERRUPTED) )
        {
          if(mState == Finished)
          {
            // When the last touch point is lifted, we should inform the Core that the Long press has finished.
            EmitGesture(Dali::Gesture::Finished);
          }
          mTouchPositions.clear();
          mState = Clear; // Reset state to clear when last touch point is lifted.
        }
      }
      break;
    }
  }
}

void LongPressGestureRecognizer::Update(const GestureRequest& request)
{
  const LongPressGestureRequest& longPress = static_cast<const LongPressGestureRequest&>(request);

  mMinimumTouchesRequired = longPress.minTouches;
  mMaximumTouchesRequired = longPress.maxTouches;
}

void LongPressGestureRecognizer::SetMinimumHoldingTime( uint32_t time )
{
  mMinimumHoldingTime = time;
}


bool LongPressGestureRecognizer::TimerCallback()
{
  EmitGesture(Dali::Gesture::Started);

  mState = Finished;

  mTimerId = 0;

  return false;
}

void LongPressGestureRecognizer::EmitGesture(Gesture::State state)
{
  unsigned int touchPoints ( static_cast<unsigned int>( mTouchPositions.size() ) );

  // We should tell Core about the Possible and Cancelled states regardless of whether we have satisfied long press requirements.
  if ( (state == Dali::Gesture::Possible) ||
       (state == Dali::Gesture::Cancelled) ||
       (touchPoints >= mMinimumTouchesRequired) )
  {
    LongPressGestureEvent longPress( state );
    longPress.numberOfTouches = touchPoints;

    for (std::map<int, Vector2>::iterator iter = mTouchPositions.begin(), endIter = mTouchPositions.end();
         iter != endIter; ++iter)
    {
      longPress.point += iter->second;
    }
    longPress.point /= static_cast<float>( touchPoints );

    longPress.time = mTouchTime;
    if ( state != Dali::Gesture::Possible )
    {
      longPress.time += mMinimumHoldingTime;
    }

    if( mScene )
    {
      // Create another handle so the recognizer cannot be destroyed during process function
      GestureRecognizerPtr recognizerHandle = this;

      mObserver.Process(*mScene, longPress);
    }
  }
}

} // namespace Internal

} // namespace Dali
