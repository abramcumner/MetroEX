#pragma once

#include "../MetroTypes.h"
#include "BlockMeta.h"

class MetroReflectionReader;

struct Block {
    using Params = MyArray<std::pair<const char*, ParamValue>>;

    Block();
    Block(uint32_t clsid, const char* name, const MetaInfo* meta);

    uint32_t   clsid = 0;
    CharString name;
    int16_t   posx = 0;
    int16_t   posy = 0;
    Params     params;

    virtual void Read(MetroReflectionReader& cfg);
    void         Serialize(MetroReflectionReader& r);

    const MetaInfo* meta = nullptr;
    BytesArray      unknown;
};

struct BlockRef : public Block {
    uint16_t blkid = 0;

    void Serialize(MetroReflectionReader& r);
};
