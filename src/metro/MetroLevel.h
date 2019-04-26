#pragma once
#include "MetroTypes.h"

PACKED_STRUCT_BEGIN
struct GeomObjectInfo {     // size = 28
    uint32_t    vertexType;
    uint32_t    vbOffset;
    uint32_t    numVertices;
    uint32_t    numShadowVertices;
    uint32_t    ibOffset;
    uint32_t    numIndices;
    uint32_t    numShadowIndices;
} PACKED_STRUCT_END;

class MetroLevel {
public:
    MetroLevel();
    ~MetroLevel();

    bool                LoadFromData(const uint8_t* data, const size_t length, const size_t fileIdx);

    size_t              GetNumMeshes() const;
    const MetroMesh*    GetMesh(const size_t idx) const;

private:
    void                ReadGeometryDescription(MemStream& stream, MyArray<GeomObjectInfo>& infos);
    void                ReadGeomObjectInfo(MemStream& stream, GeomObjectInfo& info);
    void                ReadLevelGeometry(MemStream& stream, const MyArray<GeomObjectInfo>& infos);

private:
    MyArray<MetroMesh*>   mMeshes;
};
