/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd.
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

#include "test-render-surface.h"

namespace Dali
{

TestRenderSurface::TestRenderSurface( Dali::PositionSize positionSize )
: mPositionSize( positionSize ),
  mBackgroundColor()
{
}

TestRenderSurface::~TestRenderSurface()
{
}

Dali::PositionSize TestRenderSurface::GetPositionSize() const
{
  return mPositionSize;
};

void TestRenderSurface::GetDpi( unsigned int& dpiHorizontal, unsigned int& dpiVertical )
{
  dpiHorizontal = dpiVertical = 96;
};

int TestRenderSurface::GetOrientation() const
{
  return 0;
};

void TestRenderSurface::InitializeGraphics()
{
}

void TestRenderSurface::CreateSurface()
{
}

void TestRenderSurface::DestroySurface()
{
}

bool TestRenderSurface::ReplaceGraphicsSurface()
{
  return false;
}

void TestRenderSurface::MoveResize( Dali::PositionSize positionSize )
{
  mPositionSize = positionSize;
}

void TestRenderSurface::StartRender()
{
}

bool TestRenderSurface::PreRender( bool resizingSurface )
{
  return true;
}

void TestRenderSurface::PostRender( bool renderToFbo, bool replacingSurface, bool resizingSurface )
{
}

void TestRenderSurface::StopRender()
{
}

void TestRenderSurface::ReleaseLock()
{
}

Dali::Integration::RenderSurface::Type TestRenderSurface::GetSurfaceType()
{
  return WINDOW_RENDER_SURFACE;
}

void TestRenderSurface::MakeContextCurrent()
{
}

Integration::DepthBufferAvailable TestRenderSurface::GetDepthBufferRequired()
{
  return Integration::DepthBufferAvailable::TRUE;
}

Integration::StencilBufferAvailable TestRenderSurface::GetStencilBufferRequired()
{
  return Integration::StencilBufferAvailable::TRUE;
}

void TestRenderSurface::SetBackgroundColor( Vector4 color )
{
  mBackgroundColor = color;
}

Vector4 TestRenderSurface::GetBackgroundColor()
{
  return mBackgroundColor;
}

void TestRenderSurface::SetDamagedRect( const Dali::DamagedRect& damagedRect, Dali::DamagedRect& mergedRect )
{
}

int32_t TestRenderSurface::GetBufferAge()
{
  return 0;
}

} // Namespace dali