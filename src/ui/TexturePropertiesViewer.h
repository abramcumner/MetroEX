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
            String^ get() { return marshal_as<String^>(this->mTextureInfo->name); }
        };

        property uint8_t Flags {
            uint8_t get() { return this->mTextureInfo->flags; }
        }

        property uint32_t Type {
            uint32_t get() { return this->mTextureInfo->type; }
        }

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
            vec4 get() { return this->mTextureInfo->surf_xform; }
        }

        property uint32_t Format {
            uint32_t get() { return this->mTextureInfo->format; }
        }

        property uint32_t Width {
            uint32_t get() { return this->mTextureInfo->width; }
        }

        property uint32_t Height {
            uint32_t get() { return this->mTextureInfo->height; }
        };

        property bool Animated {
            bool get() { return this->mTextureInfo->animated; }
        };

        property String^ RealPath {
            String^ get() { return this->mRealPath; }
        };
    };
}
