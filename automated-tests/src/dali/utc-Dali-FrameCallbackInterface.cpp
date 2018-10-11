/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
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
#include <dali/devel-api/common/map-wrapper.h>
#include <dali/devel-api/common/stage-devel.h>
#include <dali/devel-api/update/frame-callback-interface.h>
#include <dali/devel-api/update/update-proxy.h>
#include <dali-test-suite-utils.h>

using namespace Dali;

void utc_dali_frame_callback_interface_startup(void)
{
  test_return_value = TET_UNDEF;
}

void utc_dali_frame_callback_interface_cleanup(void)
{
  test_return_value = TET_PASS;
}

///////////////////////////////////////////////////////////////////////////////
namespace
{

class FrameCallbackBasic : public FrameCallbackInterface
{
public:

  FrameCallbackBasic()
  : mCalled( false )
  {
  }

  virtual void Update( Dali::UpdateProxy& updateProxy, float elapsedSeconds )
  {
    mCalled = true;
  }

  bool mCalled;
};

} // anon namespace

class FrameCallbackOneActor : public FrameCallbackBasic
{
public:

  FrameCallbackOneActor( unsigned int actorId )
  : mActorId( actorId )
  {
  }

  virtual void Update( Dali::UpdateProxy& updateProxy, float elapsedSeconds )
  {
    FrameCallbackBasic::Update( updateProxy, elapsedSeconds );
    mSizeGetSizeCall = updateProxy.GetSize( mActorId );
    mPositionGetPositionCall = updateProxy.GetPosition( mActorId );
    updateProxy.GetPositionAndSize( mActorId, mPositionGetPositionAndSizeCall, mSizeGetPositionAndSizeCall );
    mColor = updateProxy.GetColor( mActorId );
    mScale = updateProxy.GetScale( mActorId );
  }

  const unsigned int mActorId;

  Vector3 mSizeGetSizeCall;
  Vector3 mPositionGetPositionCall;
  Vector3 mPositionGetPositionAndSizeCall;
  Vector3 mSizeGetPositionAndSizeCall;
  Vector4 mColor;
  Vector3 mScale;
};

class FrameCallbackSetter : public FrameCallbackBasic
{
public:

  FrameCallbackSetter(
      unsigned int actorId,
      const Vector3& sizeToSet,
      const Vector3& positionToSet,
      const Vector4& colorToSet,
      const Vector3& scaleToSet )
  : mActorId( actorId ),
    mSizeToSet( sizeToSet ),
    mPositionToSet( positionToSet ),
    mColorToSet( colorToSet ),
    mScaleToSet( scaleToSet )
  {
  }

  virtual void Update( Dali::UpdateProxy& updateProxy, float elapsedSeconds )
  {
    FrameCallbackBasic::Update( updateProxy, elapsedSeconds );
    updateProxy.SetSize( mActorId, mSizeToSet );
    updateProxy.SetPosition( mActorId, mPositionToSet );
    updateProxy.SetColor( mActorId, mColorToSet );
    updateProxy.SetScale( mActorId, mScaleToSet );
    mSizeAfterSetting = updateProxy.GetSize( mActorId );
    mPositionAfterSetting = updateProxy.GetPosition( mActorId );
    mColorAfterSetting = updateProxy.GetColor( mActorId );
    mScaleAfterSetting = updateProxy.GetScale( mActorId );
  }

  const unsigned int mActorId;
  const Vector3& mSizeToSet;
  const Vector3& mPositionToSet;
  const Vector4& mColorToSet;
  const Vector3& mScaleToSet;

  Vector3 mSizeAfterSetting;
  Vector3 mPositionAfterSetting;
  Vector4 mColorAfterSetting;
  Vector3 mScaleAfterSetting;
};

class FrameCallbackBaker : public FrameCallbackBasic
{
public:

  FrameCallbackBaker(
      unsigned int actorId,
      const Vector3& sizeToSet,
      const Vector3& positionToSet,
      const Vector4& colorToSet,
      const Vector3& scaleToSet )
  : mActorId( actorId ),
    mSizeToSet( sizeToSet ),
    mPositionToSet( positionToSet ),
    mColorToSet( colorToSet ),
    mScaleToSet( scaleToSet )
  {
  }

  virtual void Update( Dali::UpdateProxy& updateProxy, float elapsedSeconds )
  {
    FrameCallbackBasic::Update( updateProxy, elapsedSeconds );
    updateProxy.BakeSize( mActorId, mSizeToSet );
    updateProxy.BakePosition( mActorId, mPositionToSet );
    updateProxy.BakeColor( mActorId, mColorToSet );
    updateProxy.BakeScale( mActorId, mScaleToSet );
    mSizeAfterSetting = updateProxy.GetSize( mActorId );
    mPositionAfterSetting = updateProxy.GetPosition( mActorId );
    mColorAfterSetting = updateProxy.GetColor( mActorId );
    mScaleAfterSetting = updateProxy.GetScale( mActorId );
  }

  const unsigned int mActorId;
  const Vector3& mSizeToSet;
  const Vector3& mPositionToSet;
  const Vector4& mColorToSet;
  const Vector3& mScaleToSet;

  Vector3 mSizeAfterSetting;
  Vector3 mPositionAfterSetting;
  Vector4 mColorAfterSetting;
  Vector3 mScaleAfterSetting;
};


class FrameCallbackMultipleActors : public FrameCallbackBasic
{
public:

  FrameCallbackMultipleActors()
  {
  }

  virtual void Update( Dali::UpdateProxy& updateProxy, float elapsedSeconds )
  {
    FrameCallbackBasic::Update( updateProxy, elapsedSeconds );
    for( auto&& i : mActorIds )
    {
      Vector3 position;
      Vector3 size;
      updateProxy.GetPositionAndSize( i, position, size );
      mPositions[ i ] = position;
      mSizes[ i ] = size;
    }
  }

  Vector< unsigned int > mActorIds;

  std::map< unsigned int, Vector3 > mPositions;
  std::map< unsigned int, Vector3 > mSizes;
};

///////////////////////////////////////////////////////////////////////////////

int UtcDaliFrameCallbackCheckInstallationAndRemoval(void)
{
  TestApplication application;

  FrameCallbackBasic frameCallback;

  Stage stage = Stage::GetCurrent();
  DevelStage::AddFrameCallback( stage, frameCallback, stage.GetRootLayer() );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( frameCallback.mCalled, true, TEST_LOCATION );

  frameCallback.mCalled = false;

  DevelStage::RemoveFrameCallback( stage, frameCallback );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( frameCallback.mCalled, false, TEST_LOCATION );

  END_TEST;
}

int UtcDaliFrameCallbackGetters(void)
{
  TestApplication application;
  Vector2 actorSize( 200, 300 );
  Vector4 color( 0.5f, 0.6f, 0.7f, 0.8f );
  Vector3 position( 10.0f, 20.0f, 30.0f );
  Vector3 scale( 2.0f, 4.0f, 6.0f );

  Actor actor = Actor::New();
  actor.SetParentOrigin( ParentOrigin::TOP_LEFT );
  actor.SetAnchorPoint( AnchorPoint::TOP_LEFT );
  actor.SetSize( actorSize );
  actor.SetColor( color );
  actor.SetPosition( position );
  actor.SetScale( scale );

  Stage stage = Stage::GetCurrent();
  stage.Add( actor );

  FrameCallbackOneActor frameCallback( actor.GetId() );
  DevelStage::AddFrameCallback( stage, frameCallback, stage.GetRootLayer() );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( frameCallback.mCalled, true, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mSizeGetSizeCall, Vector3( actorSize.width, actorSize.height, 0.0f ), TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mPositionGetPositionCall, position, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mPositionGetPositionAndSizeCall, position, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mSizeGetPositionAndSizeCall, Vector3( actorSize.width, actorSize.height, 0.0f ), TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mColor, color, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mScale, scale, TEST_LOCATION );

  END_TEST;
}

int UtcDaliFrameCallbackSetters(void)
{
  TestApplication application;
  Vector2 actorSize( 200, 300 );

  Actor actor = Actor::New();
  actor.SetParentOrigin( ParentOrigin::TOP_LEFT );
  actor.SetAnchorPoint( AnchorPoint::TOP_LEFT );
  actor.SetSize( actorSize );

  Stage stage = Stage::GetCurrent();
  stage.Add( actor );

  Vector3 sizeToSet( 1.0f, 2.0f, 3.0f );
  Vector3 positionToSet( 10.0f, 20.0f, 30.0f );
  Vector4 colorToSet( Color::MAGENTA );
  Vector3 scaleToSet( 1.0f, 3.0f, 5.0f );

  FrameCallbackSetter frameCallback( actor.GetId(), sizeToSet, positionToSet, colorToSet, scaleToSet );
  DevelStage::AddFrameCallback( stage, frameCallback, stage.GetRootLayer() );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( frameCallback.mCalled, true, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mSizeAfterSetting, sizeToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mPositionAfterSetting, positionToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mColorAfterSetting, colorToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mScaleAfterSetting, scaleToSet, TEST_LOCATION );

  // Ensure the actual actor values haven't changed as we didn't bake the values after removing the callback
  DevelStage::RemoveFrameCallback( stage, frameCallback );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::POSITION ).Get< Vector3 >(), Vector3::ZERO, TEST_LOCATION );
  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::SIZE ).Get< Vector3 >(), Vector3( actorSize ), TEST_LOCATION );
  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::COLOR ).Get< Vector4 >(), Color::WHITE, TEST_LOCATION );
  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::SCALE ).Get< Vector3 >(), Vector3::ONE, TEST_LOCATION );

  END_TEST;
}

int UtcDaliFrameCallbackBake(void)
{
  TestApplication application;
  Vector2 actorSize( 200, 300 );

  Actor actor = Actor::New();
  actor.SetParentOrigin( ParentOrigin::TOP_LEFT );
  actor.SetAnchorPoint( AnchorPoint::TOP_LEFT );
  actor.SetSize( actorSize );

  Stage stage = Stage::GetCurrent();
  stage.Add( actor );

  Vector3 sizeToSet( 1.0f, 2.0f, 3.0f );
  Vector3 positionToSet( 10.0f, 20.0f, 30.0f );
  Vector4 colorToSet( Color::MAGENTA );
  Vector3 scaleToSet( 1.0f, 3.0f, 5.0f );

  FrameCallbackBaker frameCallback( actor.GetId(), sizeToSet, positionToSet, colorToSet, scaleToSet );
  DevelStage::AddFrameCallback( stage, frameCallback, stage.GetRootLayer() );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( frameCallback.mCalled, true, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mSizeAfterSetting, sizeToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mPositionAfterSetting, positionToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mColorAfterSetting, colorToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mScaleAfterSetting, scaleToSet, TEST_LOCATION );

  // Ensure the new values are saved after removing the callback
  DevelStage::RemoveFrameCallback( stage, frameCallback );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::POSITION ).Get< Vector3 >(), positionToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::SIZE ).Get< Vector3 >(), sizeToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::COLOR ).Get< Vector4 >(), colorToSet, TEST_LOCATION );
  DALI_TEST_EQUALS( actor.GetCurrentProperty( Actor::Property::SCALE ).Get< Vector3 >(), scaleToSet, TEST_LOCATION );

  END_TEST;
}


int UtcDaliFrameCallbackMultipleActors(void)
{
  /**
   * Tree:
   *              root-layer
   *              /        \
   *             /          \
   *            /            \
   *           /              \
   *        actorA           actorE
   *         / \              / \
   *        /   \            /   \
   *    actorB  actorD   actorF actorG
   *      /                        \
   *   actorC                     actorH
   *
   *  Screen positions (with minor alterations due to local position):
   *  -----------------------
   *  |actorA|actorD        |
   *  |      actorB         |
   *  |      actorC         |
   *  |                     |
   *  |                     |
   *  |                     |
   *  |                     |
   *  |                     |
   *  |actorF       actorH  |
   *  |actorE|actorG        |
   *  -----------------------
   */

  TestApplication application;
  Stage stage = Stage::GetCurrent();

  std::map< char, Vector3 > sizes;
  sizes['A'] = Vector3(  50.0f,  50.0f, 0.0f );
  sizes['B'] = Vector3( 100.0f, 100.0f, 0.0f );
  sizes['C'] = Vector3( 150.0f, 150.0f, 0.0f );
  sizes['D'] = Vector3( 200.0f, 200.0f, 0.0f );
  sizes['E'] = Vector3( 250.0f, 250.0f, 0.0f );
  sizes['F'] = Vector3( 300.0f, 300.0f, 0.0f );
  sizes['G'] = Vector3( 350.0f, 350.0f, 0.0f );
  sizes['H'] = Vector3( 400.0f, 350.0f, 0.0f );

  std::map< char, Vector3 > positions;
  positions['A'] = Vector3(  0.0f,  1.0f,  2.0f );
  positions['B'] = Vector3(  2.0f,  3.0f,  4.0f );
  positions['C'] = Vector3(  5.0f,  6.0f,  7.0f );
  positions['D'] = Vector3(  8.0f,  9.0f, 10.0f );
  positions['E'] = Vector3( 11.0f, 12.0f, 13.0f );
  positions['F'] = Vector3( 14.0f, 15.0f, 16.0f );
  positions['G'] = Vector3( 17.0f, 18.0f, 19.0f );
  positions['H'] = Vector3( 20.0f, 21.0f, 22.0f );

  Actor actorA = Actor::New();
  actorA.SetParentOrigin( ParentOrigin::TOP_LEFT );
  actorA.SetAnchorPoint( AnchorPoint::TOP_LEFT );
  actorA.SetSize( sizes['A'] );
  actorA.SetPosition( positions['A'] );
  stage.Add( actorA );

  Actor actorB = Actor::New();
  actorB.SetParentOrigin( ParentOrigin::BOTTOM_RIGHT );
  actorB.SetAnchorPoint( AnchorPoint::TOP_LEFT );
  actorB.SetSize( sizes['B'] );
  actorB.SetPosition( positions['B'] );
  actorA.Add( actorB );

  Actor actorC = Actor::New();
  actorC.SetParentOrigin( ParentOrigin::BOTTOM_CENTER );
  actorC.SetAnchorPoint( AnchorPoint::TOP_CENTER );
  actorC.SetSize( sizes['C'] );
  actorC.SetPosition( positions['C'] );
  actorB.Add( actorC );

  Actor actorD = Actor::New();
  actorD.SetParentOrigin( ParentOrigin::CENTER_RIGHT );
  actorD.SetAnchorPoint( AnchorPoint::CENTER_LEFT );
  actorD.SetSize( sizes['D'] );
  actorD.SetPosition( positions['D'] );
  actorA.Add( actorD );

  Actor actorE = Actor::New();
  actorE.SetParentOrigin( ParentOrigin::BOTTOM_LEFT );
  actorE.SetAnchorPoint( AnchorPoint::BOTTOM_LEFT );
  actorE.SetSize( sizes['E'] );
  actorE.SetPosition( positions['E'] );
  stage.Add( actorE );

  Actor actorF = Actor::New();
  actorF.SetParentOrigin( ParentOrigin::TOP_CENTER );
  actorF.SetAnchorPoint( AnchorPoint::BOTTOM_CENTER );
  actorF.SetSize( sizes['F'] );
  actorF.SetPosition( positions['F'] );
  actorE.Add( actorF );

  Actor actorG = Actor::New();
  actorG.SetParentOrigin( ParentOrigin::CENTER_RIGHT );
  actorG.SetAnchorPoint( AnchorPoint::CENTER_LEFT );
  actorG.SetSize( sizes['G'] );
  actorG.SetPosition( positions['G'] );
  actorE.Add( actorG );

  Actor actorH = Actor::New();
  actorH.SetParentOrigin( ParentOrigin::TOP_RIGHT );
  actorH.SetAnchorPoint( AnchorPoint::BOTTOM_LEFT );
  actorH.SetSize( sizes['H'] );
  actorH.SetPosition( positions['H'] );
  actorG.Add( actorH );

  std::map< char, unsigned int > actorIds;
  actorIds['A'] = actorA.GetId();
  actorIds['B'] = actorB.GetId();
  actorIds['C'] = actorC.GetId();
  actorIds['D'] = actorD.GetId();
  actorIds['E'] = actorE.GetId();
  actorIds['F'] = actorF.GetId();
  actorIds['G'] = actorG.GetId();
  actorIds['H'] = actorH.GetId();

  FrameCallbackMultipleActors frameCallback;
  for( auto&& i : actorIds )
  {
    frameCallback.mActorIds.PushBack( i.second );
  }

  DevelStage::AddFrameCallback( stage, frameCallback, stage.GetRootLayer() );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( frameCallback.mCalled, true, TEST_LOCATION );

  for( char i = 'A'; i <= 'H'; ++i )
  {
    DALI_TEST_EQUALS( frameCallback.mPositions[ actorIds[ i ] ], positions[ i ], TEST_LOCATION );
    DALI_TEST_EQUALS( frameCallback.mSizes[ actorIds[ i ] ], sizes[ i ], TEST_LOCATION );
  }

  // Render again to make sure it still gets called and gives the correct values (in case any optimisations break this)
  frameCallback.mCalled = false;

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( frameCallback.mCalled, true, TEST_LOCATION );

  for( char i = 'A'; i <= 'H'; ++i )
  {
    DALI_TEST_EQUALS( frameCallback.mPositions[ actorIds[ i ] ], positions[ i ], TEST_LOCATION );
    DALI_TEST_EQUALS( frameCallback.mSizes[ actorIds[ i ] ], sizes[ i ], TEST_LOCATION );
  }

  END_TEST;
}

int UtcDaliFrameCallbackCheckActorNotAdded(void)
{
  TestApplication application;

  Actor actor = Actor::New();
  actor.SetParentOrigin( ParentOrigin::TOP_LEFT );
  actor.SetAnchorPoint( AnchorPoint::TOP_LEFT );
  actor.SetSize( 200, 300 );

  Stage stage = Stage::GetCurrent();
  FrameCallbackOneActor frameCallback( actor.GetId() );
  DevelStage::AddFrameCallback( stage, frameCallback, stage.GetRootLayer() );

  application.SendNotification();
  application.Render();

  // All should be default constructed objects
  DALI_TEST_EQUALS( frameCallback.mCalled, true, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mPositionGetPositionCall, Vector3::ZERO, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mSizeGetSizeCall, Vector3::ZERO, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mColor, Vector4::ZERO, TEST_LOCATION );
  DALI_TEST_EQUALS( frameCallback.mScale, Vector3::ZERO, TEST_LOCATION );

  END_TEST;
}
