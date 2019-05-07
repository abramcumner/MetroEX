#pragma once

#include "../MetroTypes.h"
#include "Block.h"
#include <array>

class MetroReflectionReader;

struct ScriptGroup {
    void Serialize(MetroReflectionReader& r);
};

struct ScriptBlocks {
    uint16_t       version;
    uint32_t       block_count;
    MyArray<Block> blocks;

    void Serialize(MetroReflectionReader& r);
};

struct Script {
    MyArray<ScriptGroup> groups;
    ScriptBlocks         blocks;
    uint32_t             link_count;
    MyArray<vec4s16>     links;

    void Serialize(MetroReflectionReader& r);
};

struct ScriptRef {
    CharString     vs_name;
    bool           vs_debug;
    bool           vs_active;
    bool           disable_qsave;
    bool           save_on_nextlevel;
    CharString     vs_ref;
    bool           vs_ref_dyn_state_exist;
    MyArray<BlockRef> exposed_blocks;

    void Serialize(MetroReflectionReader& r);
};
