
/********************************************************************
    File:       RenderWindow.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Texture.h"

namespace Demi 
{
    class DI_GFX_API DiRenderWindow : public DiBase
    {
    public:

        DiRenderWindow(const DiString& name);

        virtual                 ~DiRenderWindow();

    public:

        static DiRenderWindow*  ActiveWindow;

        void                    Create(DiWndHandle hwnd);

        void                    Create(uint32 width, uint32 height, const DiString& title, bool fullscreen);

        void                    Render();

        void                    SwapBuffer();

        void                    AddUpdateRenderTarget(DiRenderTarget* rt);

        void                    RemoveUpdateRenderTarget(DiRenderTarget* rt);

        DiSceneManager*         GetSceneManager() { return mSceneManager; }

        DiPostEffectManager*    GetPostEffectManager() {return mPostEffectMgr;}

        DiRenderTarget*         GetRenderBuffer() {return mRenderBuffer;}

        DiGBuffer*              GetGBuffer() const {return mGBuffer;}

        DiTexturePtr            GetCanvasTexture() const { return mCanvasTexture; }

        void                    OnDeviceLost();

        void                    OnDeviceReset();

        uint32                  GetWidth();

        uint32                  GetHeight();

        bool                    GetEnable() const { return mEnable; }

        void                    SetEnable(bool val) { mEnable = val; }

        void                    SetCurrentBatchesNum( uint32 val ) {mCurrentBatchesNum = val;}

        void                    SetCurrentPrimitivesNum( uint32 val ){mCurrentPrimitivesNum = val;}

        uint32                  GetCurrentBatchesNum() const { return mCurrentBatchesNum; }

        uint32                  GetCurrentPrimitivesNum() const { return mCurrentPrimitivesNum; }

        const DiString&         GetName(){return mName;}

        void                    Closing();

    protected:

        void                    Init();

        void                    Shutdown();

    protected:

        DiSceneManager*         mSceneManager;
        
        DiRenderTarget*         mSceneCanvas;

        DiRenderTarget*         mRenderBuffer;

        DiCamera*               mMainCamera;

        DiPostEffectManager*    mPostEffectMgr;

        DiTexturePtr            mCanvasTexture;

        DiTexturePtr            mDepthStencil;

        typedef DiVector<DiRenderTarget*> UpdateRTs;

        UpdateRTs               mUpdateRTs;

        DiGBuffer*              mGBuffer;

        DiString                mName;

        bool                    mEnable;

        uint32                  mCurrentBatchesNum;

        uint32                  mCurrentPrimitivesNum;
        
        uint32                  mWidth;

        uint32                  mHeight;

        DiWndHandle             mWndHandle;

        DiWindow*               mWindow;
    };
}
