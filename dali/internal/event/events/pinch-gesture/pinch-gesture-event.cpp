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
#include <dali/internal/event/events/pinch-gesture/pinch-gesture-event.h>

// INTERNAL INCLUDES
#include <dali/public-api/common/dali-common.h>

namespace Dali
{

namespace Internal
{

PinchGestureEvent::PinchGestureEvent(Gesture::State state)
: GestureEvent(Dali::Gesture::Pinch, state),
  scale(0.0f),
  speed(0.0f),
  centerPoint()
{
}

PinchGestureEvent::~PinchGestureEvent()
{
}

} // namespace Internal

} // namespace Dali
