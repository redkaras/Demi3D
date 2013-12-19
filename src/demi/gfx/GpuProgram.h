
/********************************************************************
    File:       GpuProgram.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Skeleton.h"
#include "GfxDriver.h"
#include "Any.h"
#include <functional>

namespace Demi 
{
    enum DiVsVersion
    {
        VS_INVALID = -1,
        VS_NONE = 0,
        VS_1_1,
        VS_2_0,
        VS_2_a,
        VS_3_0,
        VS_4_0,

        NUM_VERTEX_SHADER_VERSIONS
    };

    enum DiPsVersion
    {
        PS_INVALID = -1,
        PS_NONE = 0,
        PS_1_1,
        PS_1_2,
        PS_1_3,
        PS_1_4,
        PS_2_0,
        PS_2_b,
        PS_2_a,
        PS_3_0,
        PS_4_0,

        NUM_PIXEL_SHADER_VERSIONS
    };

    typedef DiPair<DiString,DiString>    DiMarcoDefine;
    typedef DiVector<DiMarcoDefine>        DiMarcoDefineList;

    struct DiCompileDesc
    {

        void                AddMarco(const DiString& name, const DiString& def = DiString::BLANK);

        void                RemoveMarco(const DiString& name);

        DiString            profile;

        DiMarcoDefineList    marcos;

        DiString            entryName;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiGpuVariable
    {
        friend class DiShaderParameter;
    public:

        enum Type
        {
            VARIABLE_FLOAT = 0,
            VARIABLE_FLOAT2,
            VARIABLE_FLOAT3,
            VARIABLE_COLOR,    
            VARIABLE_FLOAT4,
            VARIABLE_FLOAT4_ARRAY,

            VARIABLE_SAMPLER2D,
            VARIABLE_SAMPLERCUBE,

            NUM_VARIABLE_TYPES
        };

        struct HelperStruct
        {
            HelperStruct();

            DiString    refName;
            DiString    desc;
            float        maxValue;
            float        minValue;
            float        stepValue;
            bool        asColor;
        };

        DiGpuVariable(const DiString& name, Type type);

        virtual                 ~DiGpuVariable(void);

    public:

        const DiString&            GetName(void) const;

        Type                    GetType(void) const;

        uint32                    GetDataSize(void) const;

        virtual void            Bind() const = 0;

        DiAny&                    GetData() {return mData;}

        const DiAny&            GetData() const {return mData;}

        void                    SetData(DiAny& a){ mData = a; }

        bool                    IsPublicVariable() const { return mPublicVar; }

        HelperStruct&            GetHelperData() { return mHelperData; }

        const HelperStruct&        GetHelperData() const { return mHelperData; }

        void                    ParsePublicHelperScript();

    protected:

        DiString                mName;

        Type                    mType;

        DiAny                    mData;

        bool                    mPublicVar;
        
        DiString                mHelperScript;

        HelperStruct            mHelperData;
    };


    //////////////////////////////////////////////////////////////////////////
    class DiShaderEnvironment
    {
    public:
        DiShaderEnvironment(void);

    public:
        DiMat4      modelMatrix;

        DiMat4      viewMatrix;
        DiMat4      projMatrix;
        DiMat4      modelViewMatrix;
        DiMat4      modelViewProjMatrix;
        DiMat4      viewProjMatrix;
        DiMat4      texViewProjMatrix;

        DiVec3      eyePosition;
        DiVec4      eyePositionObjSpace;
        DiVec3      eyeDirection;        

        DiVec4      farnearPlane;

        DiColor     globalAmbient;
        DiColor     ambientColor;
        DiColor     diffuseColor;
        DiColor     specularColor;
        float       opacity;
        float       shininess;

        DiVec4      boneMatrices[MAX_BONE_NUM*3];    
        DiVec4      modelMatrices[MAX_WORLD_MATS*3];
        uint32      numBones;
        uint32      numModelMatrices;

        float       time;
        DiVec4      viewportSize;
        DiVec4      texelOffsets;

        int         numDirLights;
        DiColor     dirLightsColor[MAX_LIGHTS];
        DiVec4      dirLightsDir[MAX_LIGHTS];

        int         numPointLights;
        DiColor     pointLightsColor[MAX_LIGHTS];
        DiVec4      pointLightsPosition[MAX_LIGHTS];
        DiVec4      pointLightsAttenuation[MAX_LIGHTS];

        void        SetIdentity();
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiShaderInstance : public DiBase
    {
    public:

        DiShaderInstance(DiShaderType type){}
        

        virtual void            Bind(const DiShaderEnvironment& shaderEnv) = 0;

        virtual void            Release()   = 0;

        virtual DiShaderType    GetType()   = 0;

        virtual void            LoadVariables(std::function<void(DiGpuVariable*)> func) = 0;

        virtual void            Compile(const DiString& code) = 0;

        DiCompileDesc           mCompileDesc;
    };
}