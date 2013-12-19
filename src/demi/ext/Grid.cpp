
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "Color.h"
#include "Grid.h"
#include "GfxDriver.h"

namespace Demi 
{
    DiGridPlane::DiGridPlane( UINT size, float cellSize )
        :DiTransformUnit(),DiRenderUnit()
    {
        DiColor c1(0.47f,0.54f,0.64f);
        DiColor c2(0.37f,0.42f,0.78f);
        BuildGrid(size,cellSize,c1,c2);
    }

    DiGridPlane::DiGridPlane( UINT size, float cellSize, const DiColor& color1, const DiColor& color2 )
    {
        BuildGrid(size,cellSize,color1,color2);
    }

    DiGridPlane::~DiGridPlane( void )
    {
        ReleaseSourceData();
        ReleaseVertexDeclaration();
    }

    void DiGridPlane::GetWorldTransform( DiMat4* xform ) const
    {
        *xform = GetTransform();
    }
    const DiAABB& DiGridPlane::GetBoundingBox( void ) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiGridPlane::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(this);
    }

    void DiGridPlane::CullingUpdate( DiRenderBatchGroup* group,DiCamera* )
    {
        group->AddRenderUnit(this);
    }

    void DiGridPlane::BuildGrid( UINT size, float cellSize, const DiColor& c1, const DiColor& c2 )
    {
        const UINT32 numColLines = size*2 + 1;
        const UINT32 numLines    = numColLines*2;
        const UINT32 numVerts    = numLines*2;

        mPrimitiveType = PT_LINELIST;
        mPrimitiveCount = numLines;
        mVerticesNum = numVerts;

        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,  VERT_USAGE_COLOR);
        mVertexDecl->Create();

        USHORT vertSize = sizeof(float)*3 + sizeof(ARGB);

        DiVertexBuffer* vb = Driver->CreateVertexBuffer();
        mSourceData.push_back(vb);

        vb->Create(vertSize * numVerts);
        vb->SetStride(vertSize);

        BYTE* buffer = (BYTE*) vb->Lock(0,vertSize * numVerts);
        float* positions = (float*)buffer;
        UINT8* colors = buffer + sizeof(float)*3;

        ARGB color2 = c2.GetAsARGB();
        ARGB color1 = c1.GetAsARGB();

        const float radius   = size*cellSize;
        const float diameter = radius*2;
        for(UINT c = 0; c<numColLines; c++)
        {
            float *p0    = (float*)positions;
            UINT8  *c0    = (UINT8*)colors;
            positions    = (float*)(((UINT8*)positions)+vertSize);
            colors        = ((UINT8*)colors)+vertSize;
            float *p1    = (float*)positions;
            UINT8  *c1    = (UINT8*)colors;
            positions    = (float*)(((UINT8*)positions)+vertSize);
            colors        = ((UINT8*)colors)+vertSize;

            const float t = c / (float)(numColLines-1);
            const float d = diameter*t - radius;

            UINT *col1 = (UINT *)c0;
            UINT *col2 = (UINT *)c1;
            if(d==0.0f) 
            {
                *col1 = color1;
                *col2 = color1;
            }
            else        
            {
                *col1 = color2;
                *col2 = color2;
            }
            p0[0] = -radius; p0[1] = 0; p0[2] = d;
            p1[0] =  radius; p1[1] = 0; p1[2] = d;
        }
        for(UINT r=0; r<numColLines; r++)
        {
            float *p0 = (float*)positions;
            UINT8  *c0 = (UINT8*)colors;
            positions = (float*)(((UINT8*)positions)+vertSize);
            colors    = ((UINT8*)colors)+vertSize;
            float *p1 = (float*)positions;
            UINT8  *c1 = (UINT8*)colors;
            positions = (float*)(((UINT8*)positions)+vertSize);
            colors    = ((UINT8*)colors)+vertSize;

            const float t = r / (float)(numColLines-1);
            const float d = diameter*t - radius;
            UINT *col1 = (UINT *)c0;
            UINT *col2 = (UINT *)c1;
            if(d==0.0f) 
            {
                *col1 = color1;
                *col2 = color1;
            }
            else        
            {
                *col1 = color2;
                *col2 = color2;
            }

            p0[0] = d; p0[1] = 0; p0[2] = -radius;
            p1[0] = d; p1[1] = 0; p1[2] =  radius;
        }

        vb->Unlock();
        SetMaterial("unlighting_color.mtl");
    }

    DiString& DiGridPlane::GetType()
    {
        static DiString type = "GridPlane";
        return type;
    }
}