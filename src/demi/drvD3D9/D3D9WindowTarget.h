
/********************************************************************
    File:       D3D9WindowTarget.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "D3D9RenderTarget.h"

namespace Demi
{
    // used for render window
    class DiD3D9WindowTarget : public DiD3D9RenderTarget
    {
    public:
        DiD3D9WindowTarget(void);

        virtual                 ~DiD3D9WindowTarget(void);
                                
    public:                     
                                
        virtual void            Create(HWND hwnd);
                                
        HWND                    GetWindowHandle() const { return mWndHandle; }
                                
        void                    OnDeviceLost();
                                
        void                    OnDeviceReset();
                                
        uint32                  GetWidth()const;
                                
        uint32                  GetHeight()const;
                                
        bool                    IsSwapChainWindow();
                                
        void                    ReleaseResources();
                                
        bool                    CheckSizeChanged();
                                
        bool                    SwapBuffer();
                                
    protected:                  
                                
        HRESULT                 Present();

    protected:

        HWND                    mWndHandle;

        IDirect3DSwapChain9*    mSwapChain;

        D3DPRESENT_PARAMETERS   mParameters;

        IDirect3DSurface9*      mD3D9DepthBuffer;
    };
}