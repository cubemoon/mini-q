/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCTransition.h"
#include "CCCamera.h"
#include "support/CCPointExtension.h"
#include "CCDirector.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "CCLayer.h"
#include "misc_nodes/CCRenderTexture.h"


NS_CC_BEGIN

const unsigned int kSceneFade = 0xFADEFADE;

CCTransitionScene::CCTransitionScene()
{
}
CCTransitionScene::~CCTransitionScene()
{
    m_pInScene->release();
    m_pOutScene->release();
}

CCTransitionScene * CCTransitionScene::create(float t, CCScene *scene)
{
    CCTransitionScene * pScene = new CCTransitionScene();
    if(pScene && pScene->initWithDuration(t,scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

bool CCTransitionScene::initWithDuration(float t, CCScene *scene)
{
    CCAssert( scene != NULL, "Argument scene must be non-nil");

    if (CCScene::init())
    {
        m_fDuration = t;

        // retain
        m_pInScene = scene;
        m_pInScene->retain();
        m_pOutScene = CCDirector::sharedDirector()->getRunningScene();
        if (m_pOutScene == NULL)
        {
            m_pOutScene = CCScene::create();
            m_pOutScene->init();
        }
        m_pOutScene->retain();

        CCAssert( m_pInScene != m_pOutScene, "Incoming scene must be different from the outgoing scene" );
        
        sceneOrder();

        return true;
    }
    else
    {
        return false;
    }
}

void CCTransitionScene::sceneOrder()
{
    m_bIsInSceneOnTop = true;
}

void CCTransitionScene::draw()
{
    CCScene::draw();

    if( m_bIsInSceneOnTop ) {
        m_pOutScene->visit();
        m_pInScene->visit();
    } else {
        m_pInScene->visit();
        m_pOutScene->visit();
    }
}

void CCTransitionScene::finish()
{
    // clean up     
     m_pInScene->setVisible(true);
     m_pInScene->setPosition(ccp(0,0));
     m_pInScene->setScale(1.0f);
     m_pInScene->setRotation(0.0f);
     m_pInScene->getCamera()->restore();
 
     m_pOutScene->setVisible(false);
     m_pOutScene->setPosition(ccp(0,0));
     m_pOutScene->setScale(1.0f);
     m_pOutScene->setRotation(0.0f);
     m_pOutScene->getCamera()->restore();

    //[self schedule:@selector(setNewScene:) interval:0];
    this->schedule(schedule_selector(CCTransitionScene::setNewScene), 0);

}

void CCTransitionScene::setNewScene(float dt)
{    
    CC_UNUSED_PARAM(dt);

    this->unschedule(schedule_selector(CCTransitionScene::setNewScene));
    
    // Before replacing, save the "send cleanup to scene"
    CCDirector *director = CCDirector::sharedDirector();
    m_bIsSendCleanupToScene = director->isSendCleanupToScene();
    
    director->replaceScene(m_pInScene);
    
    // issue #267
    m_pOutScene->setVisible(true);
}

void CCTransitionScene::hideOutShowIn()
{
    m_pInScene->setVisible(true);
    m_pOutScene->setVisible(false);
}


// custom onEnter
void CCTransitionScene::onEnter()
{
    CCScene::onEnter();
    
    // disable events while transitions
    CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(false);
    
    // outScene should not receive the onEnter callback
    // only the onExitTransitionDidStart
    m_pOutScene->onExitTransitionDidStart();
    
    m_pInScene->onEnter();
}

// custom onExit
void CCTransitionScene::onExit()
{
    CCScene::onExit();
    
    // enable events while transitions
    CCDirector::sharedDirector()->getTouchDispatcher()->setDispatchEvents(true);
    
    m_pOutScene->onExit();

    // m_pInScene should not receive the onEnter callback
    // only the onEnterTransitionDidFinish
    m_pInScene->onEnterTransitionDidFinish();
}

// custom cleanup
void CCTransitionScene::cleanup()
{
    CCScene::cleanup();

    if( m_bIsSendCleanupToScene )
        m_pOutScene->cleanup();
}

//
// Oriented Transition
//

CCTransitionSceneOriented::CCTransitionSceneOriented()
{
}

CCTransitionSceneOriented::~CCTransitionSceneOriented()
{
}

CCTransitionSceneOriented * CCTransitionSceneOriented::create(float t, CCScene *scene, tOrientation orientation)
{
    CCTransitionSceneOriented * pScene = new CCTransitionSceneOriented();
    pScene->initWithDuration(t,scene,orientation);
    pScene->autorelease();
    return pScene;
}

bool CCTransitionSceneOriented::initWithDuration(float t, CCScene *scene, tOrientation orientation)
{
    if ( CCTransitionScene::initWithDuration(t, scene) )
    {
        m_eOrientation = orientation;
    }
    return true;
}

//
// RotoZoom
//
CCTransitionRotoZoom::CCTransitionRotoZoom()
{
}

CCTransitionRotoZoom* CCTransitionRotoZoom::create(float t, CCScene* scene)                   
{                                                               
    CCTransitionRotoZoom* pScene = new CCTransitionRotoZoom();                                
    if(pScene && pScene->initWithDuration(t, scene))            
    {                                                           
        pScene->autorelease();                                  
        return pScene;                                          
    }                                                           
    CC_SAFE_DELETE(pScene);                                     
    return NULL;                                                
}

CCTransitionRotoZoom::~CCTransitionRotoZoom()
{
}

void CCTransitionRotoZoom:: onEnter()
{
    CCTransitionScene::onEnter();
}

//
// JumpZoom
//
CCTransitionJumpZoom::CCTransitionJumpZoom()
{
}
CCTransitionJumpZoom::~CCTransitionJumpZoom()
{
}

CCTransitionJumpZoom* CCTransitionJumpZoom::create(float t, CCScene* scene)
{
    CCTransitionJumpZoom* pScene = new CCTransitionJumpZoom();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionJumpZoom::onEnter()
{
    CCTransitionScene::onEnter();
}

//
// MoveInL
//
CCTransitionMoveInL::CCTransitionMoveInL()
{
}

CCTransitionMoveInL::~CCTransitionMoveInL()
{
}

CCTransitionMoveInL* CCTransitionMoveInL::create(float t, CCScene* scene)
{
    CCTransitionMoveInL* pScene = new CCTransitionMoveInL();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionMoveInL::onEnter()
{
    CCTransitionScene::onEnter();
    this->initScenes();
}

void CCTransitionMoveInL::initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition(ccp(-s.width,0));
}

//
// MoveInR
//
CCTransitionMoveInR::CCTransitionMoveInR()
{
}
CCTransitionMoveInR::~CCTransitionMoveInR()
{
}

CCTransitionMoveInR* CCTransitionMoveInR::create(float t, CCScene* scene)
{
    CCTransitionMoveInR* pScene = new CCTransitionMoveInR();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionMoveInR::initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition( ccp(s.width,0) );
}

//
// MoveInT
//
CCTransitionMoveInT::CCTransitionMoveInT()
{
}
CCTransitionMoveInT::~CCTransitionMoveInT()
{
}

CCTransitionMoveInT* CCTransitionMoveInT::create(float t, CCScene* scene)
{
    CCTransitionMoveInT* pScene = new CCTransitionMoveInT();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionMoveInT::initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition( ccp(0,s.height) );
}

//
// MoveInB
//
CCTransitionMoveInB::CCTransitionMoveInB()
{
}
CCTransitionMoveInB::~CCTransitionMoveInB()
{
}

CCTransitionMoveInB* CCTransitionMoveInB::create(float t, CCScene* scene)
{
    CCTransitionMoveInB* pScene = new CCTransitionMoveInB();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionMoveInB::initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition( ccp(0,-s.height) );
}


//
// SlideInL
//

// The adjust factor is needed to prevent issue #442
// One solution is to use DONT_RENDER_IN_SUBPIXELS images, but NO
// The other issue is that in some transitions (and I don't know why)
// the order should be reversed (In in top of Out or vice-versa).
#define ADJUST_FACTOR 0.5f
CCTransitionSlideInL::CCTransitionSlideInL()
{
}

CCTransitionSlideInL::~CCTransitionSlideInL()
{
}

void CCTransitionSlideInL::onEnter()
{
    CCTransitionScene::onEnter();
    this->initScenes();
}

void CCTransitionSlideInL::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void CCTransitionSlideInL:: initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition( ccp(-(s.width-ADJUST_FACTOR),0) );
}

CCTransitionSlideInL* CCTransitionSlideInL::create(float t, CCScene* scene)
{
    CCTransitionSlideInL* pScene = new CCTransitionSlideInL();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

//
// SlideInR
//
CCTransitionSlideInR::CCTransitionSlideInR()
{
}
CCTransitionSlideInR::~CCTransitionSlideInR()
{
}

CCTransitionSlideInR* CCTransitionSlideInR::create(float t, CCScene* scene)
{
    CCTransitionSlideInR* pScene = new CCTransitionSlideInR();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionSlideInR::sceneOrder()
{
    m_bIsInSceneOnTop = true;
}

void CCTransitionSlideInR::initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition( ccp(s.width-ADJUST_FACTOR,0) );
}


//
// SlideInT
//
CCTransitionSlideInT::CCTransitionSlideInT()
{
}
CCTransitionSlideInT::~CCTransitionSlideInT()
{
}

CCTransitionSlideInT* CCTransitionSlideInT::create(float t, CCScene* scene)
{
    CCTransitionSlideInT* pScene = new CCTransitionSlideInT();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionSlideInT::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void CCTransitionSlideInT::initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition( ccp(0,s.height-ADJUST_FACTOR) );
}

//
// SlideInB
//
CCTransitionSlideInB::CCTransitionSlideInB()
{
}
CCTransitionSlideInB::~CCTransitionSlideInB()
{
}

CCTransitionSlideInB* CCTransitionSlideInB::create(float t, CCScene* scene)
{
    CCTransitionSlideInB* pScene = new CCTransitionSlideInB();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionSlideInB::sceneOrder()
{
    m_bIsInSceneOnTop = true;
}

void CCTransitionSlideInB:: initScenes()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    m_pInScene->setPosition( ccp(0,-(s.height-ADJUST_FACTOR)) );
}

//
// ShrinkGrow Transition
//
CCTransitionShrinkGrow::CCTransitionShrinkGrow()
{
}
CCTransitionShrinkGrow::~CCTransitionShrinkGrow()
{
}

CCTransitionShrinkGrow* CCTransitionShrinkGrow::create(float t, CCScene* scene)
{
    CCTransitionShrinkGrow* pScene = new CCTransitionShrinkGrow();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionShrinkGrow::onEnter()
{
    CCTransitionScene::onEnter();
}

//
// FlipX Transition
//
CCTransitionFlipX::CCTransitionFlipX()
{
}
CCTransitionFlipX::~CCTransitionFlipX()
{
}

void CCTransitionFlipX::onEnter()
{
    CCTransitionSceneOriented::onEnter();
}

CCTransitionFlipX* CCTransitionFlipX::create(float t, CCScene* s, tOrientation o)
{
    CCTransitionFlipX* pScene = new CCTransitionFlipX();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

CCTransitionFlipX* CCTransitionFlipX::create(float t, CCScene* s)
{
    return CCTransitionFlipX::create(t, s, kCCTransitionOrientationRightOver);
}

//
// FlipY Transition
//
CCTransitionFlipY::CCTransitionFlipY()
{
}
CCTransitionFlipY::~CCTransitionFlipY()
{
}

void CCTransitionFlipY::onEnter()
{
    CCTransitionSceneOriented::onEnter();
}

CCTransitionFlipY* CCTransitionFlipY::create(float t, CCScene* s, tOrientation o)
{
    CCTransitionFlipY* pScene = new CCTransitionFlipY();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

CCTransitionFlipY* CCTransitionFlipY::create(float t, CCScene* s)
{
    return CCTransitionFlipY::create(t, s, kCCTransitionOrientationUpOver);
}

//
// FlipAngular Transition
//
CCTransitionFlipAngular::CCTransitionFlipAngular()
{
}

CCTransitionFlipAngular::~CCTransitionFlipAngular()
{
}

void CCTransitionFlipAngular::onEnter()
{
    CCTransitionSceneOriented::onEnter();
}

CCTransitionFlipAngular* CCTransitionFlipAngular::create(float t, CCScene* s, tOrientation o)
{
    CCTransitionFlipAngular* pScene = new CCTransitionFlipAngular();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

CCTransitionFlipAngular* CCTransitionFlipAngular::create(float t, CCScene* s)
{
    return CCTransitionFlipAngular::create(t, s, kCCTransitionOrientationRightOver);
}

//
// ZoomFlipX Transition
//
CCTransitionZoomFlipX::CCTransitionZoomFlipX()
{
}
CCTransitionZoomFlipX::~CCTransitionZoomFlipX()
{
}

void CCTransitionZoomFlipX::onEnter()
{
    CCTransitionSceneOriented::onEnter();
}

CCTransitionZoomFlipX* CCTransitionZoomFlipX::create(float t, CCScene* s, tOrientation o)
{
    CCTransitionZoomFlipX* pScene = new CCTransitionZoomFlipX();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

CCTransitionZoomFlipX* CCTransitionZoomFlipX::create(float t, CCScene* s)
{
    return CCTransitionZoomFlipX::create(t, s, kCCTransitionOrientationRightOver);
}

//
// ZoomFlipY Transition
//
CCTransitionZoomFlipY::CCTransitionZoomFlipY()
{
}

CCTransitionZoomFlipY::~CCTransitionZoomFlipY()
{
}

void CCTransitionZoomFlipY::onEnter()
{
    CCTransitionSceneOriented::onEnter();
}

CCTransitionZoomFlipY* CCTransitionZoomFlipY::create(float t, CCScene* s, tOrientation o)
{
    CCTransitionZoomFlipY* pScene = new CCTransitionZoomFlipY();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

CCTransitionZoomFlipY* CCTransitionZoomFlipY::create(float t, CCScene* s)
{
    return CCTransitionZoomFlipY::create(t, s, kCCTransitionOrientationUpOver);
}

//
// ZoomFlipAngular Transition
//
CCTransitionZoomFlipAngular::CCTransitionZoomFlipAngular()
{
}
CCTransitionZoomFlipAngular::~CCTransitionZoomFlipAngular()
{
}


void CCTransitionZoomFlipAngular::onEnter()
{
    CCTransitionSceneOriented::onEnter();
}

CCTransitionZoomFlipAngular* CCTransitionZoomFlipAngular::create(float t, CCScene* s, tOrientation o)
{
    CCTransitionZoomFlipAngular* pScene = new CCTransitionZoomFlipAngular();
    pScene->initWithDuration(t, s, o);
    pScene->autorelease();

    return pScene;
}

CCTransitionZoomFlipAngular* CCTransitionZoomFlipAngular::create(float t, CCScene* s)
{
    return CCTransitionZoomFlipAngular::create(t, s, kCCTransitionOrientationRightOver);
}

//
// Fade Transition
//
CCTransitionFade::CCTransitionFade()
{
}
CCTransitionFade::~CCTransitionFade()
{
}

CCTransitionFade * CCTransitionFade::create(float duration, CCScene *scene, const ccColor3B& color)
{
    CCTransitionFade * pTransition = new CCTransitionFade();
    pTransition->initWithDuration(duration, scene, color);
    pTransition->autorelease();
    return pTransition;
}

CCTransitionFade* CCTransitionFade::create(float duration,CCScene* scene)
{
    return CCTransitionFade::create(duration, scene, ccBLACK);
}

bool CCTransitionFade::initWithDuration(float duration, CCScene *scene, const ccColor3B& color)
{
    if (CCTransitionScene::initWithDuration(duration, scene))
    {
        m_tColor.r = color.r;
        m_tColor.g = color.g;
        m_tColor.b = color.b;
        m_tColor.a = 0;
    }
    return true;
}

bool CCTransitionFade::initWithDuration(float t, CCScene *scene)
{
    this->initWithDuration(t, scene, ccBLACK);
    return true;
}

void CCTransitionFade :: onEnter()
{
    CCTransitionScene::onEnter();
}

void CCTransitionFade::onExit()
{
    CCTransitionScene::onExit();
    this->removeChildByTag(kSceneFade, false);
}

//
// Cross Fade Transition
//
CCTransitionCrossFade::CCTransitionCrossFade()
{
}
CCTransitionCrossFade::~CCTransitionCrossFade()
{
}

CCTransitionCrossFade* CCTransitionCrossFade::create(float t, CCScene* scene)
{
    CCTransitionCrossFade* pScene = new CCTransitionCrossFade();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionCrossFade:: draw()
{
    // override draw since both scenes (textures) are rendered in 1 scene
}

void CCTransitionCrossFade::onEnter()
{
    CCTransitionScene::onEnter();
}

// clean up on exit
void CCTransitionCrossFade::onExit()
{
    // remove our layer and release all containing objects 
    this->removeChildByTag(kSceneFade, false);
    CCTransitionScene::onExit();
}

//
// TurnOffTilesTransition
//
CCTransitionTurnOffTiles::CCTransitionTurnOffTiles()
{
}

CCTransitionTurnOffTiles::~CCTransitionTurnOffTiles()
{
}

CCTransitionTurnOffTiles* CCTransitionTurnOffTiles::create(float t, CCScene* scene)
{
    CCTransitionTurnOffTiles* pScene = new CCTransitionTurnOffTiles();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

// override addScenes, and change the order
void CCTransitionTurnOffTiles::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void CCTransitionTurnOffTiles::onEnter()
{
    CCTransitionScene::onEnter();
}

//
// SplitCols Transition
//
CCTransitionSplitCols::CCTransitionSplitCols()
{
}
CCTransitionSplitCols::~CCTransitionSplitCols()
{
}

CCTransitionSplitCols* CCTransitionSplitCols::create(float t, CCScene* scene)
{
    CCTransitionSplitCols* pScene = new CCTransitionSplitCols();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionSplitCols::onEnter()
{
    CCTransitionScene::onEnter();
}

//
// SplitRows Transition
//
CCTransitionSplitRows::CCTransitionSplitRows()
{
}

CCTransitionSplitRows::~CCTransitionSplitRows()
{
}

CCTransitionSplitRows* CCTransitionSplitRows::create(float t, CCScene* scene)
{
    CCTransitionSplitRows* pScene = new CCTransitionSplitRows();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

//
// FadeTR Transition
//
CCTransitionFadeTR::CCTransitionFadeTR()
{
}
CCTransitionFadeTR::~CCTransitionFadeTR()
{
}

CCTransitionFadeTR* CCTransitionFadeTR::create(float t, CCScene* scene)
{
    CCTransitionFadeTR* pScene = new CCTransitionFadeTR();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

void CCTransitionFadeTR::sceneOrder()
{
    m_bIsInSceneOnTop = false;
}

void CCTransitionFadeTR::onEnter()
{
    CCTransitionScene::onEnter();
}


//
// FadeBL Transition
//

CCTransitionFadeBL::CCTransitionFadeBL()
{
}
CCTransitionFadeBL::~CCTransitionFadeBL()
{
}

CCTransitionFadeBL* CCTransitionFadeBL::create(float t, CCScene* scene)
{
    CCTransitionFadeBL* pScene = new CCTransitionFadeBL();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

//
// FadeUp Transition
//
CCTransitionFadeUp::CCTransitionFadeUp()
{
}

CCTransitionFadeUp::~CCTransitionFadeUp()
{
}

CCTransitionFadeUp* CCTransitionFadeUp::create(float t, CCScene* scene)
{
    CCTransitionFadeUp* pScene = new CCTransitionFadeUp();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}

//
// FadeDown Transition
//
CCTransitionFadeDown::CCTransitionFadeDown()
{
}
CCTransitionFadeDown::~CCTransitionFadeDown()
{
}

CCTransitionFadeDown* CCTransitionFadeDown::create(float t, CCScene* scene)
{
    CCTransitionFadeDown* pScene = new CCTransitionFadeDown();
    if(pScene && pScene->initWithDuration(t, scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return NULL;
}


NS_CC_END
