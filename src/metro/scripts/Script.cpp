#include "Script.h"
#include "../MetroReflection.h"
#include "Block.h"

void ScriptGroup::Serialize(MetroReflectionReader& r) {
    assert(false);
}

void ScriptBlocks::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, version);
    METRO_READ_MEMBER(r, block_count);
    if (block_count == ~0u) {
        r.VerifyTypeInfo("count", MetroTypeGetAlias<uint32_t>());
        r >> block_count;
    }
    blocks.resize(block_count);
    for (uint32_t i = 0; i != block_count; i++) {
        auto s = r.OpenSection(kEmptyString, true, true);
        s >> blocks[i];
        r.CloseSection(s);
    }
}

void Script::Serialize(MetroReflectionReader& r) {
    METRO_READ_CHILD_STRUCT_ARRAY(r, groups);
    METRO_READ_CHILD_STRUCT(r, blocks);
    METRO_READ_MEMBER(r, link_count);
    links.resize(link_count);
    for (uint32_t i = 0; i != link_count; i++) {
        r >> links[i];
    }
}

void ScriptRef::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, vs_name);
    METRO_READ_MEMBER(r, vs_debug);
    METRO_READ_MEMBER(r, vs_active);
    METRO_READ_MEMBER(r, disable_qsave);
    METRO_READ_MEMBER(r, save_on_nextlevel);
    METRO_READ_MEMBER(r, vs_ref);
    METRO_READ_MEMBER(r, vs_ref_dyn_state_exist);
    assert(vs_ref_dyn_state_exist == false);
    if (!vs_ref.empty()) {
        METRO_READ_CHILD_STRUCT_ARRAY(r, exposed_blocks);
    }
}
