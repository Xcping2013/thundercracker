/* -*- mode: C; c-basic-offset: 4; intent-tabs-mode: nil -*-
 *
 * Copyright <c> 2012 Sifteo, Inc. All rights reserved.
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "CubeWrapper.h"
#include <sifteo/asset.h>
#include "Config.h"
#include "assets.gen.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

using namespace Sifteo;

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace Buddies {

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const AssetImage &GetBuddyFaceBackgroundAsset(int buddyId)
{
    switch (buddyId)
    {
        default:
        case 0: return BuddyFaceBackground0;
        case 1: return BuddyFaceBackground1;
        case 2: return BuddyFaceBackground2;
        case 3: return BuddyFaceBackground3;
        case 4: return BuddyFaceBackground4;
        case 5: return BuddyFaceBackground5;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const PinnedAssetImage &GetBuddyFacePartsAsset(int buddyId)
{
    switch (buddyId)
    {
        default:
        case 0: return BuddyFaceParts0;
        case 1: return BuddyFaceParts1;
        case 2: return BuddyFaceParts2;
        case 3: return BuddyFaceParts3;
        case 4: return BuddyFaceParts4;
        case 5: return BuddyFaceParts5;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Vec2 GetHintBarPoint(Cube::Side side)
{
    ASSERT(side >= 0 && side < NUM_SIDES);
    
    switch (side)
    {
        default:
        case SIDE_TOP:    return Vec2( 0,  0);
        case SIDE_LEFT:   return Vec2( 0,  0);
        case SIDE_BOTTOM: return Vec2( 0, 11);
        case SIDE_RIGHT:  return Vec2(11,  0);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const AssetImage &GetHintBarAsset(Cube::ID cubeId, Cube::Side side)
{
    ASSERT(side >= 0 && side < NUM_SIDES);
    
    switch (side)
    {
        default:
        case SIDE_TOP:    return cubeId == 0 ? HintBarBlueTop    : HintBarOrangeTop;
        case SIDE_LEFT:   return cubeId == 0 ? HintBarBlueLeft   : HintBarOrangeLeft;
        case SIDE_BOTTOM: return cubeId == 0 ? HintBarBlueBottom : HintBarOrangeBottom;
        case SIDE_RIGHT:  return cubeId == 0 ? HintBarBlueRight  : HintBarOrangeRight;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const Vec2 kPartPositions[NUM_SIDES] =
{
    Vec2(32, -8),
    Vec2(-8, 32),
    Vec2(32, 72),
    Vec2(72, 32),
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CubeWrapper::CubeWrapper()
    : mCube()
    , mBg1Helper(mCube)
    , mEnabled(false)
    , mBuddyId(0)
    , mPieces()
    , mPiecesSolution()
    , mPieceOffsets()
    , mPieceAnimT(0.0f)
    , mCutsceneSpriteJumpRandom()
    , mCutsceneSpriteJump0(false)
    , mCutsceneSpriteJump1(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Reset()
{
    for (unsigned int i = 0; i < NUM_SIDES; ++i)
    {
        mPieces[i] = Piece();
        mPiecesSolution[i] = Piece();
        mPieceOffsets[i] = 0;
    }
    mPieceAnimT = 0.0f;
    mCutsceneSpriteJump0 = false;
    mCutsceneSpriteJump1 = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Update(float dt)
{
    mPieceAnimT += dt;
    while (mPieceAnimT > kPieceAnimPeriod)
    {
        mPieceAnimT -= kPieceAnimPeriod;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawBuddy()
{
    ASSERT(IsEnabled());
    
    Video().BG0_drawAsset(Vec2(0, 0), GetBuddyFaceBackgroundAsset(mBuddyId));
    
    DrawPiece(mPieces[SIDE_TOP], SIDE_TOP);
    DrawPiece(mPieces[SIDE_LEFT], SIDE_LEFT);
    DrawPiece(mPieces[SIDE_BOTTOM], SIDE_BOTTOM);
    DrawPiece(mPieces[SIDE_RIGHT], SIDE_RIGHT);
}  

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawBuddyWithStoryHint(Sifteo::Cube::Side side, bool blink)
{
    ASSERT(IsEnabled());
    
    Video().BG0_drawAsset(Vec2(0, 0), GetBuddyFaceBackgroundAsset(mBuddyId));
    
    for (unsigned int i = 0; i < NUM_SIDES; ++i)
    {
        if (side != int(i) || !blink)
        {
            DrawPiece(mPieces[i], i);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawOverlay(const Sifteo::AssetImage &asset, const char *text)
{
    mBg1Helper.DrawAsset(Vec2(0, 3), asset);
    
    if (text != NULL)
    {
        mBg1Helper.DrawText(Vec2(2, 4), Font, text);
    }
    
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawTextBanner(const char *text)
{
    ASSERT(text != NULL);
    
    mBg1Helper.DrawAsset(Vec2(0, 0), BannerEmpty);
    mBg1Helper.DrawText(Vec2(0, 0), Font, text);
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawBanner(const Sifteo::AssetImage &asset)
{
    mBg1Helper.DrawAsset(Vec2(0, 0), asset);
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawScoreBanner(const Sifteo::AssetImage &asset, int minutes, int seconds)
{
    mBg1Helper.DrawAsset(Vec2(0, 0), asset); // Banner Background
    
    const AssetImage &font = mCube.id() == 0 ? FontScoreBlue : FontScoreOrange;
    
    int x = 11;
    int y = 0;
    mBg1Helper.DrawAsset(Vec2(x++, y), font, minutes / 10); // Mintues (10s)
    mBg1Helper.DrawAsset(Vec2(x++, y), font, minutes % 10); // Minutes ( 1s)
    mBg1Helper.DrawAsset(Vec2(x++, y), font, 10); // ":"
    mBg1Helper.DrawAsset(Vec2(x++, y), font, seconds / 10); // Seconds (10s)
    mBg1Helper.DrawAsset(Vec2(x++, y), font, seconds % 10); // Seconds ( 1s)
    
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawHintBar(Cube::Side side)
{
    ASSERT(side >= 0 && side < NUM_SIDES);
    
    mBg1Helper.DrawAsset(GetHintBarPoint(side), GetHintBarAsset(mCube.id(), side));
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawBackground(const Sifteo::AssetImage &asset)
{
    Video().BG0_drawAsset(Vec2(0, 0), asset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawBackgroundWithText(
    const Sifteo::AssetImage &asset,
    const char *text, const Sifteo::Vec2 &textPosition)
{
    ASSERT(text != NULL);
    
    Video().BG0_drawAsset(Vec2(0, 0), asset);
    
    mBg1Helper.DrawText(textPosition, Font, text);
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::UpdateCutscene()
{
    UpdateCutsceneSpriteJump(mCutsceneSpriteJump0, 8, 1);
    UpdateCutsceneSpriteJump(mCutsceneSpriteJump1, 16, 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawCutscene(const char *text)
{
    ASSERT(text != NULL);
    ASSERT(2 <= _SYS_VRAM_SPRITES);
    
    Video().BG0_drawAsset(Vec2(0, 0), CutsceneBackground);
    
    Video().setSpriteImage(0, CutsceneSprites, 0);
    Video().setSpriteImage(1, CutsceneSprites, 1);
    
    Video().moveSprite(0, Vec2( 0, mCutsceneSpriteJump0 ? 64 : 72));
    Video().moveSprite(1, Vec2(64, mCutsceneSpriteJump1 ? 64 : 72));
    
    mBg1Helper.DrawAsset(Vec2(0, 0), CutsceneTextBubble);
    mBg1Helper.DrawText(Vec2(1, 1), Font, text);
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Clear()
{
    Video().set();
    Video().clear();
    
    for (int i = 0; i < _SYS_VRAM_SPRITES; ++i)
    {
        Video().hideSprite(i);
    }
    
    mBg1Helper.Clear();
    mBg1Helper.Flush();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::NeedsPaintSync()
{
    return mBg1Helper.NeedFinish();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsLoadingAssets()
{
    ASSERT(IsEnabled());
    
    return mCube.assetDone(GameAssets);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::LoadAssets()
{
    ASSERT(IsEnabled());
    
    mCube.loadAssets(GameAssets);
    
    VidMode_BG0_ROM rom(mCube.vbuf);
    rom.init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawLoadingAssets()
{
    ASSERT(IsEnabled());
    
    VidMode_BG0_ROM rom(mCube.vbuf);
    rom.BG0_progressBar(Vec2(0, 0), mCube.assetProgress(GameAssets, VidMode_BG0::LCD_width), 16);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Sifteo::Cube::ID CubeWrapper::GetId()
{
    return mCube.id();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsEnabled() const
{
    return mEnabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Enable(Cube::ID cubeId, unsigned int buddyId)
{
    ASSERT(!IsEnabled());
    
    mCube.enable(cubeId);
    
    // This ensure proper video state is set, even if we have kLoadAssets = false.
    Video().setWindow(0, VidMode::LCD_height);
    
    mEnabled = true;
    mBuddyId = buddyId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::Disable()
{
    ASSERT(IsEnabled());
    
    mEnabled = false;
    mCube.disable();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const Piece &CubeWrapper::GetPiece(Cube::Side side) const
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    return mPieces[side];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::SetPiece(Cube::Side side, const Piece &piece)
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    mPieces[side] = piece;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

const Piece &CubeWrapper::GetPieceSolution(Cube::Side side) const
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    return mPiecesSolution[side];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::SetPieceSolution(Cube::Side side, const Piece &piece)
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    mPiecesSolution[side] = piece;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

int CubeWrapper::GetPieceOffset(Cube::Side side) const
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    return mPieceOffsets[side];
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::SetPieceOffset(Cube::Side side, int offset)
{
    ASSERT(side >= 0 && side < int(arraysize(mPieceOffsets)));
    
    mPieceOffsets[side] = offset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsSolved() const
{
    ASSERT(IsEnabled());
    
    for (unsigned int i = 0; i < arraysize(mPiecesSolution); ++i)
    {
        if (mPiecesSolution[i].mMustSolve)
        {
            if (mPieces[i].mBuddy != mPiecesSolution[i].mBuddy ||
                mPieces[i].mPart != mPiecesSolution[i].mPart)
            {
                return false;
            }
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

bool CubeWrapper::IsTouching() const
{
#ifdef SIFTEO_SIMULATOR
    return mCube.touching();
#else
    return false;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Sifteo::VidMode_BG0_SPR_BG1 CubeWrapper::Video()
{
    return VidMode_BG0_SPR_BG1(mCube.vbuf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::DrawPiece(const Piece &piece, Cube::Side side)
{
    ASSERT(piece.mPart >= 0 && piece.mPart < NUM_SIDES);
    ASSERT(side >= 0 && side < NUM_SIDES);
    ASSERT((NUM_SIDES * 2) <= _SYS_VRAM_SPRITES);
    
    int spriteLayer0 = side;
    int spriteLayer1 = side + NUM_SIDES;
    
    if (piece.mAttribute == Piece::ATTR_FIXED)
    {
        Video().setSpriteImage(spriteLayer0, BuddyFacePartFixed);
    }
    else
    {
        Video().hideSprite(spriteLayer0);
    }
    
    if (piece.mAttribute == Piece::ATTR_HIDDEN)
    {
        Video().setSpriteImage(spriteLayer1, BuddyFacePartHidden);
    }
    else
    {
        int rotation = side - piece.mPart;
        if (rotation < 0)
        {
            rotation += NUM_SIDES;
        }
        
        const Sifteo::PinnedAssetImage &asset = GetBuddyFacePartsAsset(piece.mBuddy);
        unsigned int frame = (rotation * NUM_SIDES) + piece.mPart;
        
        ASSERT(frame < asset.frames);
        Video().setSpriteImage(spriteLayer1, asset, frame);
    }
    
    Vec2 point = kPartPositions[side];
    
    switch(side)
    {
        case SIDE_TOP:
        {
            point.y += mPieceOffsets[side];
            break;
        }
        case SIDE_LEFT:
        {
            point.x += mPieceOffsets[side];
            break;
        }
        case SIDE_BOTTOM:
        {
            point.y -= mPieceOffsets[side];
            break;
        }
        case SIDE_RIGHT:
        {
            point.x -= mPieceOffsets[side];
            break;
        }
    }
    
    ASSERT(kPieceAnimPeriod > 0.0f);
    float w = 2.0f * M_PI / kPieceAnimPeriod;
    float x = kPieceAnimX * cosf(w * mPieceAnimT);
    float y = kPieceAnimY * sinf(w * mPieceAnimT);
    
    point.x += int(x);
    point.y += int(y);
    
    Video().moveSprite(spriteLayer0, point);
    Video().moveSprite(spriteLayer1, point);
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void CubeWrapper::UpdateCutsceneSpriteJump(bool &cutsceneSpriteJump, int upChance, int downChance)
{
    if (!cutsceneSpriteJump)
    {
        if (mCutsceneSpriteJumpRandom.randrange(upChance) == 0)
        {
            cutsceneSpriteJump = true;
        }
    }
    else
    {
        if (mCutsceneSpriteJumpRandom.randrange(downChance) == 0)
        {
            cutsceneSpriteJump = false;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

}
