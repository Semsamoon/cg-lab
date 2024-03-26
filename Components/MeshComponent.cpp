#include "MeshComponent.h"

#include "../Lib/assimp/Importer.hpp"
#include "../Lib/assimp/scene.h"
#include "../Lib/assimp/Vertex.h"

using namespace components;

MeshComponent::MeshComponent(const aiMesh* mesh)
{
    for (uint32 i = 0; i < mesh->mNumVertices; i++)
    {
        VertexInputBufferElement params;

        params.position = float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->mTextureCoords[0])
            params.uv = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else params.uv = float2();

        if (mesh->mNormals != nullptr)
            params.normal = float3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        else params.normal = float3(1, 0, 0);

        vertex_buffer_.data.push_back(params);
    }

    for (uint32 i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];

        for (uint32 j = 0; j < face.mNumIndices; j++)
            index_buffer_.data.push_back(face.mIndices[j]);
    }
}
