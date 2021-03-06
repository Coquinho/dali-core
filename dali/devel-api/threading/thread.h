#ifndef DALI_THREAD_H
#define DALI_THREAD_H

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

// INTERNAL INCLUDES
#include <dali/public-api/common/dali-common.h>

/**
 * The top level DALi namespace
 */
namespace Dali
{
/*
 * @brief Abstract class for thread functionality. Can be used for worker threads.
 */
class DALI_CORE_API Thread
{
public:
  /**
   * @brief Creates a new thread and make it executable.
   */
  void Start();

  /**
   * @brief Wait for thread termination.
   */
  void Join();

protected:
  /**
   * @brief Constructor
   */
  Thread();

  /**
   * @brief Destructor, virtual as this is used as base class
   */
  virtual ~Thread();

  /**
   * The routine that the thread will execute once it is started.
   */
  virtual void Run() = 0;

private:
  /**
   * Helper for the thread calling the entry function.
   * @param[in] This A pointer to the current thread object
   */
  static void InternalThreadEntryFunc(Thread& This);

  // Not copyable or movable
  Thread(const Thread&) = delete;            ///< Deleted copy constructor
  Thread(Thread&&)      = delete;            ///< Deleted move constructor
  Thread& operator=(const Thread&) = delete; ///< Deleted copy assignment operator
  Thread& operator=(Thread&&) = delete;      ///< Deleted move assignment operator

private:
  struct ThreadImpl;
  ThreadImpl* mImpl;
};

} // namespace Dali

#endif // DALI_THREAD_H
