#pragma once
#include "MetroTypes.h"

//#NOTE_SK: 256 bits, one per bone
PACKED_STRUCT_BEGIN
struct BonesBitset {
    uint32_t    dwords[8];
} PACKED_STRUCT_END;

class MetroMotion {
public:
    MetroMotion();
    ~MetroMotion();

    bool        LoadFromData(const void* data, const size_t dataLength);

private:
    // header
    size_t      mVersion;
    size_t      mBonesCRC;
    size_t      mNumBones;
    // info
    size_t      mFlags;
    float       mSpeed;
    float       mAccrue;
    float       mFalloff;
    size_t      mNumKeys;
    size_t      mJumpFrame;
    size_t      mLandFrame;
    BonesBitset mAffectedBones;
    size_t      mMotionsDataSize;
    size_t      mMotionsOffsetsSize;
    BonesBitset mHighQualityBones;
    // data
    BytesArray  mMotionsData;
};