
/********************************************************************
    File:       Material.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GfxPrerequisites.h"
#include "GpuProgram.h"
#include "Asset.h"
#include "ShaderProgram.h"

namespace Demi 
{
    
    typedef shared_ptr<DiMaterial> DiMaterialPtr;

    class DI_GFX_API DiMaterial : public DiAsset
    {
    public:

        friend class                DiShaderParameter;
        friend class                DiAssetManager;
        friend class                DiRenderPipeline;

    public:

        DiMaterial(const DiString& name);

        virtual                     ~DiMaterial(void);

    public:

        void                        Release() { delete this; }

        DiCullMode                  GetCullMode() const { return mCullMode; }

        void                        SetCullMode(DiCullMode val);

        DiAssetType                 GetAssetType() const {return TYPE;}

        BOOL                        GetDepthWrite() const { return mDepthWrite; }
                                    
        void                        SetDepthWrite(BOOL val) { mDepthWrite = val; }
                                    
        BOOL                        GetDepthCheck() const { return mDepthCheck; }
                                    
        void                        SetDepthCheck(BOOL val) { mDepthCheck = val; }
                                    
        DiShaderParameter*          GetShaderParameter() const { return mShaderParameter; }

        bool                        HasTexture();

        void                        LoadShader(const DiString& vsname,const DiString& psname);

        void                        LoadVertexShader(const DiString& vsname);

        void                        LoadPixelShader(const DiString& psname);

        DiString                    GetVertexShaderName() const;

        DiString                    GetPixelShaderName() const;

        DiShaderProgram*            GetVertexShader(){return mVertexShader;}

        DiShaderProgram*            GetPixelShader(){return mPixelShader;}

        BOOL                        Load(DiDataStreamPtr data);

        BOOL                        Load();

        BOOL                        LoadingComplete() const;

        BOOL                        IsWireframe() const { return mWireframe; }

        void                        SetWireframe(BOOL val) { mWireframe = val; }

        BOOL                        GetForceWireframe() const { return mForceWireframe; }

        void                        SetForceWireframe(BOOL val) { mForceWireframe = val; }

        DiBlendMode                 GetBlendMode() const { return mBlendMode; }

        void                        SetBlendMode(DiBlendMode val) { mBlendMode = val; }

        bool                        IsTransparent() const;

        BOOL                        GetEnableVertColor() const { return mEnableVertColor; }

        void                        SetEnableVertColor(BOOL val);

        void                        SetInstanceType(DiInstanceType val);

        DiInstanceType              GetEnableInstance() const {return mInstanceState;}

        void                        SetExtraParams(DiCompileDesc& desc);

        void                        Bind() const;

        void                        SetShaderFlags(uint64 f){mShaderFlag = f;}

        uint64                      GetShaderFlags()const{return mShaderFlag;}

        void                        SetShaderFlag(uint64 f){mShaderFlag |= f;}

        void                        ClearShaderFlag(uint64 f){mShaderFlag &= (~f);}

        void                        BindVariable( const DiGpuVariable &variable, 
                                        const void *data) const;

        void                        RecompileShaderWithMarco(DiShaderType shader,
                                        const DiPair<DiString,DiString>& marco);

        DiMaterialPtr               Clone(const DiString& newname);

        DiCommonVsType              GetCommonVsType() const { return mCommonVsType; }

        static DiMaterialPtr        QuickCreate(const DiString& vs, const DiString& ps, uint64 flag = 0);
                                    
        static DiMaterialPtr        QuickCreate(const DiString& name, const DiString& vs, const DiString& ps, uint64 flag = 0);

        // common parameters

        DiColor                     GetDiffuse() const { return mDiffuse; }

        void                        SetDiffuse(DiColor val) { mDiffuse = val; }

        DiColor                     GetAmbient() const { return mAmbient; }

        void                        SetAmbient(DiColor val) { mAmbient = val; }

        float                       GetOpacity() const { return mOpacity; }

        void                        SetOpacity(float val) { mOpacity = val; }

        float                       GetShininess() const { return mShininess; }

        void                        SetShininess(float val) { mShininess = val; }

        DiColor                     GetSpecular() const { return mSpecular; }
        
        void                        SetSpecular(DiColor val) { mSpecular = val; }

    public:

        static DiAssetType          TYPE;

    protected:

        DiCullMode                  mCullMode;

        BOOL                        mDepthWrite;

        BOOL                        mDepthCheck;

        BOOL                        mWireframe;

        BOOL                        mForceWireframe;

        BOOL                        mEnableVertColor;

        DiInstanceType              mInstanceState;

        DiBlendMode                 mBlendMode;

        DiShaderParameter*          mShaderParameter;

        DiShaderProgram*            mVertexShader;

        DiShaderProgram*            mPixelShader;

        float                       mOpacity;

        float                       mShininess;

        DiColor                     mAmbient;

        DiColor                     mDiffuse;
        
        DiColor                     mSpecular;

        DiCommonVsType              mCommonVsType;

        uint64                      mShaderFlag;
    };

}