
#include "D3D9VertexBuffer.h"
#include "D3D9Driver.h"
#include "D3D9StateCache.h"

namespace Demi
{
    DiD3D9VertexBuffer::DiD3D9VertexBuffer()
        : mVertexBuffer(nullptr)
    {
    }

    DiD3D9VertexBuffer::~DiD3D9VertexBuffer()
    {
        Release();
    }

    void DiD3D9VertexBuffer::Create(uint32 size, uint32 usage, uint16 streamID)
    {
        if (mVertexBuffer)
            Release();

        mResUsage = (DiResUsage)usage;
        mUsage = 0;
        mPool = D3DPOOL_MANAGED;
        if (usage & RU_DYNAMIC)
        {
            mUsage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
            mPool = D3DPOOL_DEFAULT;
        }
        mBufferSize = size;
        mStreamId = streamID;

        if (size > 0)
            mVertexBuffer = DiD3D9Driver::CreateVertexBuffer(size, mUsage, mPool);
        else
        {
            DI_ERROR("Cannot create the vertex buffer, check the size please.");
        }
    }

    void DiD3D9VertexBuffer::Bind()
    {
        if (mInstance == SOURCE_NO_INSTANCE)
            DiD3D9Driver::StateCache->SetStreamSourceFreq(mStreamId, 1);
        else if (mInstance == SOURCE_INSTANCE_INDEX)
            DiD3D9Driver::StateCache->SetStreamSourceFreq(mStreamId, D3DSTREAMSOURCE_INDEXEDDATA | mInstanceNum);
        else if (mInstance == SOURCE_INSTANCE_DATA)
            DiD3D9Driver::StateCache->SetStreamSourceFreq(mStreamId, DWORD(D3DSTREAMSOURCE_INSTANCEDATA | 1));

        if (mVertexBuffer)
            DiD3D9Driver::StateCache->SetStreamSource(mStreamId, mVertexBuffer, mOffset, mStride);
    }

    void DiD3D9VertexBuffer::Release()
    {
        if (mVertexBuffer)
        {
            mVertexBuffer->Release();
            mVertexBuffer = nullptr;
        }
    }

    void DiD3D9VertexBuffer::OnDeviceLost()
    {
        if (mPool != D3DPOOL_MANAGED && mVertexBuffer)
        {
            mVertexBuffer->Release();
            mVertexBuffer = nullptr;
            mDeviceLost = true;
        }
    }

    void DiD3D9VertexBuffer::OnDeviceReset()
    {
        if (mDeviceLost)
        {
            mVertexBuffer = DiD3D9Driver::CreateVertexBuffer(
                mBufferSize, mUsage, mPool);
            mDeviceLost = false;
        }
    }

    void* DiD3D9VertexBuffer::Lock(uint32 offset, uint32 size, uint32 flag)
    {
        void* lk = nullptr;
        if (mVertexBuffer)
            mVertexBuffer->Lock(offset, size, &lk, flag);
        return lk;
    }

    void DiD3D9VertexBuffer::Unlock()
    {
        if (mVertexBuffer)
            mVertexBuffer->Unlock();
    }
}