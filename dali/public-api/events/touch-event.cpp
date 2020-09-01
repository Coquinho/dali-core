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
#include <dali/public-api/events/touch-event.h>

// INTERNAL INCLUDES
#include <dali/public-api/actors/actor.h>
#include <dali/internal/event/events/touch-event-impl.h>

#include <cstdio>

namespace Dali
{

TouchEvent::TouchEvent()
: BaseHandle()
{
}

TouchEvent::TouchEvent( const TouchEvent& other )
: BaseHandle( other )
{
}

TouchEvent::~TouchEvent()
{
}

TouchEvent& TouchEvent::operator=( const TouchEvent& other )
{
  BaseHandle::operator=( other );
  return *this;
}

unsigned long TouchEvent::GetTime() const
{
  return GetImplementation( *this ).GetTime();
}

std::size_t TouchEvent::GetPointCount() const
{
  return GetImplementation( *this ).GetPointCount();
}

int32_t TouchEvent::GetDeviceId( std::size_t point ) const
{
  return GetImplementation( *this ).GetDeviceId( point );
}

PointState::Type TouchEvent::GetState( std::size_t point ) const
{
  return GetImplementation( *this ).GetState( point );
}

Actor TouchEvent::GetHitActor( std::size_t point ) const
{
  return GetImplementation( *this ).GetHitActor( point );
}

const Vector2& TouchEvent::GetLocalPosition( std::size_t point ) const
{
  return GetImplementation( *this ).GetLocalPosition( point );
}

const Vector2& TouchEvent::GetScreenPosition( std::size_t point ) const
{
  return GetImplementation( *this ).GetScreenPosition( point );
}

float TouchEvent::GetRadius( std::size_t point ) const
{
  return GetImplementation( *this ).GetRadius( point );
}

const Vector2& TouchEvent::GetEllipseRadius( std::size_t point ) const
{
  return GetImplementation( *this ).GetEllipseRadius( point );
}

float TouchEvent::GetPressure( std::size_t point ) const
{
  return GetImplementation( *this ).GetPressure( point );
}

Degree TouchEvent::GetAngle( std::size_t point ) const
{
  return GetImplementation( *this ).GetAngle( point );
}

Device::Class::Type TouchEvent::GetDeviceClass( std::size_t point ) const
{
  return GetImplementation( *this ).GetDeviceClass( point );
}

Device::Subclass::Type TouchEvent::GetDeviceSubclass( std::size_t point ) const
{
  return GetImplementation( *this ).GetDeviceSubclass( point );
}

MouseButton::Type TouchEvent::GetMouseButton( std::size_t point ) const
{
  return GetImplementation( *this ).GetMouseButton( point );
}

TouchEvent::TouchEvent( Internal::TouchEvent* internal )
: BaseHandle( internal )
{
}

} // namespace Dali