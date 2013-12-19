
#include "RenderTarget.h"
#include "GfxDriver.h"
#include "Texture.h"
#include "DepthBuffer.h"
#include "GfxDriver.h"

#pragma warning(disable : 4706)

namespace Demi 
{
    DiRenderTarget::DiRenderTarget()
       :mWidth(0),
        mHeight(0),
        mParentTex(nullptr),
        mListener(nullptr),
        mShouldUpdate(true),
        mDepthBuffer(nullptr),
        mDepthClearValue(1.0f),
        mClearPerFrame(true),
        mClearColor(DiColor::Black),
        mDepthBufferPoolId(DiDepthBuffer::POOL_DEFAULT),
        mClearFlag(CLEAR_COLOR|CLEAR_DEPTH|CLEAR_STENCIL),
        mViewportDirty(true)
    {
        Driver->AddGpuResource(this);
    }

    DiRenderTarget::~DiRenderTarget()
    {
        if (Driver)
            Driver->RemoveGpuResource(this);

        DetachDepthBuffer();
    }

    void DiRenderTarget::OnDeviceLost() 
    {
    }

    void DiRenderTarget::AttachTexture( DiTexture* surface )
    {
        mParentTex = surface;
        mWidth = mParentTex->GetWidth();
        mHeight = mParentTex->GetHeight();
        mViewportDirty = true;
        AttachSurface();
    }

    void DiRenderTarget::DetachTexture()
    {
        mParentTex = nullptr;
        DetachSurface();
    }

    void DiRenderTarget::SetDepthBufferPool( uint16 poolId )
    {
        if( mDepthBufferPoolId != poolId )
        {
            mDepthBufferPoolId = poolId;    
            DetachDepthBuffer();
        }
    }

    uint16 DiRenderTarget::GetDepthBufferPool() const
    {
        return mDepthBufferPoolId;
    }

    DiDepthBuffer* DiRenderTarget::GetDepthBuffer() const
    {
        return mDepthBuffer;
    }

    bool DiRenderTarget::AttachDepthBuffer( DiDepthBuffer *depthBuffer )
    {
        bool retVal = false;

        if ((retVal = IsCompatibleWith(depthBuffer)))
        {
            DetachDepthBuffer();
            mDepthBuffer = depthBuffer;
            mDepthBuffer->NotifyRenderTargetAttached( this );
        }

        return retVal;
    }

    void DiRenderTarget::DetachDepthBuffer()
    {
        if( mDepthBuffer )
        {
            mDepthBuffer->NotifyRenderTargetDetached( this );
            mDepthBuffer = nullptr;
        }
    }

    void DiRenderTarget::Bind(uint8 mrtID)
    {
        PreBind();

        if (!BindRenderTarget(mrtID))
        {
            DI_WARNING("Cannot bind render target with mrtID %d", mrtID);
            return;
        }

        if (!BindDepthStencil())
        {
            DI_WARNING("Cannot bind render target with mrtID %d", mrtID);
        }

        if (!mClearPerFrame)
            return;

        Driver->Clear(mClearFlag, mClearColor, mDepthClearValue);
    }

    void DiRenderTarget::ClearDepthBuffer()
    {
        mDepthBuffer = nullptr;
    }
}