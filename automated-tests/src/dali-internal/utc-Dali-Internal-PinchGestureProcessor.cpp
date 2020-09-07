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

#include <iostream>

#include <stdlib.h>
#include <dali/public-api/dali-core.h>
#include <dali-test-suite-utils.h>
#include <dali/internal/event/events/pinch-gesture/pinch-gesture-processor.h>
#include <dali/internal/event/events/pinch-gesture/pinch-gesture-event.h>
#include <dali/internal/event/common/scene-impl.h>

using namespace Dali;

void utc_dali_internal_pinch_gesture_processor_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_internal_pinch_gesture_processor_cleanup(void)
{
  test_return_value = TET_PASS;
}

namespace
{

void TestAbortWithState(GestureState state)
{
  TestApplication application;

  Internal::PinchGestureProcessor processor;
  Integration::Scene scene( application.GetScene() );
  Internal::Scene& sceneImpl = GetImplementation( scene );
  Internal::PinchGestureEvent event(state);

  try
  {
    processor.Process(sceneImpl, event);
    DALI_TEST_CHECK( false ); // Should not get here as we expect an abort
  }
  catch(...)
  {
    DALI_TEST_CHECK( true ); // We aborted, so the test has passed
  }
}

} // anon namespace

int UtcDaliPinchGestureProcessorProcessClearStateN(void)
{
  TestAbortWithState(GestureState::CLEAR);
  END_TEST;
}

int UtcDaliPinchGestureProcessorProcessPossibleStateN(void)
{
  TestAbortWithState(GestureState::POSSIBLE);
  END_TEST;
}

