#pragma once
#include <msclr/marshal_cppstd.h>
#include "mycommon.h"
#include "metro\MetroTexturesDatabase.h"

namespace MetroEX {
    using namespace System;
    using namespace msclr::interop;
    using namespace System::ComponentModel;

    ref class TexturePropertiesViewer
    {
    public:
        TexturePropertiesViewer();

    private:
        const MetroTextureInfo* mTextureInfo;
        String^ mRealPath;

    public:
        void SetTextureInfo(const MetroTextureInfo* textureInfo) {
            mTextureInfo = textureInfo;
        }

        void SetRealPath(String^ path) {
            mRealPath = path;
        }

        [Category("Common")]
        [Description("Texture name")]
        property String^ Name {
            String^ get() { return marshal_as<String^>(mTextureInfo->name); }
        };

        [Category("Common")]
        [Description("Texture flags")]
        property String^ Flags {
            String^ get() { return Convert::ToString(mTextureInfo->flags, 2)->PadLeft(8, L'0'); }
        }

        [Category("Common")]
        [Description("Texture type")]
        property String^ Type {
            String^ get() {
                switch (safe_cast<MetroTextureInfo::TextureType>(mTextureInfo->type)) {
                case MetroTextureInfo::TextureType::Diffuse:
                    return "Diffuse";
                case MetroTextureInfo::TextureType::Detail_diffuse:
                    return "Detail_diffuse";
                case MetroTextureInfo::TextureType::Cubemap:
                    return "Cubemap";
                case MetroTextureInfo::TextureType::Cubemap_hdr:
                    return "Cubemap_hdr";
                case MetroTextureInfo::TextureType::Terrain:
                    return "Terrain";
                case MetroTextureInfo::TextureType::Bumpmap:
                    return "Bumpmap";
                case MetroTextureInfo::TextureType::Diffuse_va:
                    return "Diffuse_va";
                case MetroTextureInfo::TextureType::Arbitrary4:
                    return "Arbitrary4";
                case MetroTextureInfo::TextureType::Normalmap:
                    return "Normalmap";
                case MetroTextureInfo::TextureType::Normalmap_alpha:
                    return "Normalmap_alpha";
                case MetroTextureInfo::TextureType::Normalmap_detail:
                    return "Normalmap_detail";
                case MetroTextureInfo::TextureType::Unknown_01:
                    return "Unknown_01";
                case MetroTextureInfo::TextureType::Unknown_has_lum:
                    return "Unknown_has_lum";
                case MetroTextureInfo::TextureType::Instance:
                    return "Instance";
                default:
                    return "Unknown";
                }
            }
        }

        [Category("Common")]
        [Description("Texture source name")]
        property String^ SourceName {
            String^ get() {
                if (mTextureInfo->source_name.IsValidRef()) {
                    return String::Empty;
                } else {
                    return marshal_as<String^>(mTextureInfo->source_name.str);
                }
            }
        }

        property vec4 SurfaceXForm {
            vec4 get() { return mTextureInfo->surf_xform; }
        }

        property uint32_t Format {
            uint32_t get() { return mTextureInfo->format; }
        }

        [Category("Size")]
        [Description("Texture Width in pixels")]
        property uint32_t Width {
            uint32_t get() { return mTextureInfo->width; }
        }

        [Category("Size")]
        [Description("Texture Height in pixels")]
        property uint32_t Height {
            uint32_t get() { return mTextureInfo->height; }
        };

        property bool Animated {
            bool get() { return mTextureInfo->animated; }
        };

        [Category("Common")]
        [Description("Texture real path")]
        property String^ RealPath {
            String^ get() { return mRealPath; }
        };
    };
}
