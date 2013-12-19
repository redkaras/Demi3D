
/********************************************************************
    File:       SubMesh.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "VertexDeclaration.h"

namespace Demi 
{
    struct DiBoneWeight
    {
        uint32    vertexIndex;
        uint16    boneIndex;
        float    weight;
    };

    class DI_GFX_API DiSubMesh : public DiBase
    {
    public:

        DiSubMesh(DiMesh* parent);

        virtual ~DiSubMesh();

        friend class DiMesh;
        friend class DiSubModel;
        friend class DiMeshSerializerImpl;

    public:

        struct SourceData
        {
            SourceData()
            {
                data        = NULL;
                numVertices = 0;
                stride         = 0;
                stream         = 0;
            }

            SourceData(void* d,uint32 n, uint16 s,uint16 str)
            {
                data        = d;
                numVertices = n;
                stride        = s;
                stream        = str;
            }

            uint32    GetSize()
            {
                return numVertices * stride;
            }

            void*    data;
            uint32    numVertices;
            uint16    stride;
            uint16    stream;
        };

        typedef DiMap<uint32,SourceData>                SourceDataList;
        typedef DiConstMapIterator<SourceDataList>        SourceDataIterator;
        typedef std::multimap<uint32, DiBoneWeight>        BoneWeightList;
        typedef DiVector<uint16>                        IndexMap;


        void                    SetPrimitiveType(DiPrimitiveType pt){ mPrimitiveType = pt; }

        DiPrimitiveType         GetPrimitiveType()const{ return mPrimitiveType; }

        uint32                  GetPrimitiveCount()const {return mPrimitiveCount;}

        void                    SetVerticeNum(uint32 vnum);

        uint32                  GetVerticeNum() const{return mVerticesNum;}

        void                    SetPrimitiveCount(uint32 count){mPrimitiveCount = count;}

        void                    ComputePrimitiveCount();

        void                    SetMaterialName(const DiString& material);

        const DiString&         GetMaterialName()const{return mMaterialName;}

        void*                   CreateSourceData(uint16 streamid, uint32 num, uint16 stride );

        void                    CreateSourceData(uint16 streamid, uint32 num, uint16 stride, void* data );

        void                    ReleaseSourceData(uint16 streamid);

        SourceData              GetSourceData(size_t ID);

        void*                   CreateIndexData(int indexNum,BOOL use32Bit);

        uint32                  GetIndexSize() const;

        BOOL                    GetUse32BitIndex() const { return m32BitIndex; }

        uint32                  GetIndexNum()const{return mIndicsNum;}

        void*                   GetIndexData() const{return mIndexData;}

        SourceDataIterator      GetSourceDataIterator() const;

        void                    ReleaseVertexData();

        void                    ReleaseIndexData();

        DiMesh*                 GetParentMesh() const { return mMesh; }

        const BoneWeightList&   GetBoneWeights() { return mBoneWeights; }

        void                    SetupBoneWeights();

        void                    AddWeight(uint32 vertexId, uint16 boneId, float weight);

        const DiVertexElements& GetVertexElements() const { return mVertexElems; }

        DiVertexElements&       GetVertexElements() { return mVertexElems; }

        uint32                  GetMaxWeights() const { return mMaxWeights; }

        uint16                  RationaliseBoneWeights();

        void                    BuildIndexMap(IndexMap& boneIndexToBlendIndexMap, IndexMap& blendIndexToBoneIndexMap);

        IndexMap&               GetBlendIndexToBoneIndexMap() { return mBlendIndexToBoneIndexMap; }

        const IndexMap&         GetBlendIndexToBoneIndexMap() const{ return mBlendIndexToBoneIndexMap; }

        uint32                  GetIndex() const { return mIndex; }

    protected:

        void                    SetupBoneWeightsData(uint16 numBlendWeightsPerVertex);

    protected:

        DiMesh*                   mMesh;

        // the index in this submesh's parent mesh
        uint32                    mIndex;
        
        SourceDataList            mVertexData;

        uint32                    mVerticesNum;

        void*                     mIndexData;

        uint32                    mIndicsNum;
        
        BOOL                      m32BitIndex;
        
        DiPrimitiveType           mPrimitiveType;

        uint32                    mPrimitiveCount;

        DiString                  mMaterialName;

        BoneWeightList            mBoneWeights;

        DiVertexElements          mVertexElems;

        IndexMap                  mBlendIndexToBoneIndexMap;
        
        uint32                    mMaxWeights;
    };
}