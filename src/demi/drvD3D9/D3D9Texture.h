
/********************************************************************
    File:       D3D9Texture.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Texture.h"

namespace Demi
{
    class DI_D3D9DRV_API DiD3D9TextureDrv : public DiTextureDrv
    {
    public:

        DiD3D9TextureDrv(DiTexture* parent);

        ~DiD3D9TextureDrv();

    public:

        void                    CreateTexture();

        void                    Release();

        void*                   LockLevel(uint32 level, uint32 &pitch, uint32 surface = 0);

        void                    UnlockLevel(uint32 level, uint32 surface = 0);

        void                    Bind(uint32 samplerIndex);

        void                    BlitToMemory(const DiBox &srcBox, const DiPixelBox &dst);

        void*                   GetSurfaceHandle();

        IDirect3DSurface9*      GetSurface() { return mSurface; }

    private:

        IDirect3DSurface9*      mSurface;

        D3DPOOL                 mPool;

        DWORD                   mD3DUsage;

        IDirect3DBaseTexture9*  mTexture;
    };
}