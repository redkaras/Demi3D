
/********************************************************************
    File:       RenderTarget.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GfxPrerequisites.h"
#include "DeviceLostListener.h"

namespace Demi 
{
    class DI_GFX_API DiViewport
    {
    public:

        DiViewport()
            :mLeft(0), mTop(0), mWidth(1.0), mHeight(1.0)
        {
        }

        float mLeft;
              
        float mTop;
              
        float mWidth;
              
        float mHeight;
    };

    class DI_GFX_API DiRtListener
    {
    public:

        DiRtListener(){}

        virtual void PreUpdateRenderTarget(DiRenderTarget*) = 0;

        virtual void PostUpdateRenderTarget(DiRenderTarget*) = 0;
    };

    class DI_GFX_API DiRenderTarget : public DiDeviceLostListener
    {
    public:

        DiRenderTarget();

        virtual                 ~DiRenderTarget();

    public:

        virtual void            AttachSurface() = 0;

        virtual void            DetachSurface() = 0;

        void                    SetViewport(float left = 0.0f, float top = 0.0f,
                                            float width = 1.0f, float height = 1.0f)
        {
            mViewport.mLeft   = left;
            mViewport.mTop    = top;
            mViewport.mWidth  = width;
            mViewport.mHeight = height;
        }

        DiViewport&             GetViewport() { return mViewport; }

        uint32                  GetWidth(void) const { return mWidth; }
                                
        uint32                  GetHeight(void) const { return mHeight; }

        void                    AttachTexture(DiTexture* texture);

        void                    DetachTexture();

        virtual void            OnDeviceLost();

        virtual void            OnDeviceReset() {}

        virtual void            PreBind() = 0;

        virtual void            Bind(uint8 mrtID = 0);

        void                    SetListener(DiRtListener* val) { mListener = val; }

        bool                    GetShouldUpdate() const { return mShouldUpdate; }

        void                    SetShouldUpdate(bool val) { mShouldUpdate = val; }

        void                    SetDepthBufferPool( uint16 poolId );

        uint16                  GetDepthBufferPool() const;

        DiDepthBuffer*          GetDepthBuffer() const;

        virtual bool            AttachDepthBuffer( DiDepthBuffer *depthBuffer );

        virtual void            DetachDepthBuffer();

        virtual void            ClearDepthBuffer();

        virtual bool            BindRenderTarget(uint8 mrtid) = 0;

        virtual bool            BindDepthStencil() = 0;

        virtual DiDepthBuffer*  CreateDepthBuffer() = 0;

        virtual bool            IsCompatibleWith(DiDepthBuffer* db) = 0;

        bool                    GetClearPerFrame() const { return mClearPerFrame; }

        void                    SetClearPerFrame(bool val) { mClearPerFrame = val; }

        DiColor                 GetClearColor() const { return mClearColor; }

        void                    SetClearColor(const DiColor& val) { mClearColor = val; }

        float                   GetDepthClearValue() const { return mDepthClearValue; }

        void                    SetDepthClearValue(float val) { mDepthClearValue = val; }

        uint32                  GetClearFlag() const { return mClearFlag; }

        void                    SetClearFlag(uint32 val) { mClearFlag = val; }

        virtual bool            SwapBuffer() { return true; }

    protected:

        DiRtListener*           mListener;

        uint32                  mWidth;

        uint32                  mHeight;
        
        DiViewport              mViewport;

        DiTexture*              mParentTex;

        bool                    mShouldUpdate;
        
        uint16                  mDepthBufferPoolId;

        DiDepthBuffer*          mDepthBuffer;

        DiColor                 mClearColor;

        float                   mDepthClearValue;

        bool                    mClearPerFrame;

        uint32                  mClearFlag;

        bool                    mViewportDirty;
    };
}