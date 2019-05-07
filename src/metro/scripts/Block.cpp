#include "Block.h"
#include "../MetroReflection.h"
#include "BlockFactory.h"

Block::Block() {
}

Block::Block(uint32_t clsid, const char* name, const MetaInfo* meta)
    : clsid(clsid)
    , name(name)
    , meta(meta) {
}

#define READ_MEMBER_V(s, type)                                \
    {                                                         \
        type v;                                               \
        s.VerifyTypeInfo(el.name, MetroTypeGetAlias<type>()); \
        s >> v;                                               \
        value = v;                                            \
    }
#define READ_MEMBER_EXT_V(s, ext, type)                       \
    {                                                         \
        type v;                                               \
        s.ReadEditorTag(el.name);                             \
        s.VerifyTypeInfo(el.name, MetroTypeGetAlias<type>()); \
        s >> v;                                               \
        value = v;                                            \
    }
#define READ_ARRAY_MEMBER_V(s, type)                                                                  \
    {                                                                                                 \
        type v;                                                                                       \
        s.VerifyTypeInfo(el.name, MetroTypeArrayGetAlias<ArrayElementTypeGetter<type>::elem_type>()); \
        s >> v;                                                                                       \
        value = v;                                                                                    \
    }

void Block::Read(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, posx);
    METRO_READ_MEMBER(r, posy);
    if (meta) {
        for (const auto& el : meta->props) {
            ParamValue value;
            switch (el.type) {
            case Type_u16:
                READ_MEMBER_V(r, uint16_t)
                break;
            case Type_bool:
                READ_MEMBER_V(r, bool)
                break;
            case Type_bool8:
                READ_MEMBER_V(r, Flags8)
                break;
            case Type_time:
                READ_MEMBER_V(r, MetroTime)
                break;
            case Type_anim:
                READ_MEMBER_V(r, AnimationString)
                break;
            case Type_choose:
                READ_MEMBER_EXT_V(r, choose, CharString)
                break;
            case Type_color_u32:
                READ_MEMBER_V(r, ColorU32)
                break;
            case Type_entity:
                READ_MEMBER_V(r, EntityLink)
                break;
            case Type_fp32:
                READ_MEMBER_V(r, float)
                break;
            case Type_sz:
                READ_MEMBER_V(r, CharString)
                break;
            case Type_part:
                READ_MEMBER_EXT_V(r, part_str, CharString)
                break;
            case Type_u8:
                READ_MEMBER_V(r, uint8_t)
                break;
            case Type_u32:
                READ_MEMBER_V(r, uint32_t)
                break;
            case Type_s32:
                READ_MEMBER_V(r, int32_t)
                break;
            case Type_color_vec4f:
                READ_MEMBER_V(r, color4f)
                break;
            case Type_vec2f:
                READ_MEMBER_V(r, vec2)
                break;
            case Type_u8_array:
                READ_ARRAY_MEMBER_V(r, U8Array)
                break;
            default:
                assert(false);
            }
            params.emplace_back(el.name, value);
        }
    } else {
        unknown.resize(r.GetStream().Remains());
        r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
    }
}

#undef READ_MEMBER_V

void Block::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, clsid);
    *this = BlockFactory::Create(clsid);
    Read(r);
}

void BlockRef::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, blkid);
    Block::Serialize(r);
}
