#ifndef TEST_TOUCH_DATA_UTILS_H
#define TEST_TOUCH_DATA_UTILS_H

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

#include <dali/public-api/actors/actor.h>

/**
 * Functor to be connected to an Actor's TouchSignal.
 * Allows the user to specify whether the functor should return true (consumed) or false.
 */
struct TouchDataFunctorConsumeSetter
{
  TouchDataFunctorConsumeSetter( bool& consume )
  : mConsume( consume )
  {
  }

  bool operator()(Dali::Actor actor, const Dali::TouchData& touch)
  {
    return mConsume;
  }

private:
  bool& mConsume;
};

#endif // TEST_TOUCH_DATA_UTILS_H

