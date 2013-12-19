
#include "MaterialSerial.h"
#include "Material.h"
#include "AssetManager.h"
#include "ShaderParam.h"
#include "ShaderManager.h"
#include "StlUtil.h"
#include "XMLFile.h"

namespace Demi 
{
    DiMaterialSerializer::DiMaterialSerializer()
    {
        InitTokensMaps();
    }

    DiMaterialSerializer::~DiMaterialSerializer()
    {
    }

    BOOL DiMaterialSerializer::ParseMaterial( DiDataStreamPtr data, DiMaterial* target )
    {
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(data->GetAsString());
        DiXMLElement root = xmlfile->GetRoot();
        ParseXml(root,target);

        return TRUE;
    }

    BOOL DiMaterialSerializer::ParseMaterial( DiXMLElement data, DiMaterial* target )
    {
        ParseXml(data,target);
        return TRUE;
    }

    BOOL DiMaterialSerializer::SaveMaterial( const DiString& file, DiMaterial* source )
    {
        DI_LOG("Save material file to :%s",file.c_str());

        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        DiXMLElement root = xmlfile->CreateRoot("material");

        SaveXml(root,source);

        xmlfile->Save(file);

        return TRUE;
    }

    BOOL DiMaterialSerializer::SaveMaterial( DiXMLElement node, DiMaterial* source )
    {
        SaveXml(node,source);
        return TRUE;
    }

    void DiMaterialSerializer::SaveXml(DiXMLElement rootNode, DiMaterial* source)
    {
        DI_ASSERT(source);

        DiXMLElement vp = rootNode.CreateChild("shader");
        vp.SetAttribute("name","vertex");
        vp.SetValue(source->GetVertexShaderName());

        DiXMLElement fp = rootNode.CreateChild("shader");
        fp.SetAttribute("name","pixel");
        fp.SetValue(source->GetPixelShaderName());

        if (source->IsWireframe())
        {
            DiXMLElement nd = rootNode.CreateChild("wireframe");
            nd.SetValue("true");
        }

        if (source->GetCullMode() != DEFAULT_CULL_MODE)
        {
            DiXMLElement nd = rootNode.CreateChild("cullMode");
            nd.SetValue(CullmodeToString(source->GetCullMode()).c_str());
        }

        if (source->GetEnableVertColor())
        {
            DiXMLElement nd = rootNode.CreateChild("vertcolor");
            nd.SetValue("true");
        }

        if (source->GetDepthCheck() != DEFAULT_DEPTH_CHECK)
        {
            DiXMLElement nd = rootNode.CreateChild("depthCheck");
            nd.SetValue("false");
        }

        if (source->GetDepthWrite() != DEFAULT_DEPTH_WRITE)
        {
            DiXMLElement nd = rootNode.CreateChild("depthWrite");
            nd.SetValue("false");
        }

        if (source->GetBlendMode() != DEFAULT_BLEND_MODE)
        {
            DiXMLElement nd = rootNode.CreateChild("blendMode");
            nd.SetValue(BlendModeToString(source->GetBlendMode()).c_str());
        }

        if(source->GetDiffuse() != DiColor::White)
        {
            DiXMLElement nd = rootNode.CreateChild("diffuse");
            DiString col;
            col.SetColourValue(source->GetDiffuse());
            nd.SetValue(col);
        }

        if(source->GetAmbient() != DiColor::White)
        {
            DiXMLElement nd = rootNode.CreateChild("ambient");
            DiString col;
            col.SetColourValue(source->GetAmbient());
            nd.SetValue(col);
        }

        if(source->GetSpecular() != DiColor::White)
        {
            DiXMLElement nd = rootNode.CreateChild("specular");
            DiString col;
            col.SetColourValue(source->GetSpecular());
            nd.SetValue(col);
        }

        if(source->GetOpacity() != 1.0f)
        {
            DiXMLElement nd = rootNode.CreateChild("opacity");
            DiString o;
            o.SetFloat(source->GetOpacity());
            nd.SetValue(o);
        }

        if(source->GetShininess() != 32.0f)
        {
            DiXMLElement nd = rootNode.CreateChild("shininess");
            DiString o;
            o.SetFloat(source->GetShininess());
            nd.SetValue(o);
        }

        if(source->GetShaderFlags() != 0)
        {
            DiXMLElement nd = rootNode.CreateChild("flag");
            DiString flagStr = DiShaderManager::GetInstancePtr()->CoverterFlags(source->GetShaderFlags());
            nd.SetValue(flagStr);
        }

        DiShaderParameter* sm = source->GetShaderParameter();
        if (sm)
        {
            size_t varNum = sm->GetVariableNum();
            DiXMLElement varNode;
            if (varNum > 0)
            {
                varNode = rootNode.CreateChild("variables");
            }
            for (size_t i = 0; i < varNum; i++)
            {
                const DiGpuVariable* var = sm->GetVariable(i);
                const DiAny& data = var->GetData();
                if (var && !data.isEmpty())
                {
                    DiGpuVariable::Type type = var->GetType();
                    if (type == DiGpuVariable::VARIABLE_FLOAT)
                    {
                        DiXMLElement nd = varNode.CreateChild("float");
                        nd.SetAttribute("name",var->GetName());
                        float val = any_cast<float>(data);
                        DiString str;
                        str.SetFloat(val);
                        nd.SetValue(str);
                    }
                    else if (type == DiGpuVariable::VARIABLE_FLOAT2)
                    {
                        DiXMLElement nd = varNode.CreateChild("float2");
                        nd.SetAttribute("name",var->GetName());
                        DiVec2 val = any_cast<DiVec2>(data);
                        DiString str;
                        str.SetVector2(val);
                        nd.SetValue(str);
                    }
                    else if (type == DiGpuVariable::VARIABLE_FLOAT3)
                    {
                        DiXMLElement nd = varNode.CreateChild("float3");
                        nd.SetAttribute("name",var->GetName());
                        DiVec3 val = any_cast<DiVec3>(data);
                        DiString str;
                        str.SetVector3(val);
                        nd.SetValue(str);
                    }
                    else if (type == DiGpuVariable::VARIABLE_FLOAT4)
                    {
                        DiXMLElement nd = varNode.CreateChild("float4");
                        nd.SetAttribute("name",var->GetName());
                        DiVec4 val = any_cast<DiVec4>(data);
                        DiString str;
                        str.SetVector4(val);
                        nd.SetValue(str);
                    }
                    else if (type == DiGpuVariable::VARIABLE_COLOR)
                    {
                        DiXMLElement nd = varNode.CreateChild("color");
                        nd.SetAttribute("name",var->GetName());
                        DiColor val = any_cast<DiColor>(data);
                        DiString str;
                        str.SetColourValue(val);
                        nd.SetValue(str);
                    }
                    else if (type == DiGpuVariable::VARIABLE_SAMPLER2D)
                    {
                        DiXMLElement nd = varNode.CreateChild("sampler2D");
                        DiTexture* tex = any_cast<DiTexture*>(data);
                        SaveTextureXml(nd,tex,var->GetName());
                    }
                    else if (type == DiGpuVariable::VARIABLE_SAMPLERCUBE)
                    {
                        DiXMLElement nd = varNode.CreateChild("samplerCUBE");
                        DiTexture* tex = any_cast<DiTexture*>(data);
                        SaveTextureXml(nd,tex,var->GetName());
                    }
                }
            }
        }
    }

    void DiMaterialSerializer::SaveTextureXml( DiXMLElement rootNode,DiTexture* tex ,const DiString& varname )
    {
        if (!tex)
        {
            return;
        }

        DiXMLElement texNode = rootNode.CreateChild("texture");
        texNode.SetAttribute("name",varname);
        texNode.SetValue(tex->GetName());

        if (tex->GetMinFilter() != DEFAULT_FILTER)
        {
            DiXMLElement nd = rootNode.CreateChild("filter");
            nd.SetAttribute("name","min");
            nd.SetValue(FilterToString(tex->GetMinFilter()).c_str());
        }
        if (tex->GetMagFilter() != DEFAULT_FILTER)
        {
            DiXMLElement nd = rootNode.CreateChild("filter");
            nd.SetAttribute("name","mag");
            nd.SetValue(FilterToString(tex->GetMagFilter()).c_str());
        }
        if (tex->GetMipFilter() != DEFAULT_FILTER)
        {
            DiXMLElement nd = rootNode.CreateChild("filter");
            nd.SetAttribute("name","mig");
            nd.SetValue(FilterToString(tex->GetMinFilter()).c_str());
        }
        if (tex->GetAddressingU() != DEFAULT_ADDRESSING_MODE)
        {
            DiXMLElement nd = rootNode.CreateChild("addressing");
            nd.SetAttribute("name","u");
            nd.SetValue(AddrModeToString(tex->GetAddressingU()).c_str());
        }
        if (tex->GetAddressingV() != DEFAULT_ADDRESSING_MODE)
        {
            DiXMLElement nd = rootNode.CreateChild("addressing");
            nd.SetAttribute("name","v");
            nd.SetValue(AddrModeToString(tex->GetAddressingV()).c_str());
        }
    }

    DiString DiMaterialSerializer::FilterToString( DiFilterType ft )
    {
        for (auto it = sFilterTypeMap.begin(); it != sFilterTypeMap.end(); ++it)
        {
            if (it->second == ft)
            {
                return it->first;
            }
        }
        return "LINEAR";
    }

    DiString DiMaterialSerializer::AddrModeToString( DiAddMode ad )
    {
        for (auto it = sAddrModeMap.begin(); it != sAddrModeMap.end(); ++it)
        {
            if (it->second == ad)
            {
                return it->first;
            }
        }
        return "WRAP";
    }

    DiString DiMaterialSerializer::CullmodeToString( DiCullMode cm )
    {
        DiMap<DiString, DiCullMode>::iterator it;
        DiMap<DiString, DiCullMode>::iterator itEnd = sCullModeMap.end();
        for (it = sCullModeMap.begin(); it != itEnd; ++it)
        {
            if (it->second == cm)
            {
                return it->first;
            }
        }
        return "CW";
    }

    DiString DiMaterialSerializer::BlendModeToString( DiBlendMode cm )
    {
        DiMap<DiString,DiBlendMode>::iterator it;
        DiMap<DiString,DiBlendMode>::iterator itEnd = sBlendModeMap.end();
        for (it = sBlendModeMap.begin(); it != itEnd; ++it)
        {
            if (it->second == cm)
            {
                return it->first;
            }
        }
        return "REPLACE";
    }

    void DiMaterialSerializer::ParseXml(DiXMLElement rootNode, DiMaterial* target)
    {
        if(rootNode.GetName() != "material")
        {
            DI_ERROR("Invalid material script.");
            return;
        }

        ResetDefaultValue(target);

        DiVector<DiXMLElement> shaders;
        DiVector<DiXMLElement> variables;
        DiVector<DiXMLElement> ops;

        DiXMLElement child = rootNode.GetChild();
        while (child)
        {
            if(child.CheckName("shader"))
                shaders.push_back(child);
            else if(child.CheckName("variables"))
                variables.push_back(child);
            else
                ops.push_back(child);

            child = child.GetNext();
        }

        // orders are important
        // op -> shader -> variables
        for(size_t i = 0; i < ops.size(); ++i)
            ParseMatOpNode(ops[i],target);
        for(size_t i = 0; i < shaders.size(); ++i)
            ParseShaderNode(shaders[i],target);
        for(size_t i = 0; i < variables.size(); ++i)
            ParseVarNode(variables[i],target);
    }

    void DiMaterialSerializer::ParseShaderNode( DiXMLElement shaderNode, DiMaterial* target )
    {
        DiString shadertype = shaderNode.GetAttribute("name");
        DiString shaderfile = shaderNode.GetValue();
        if (shadertype.empty())
        {
            DI_WARNING("Parsing material failed, invalid shader tags.");
            return;
        }
        if (shadertype == "vertex")
            target->LoadVertexShader(shaderfile);
        else if (shadertype == "pixel")
            target->LoadPixelShader(shaderfile);
        else
        {
            DI_WARNING("Parsing material failed, invalid shader type.");
        }
    }


    void DiMaterialSerializer::ParseVarNode( DiXMLElement varNode, DiMaterial* target )
    {
        DiXMLElement child = varNode.GetChild();
        while(child)
        {
            DiString tag = child.GetName();

            if (tag == "sampler2D")
                ParseVarSampler(child,target,false);
            else if (tag == "samplerCUBE")
                ParseVarSampler(child,target,true);
            else
            {
                DiString name = child.GetAttribute("name");
                if (name.empty())
                {
                    DI_WARNING("Cannot find the parameter name.");
                    continue;
                }
                DiShaderParameter* sm = target->GetShaderParameter();
                DiString var = child.GetValue();

                if (tag == "float")
                    sm->WriteFloat(name,var.AsFloat());
                else if (tag == "float2")
                    sm->WriteFloat2(name,var.AsVector2());
                else if (tag == "float3")
                    sm->WriteFloat3(name,var.AsVector3());
                else if (tag == "float4")
                    sm->WriteFloat4(name,var.AsVector4());
            }

            child = child.GetNext();
        }
    }

    void DiMaterialSerializer::ParseVarSampler( DiXMLElement samplerNode, DiMaterial* target ,bool cubemap)
    {
        DiShaderParameter* sm = target->GetShaderParameter();
        DiXMLElement child = samplerNode.GetChild();

        DiString texName,texFileName;

        DiFilterType filtMin = FT_LINEAR;
        DiFilterType filtMag = FT_LINEAR;
        DiFilterType filtMip = FT_LINEAR;

        DiAddMode addrU = AM_WRAP;
        DiAddMode addrV = AM_WRAP;

        while(child)
        {
            DiString name = child.GetAttribute("name");
            DiString tag = child.GetName();
            DiString val = child.GetValue();
            if (name.empty())
            {
                DI_WARNING("Cannot find the sampler name.");
                continue;
            }

            if (tag == "texture")
            {
                texName = name;
                texFileName = val;
            }
            else if (tag == "filter")
            {
                DiFilterType ft = StringToFilter(val);
                if (name == "min")
                    filtMin = ft;
                else if (name == "mag")
                    filtMag = ft;
                else if (name == "mip")
                    filtMip = ft;
            }
            else if (tag == "addressing")
            {
                DiAddMode addr = StringToAddrMode(val);
                if (name == "u")
                    addrU = addr;
                else if (name == "v")
                    addrV = addr;
                else if (name, "uv")
                {
                    addrV = addr;
                    addrV = addr;
                }
            }

            child = child.GetNext();
        }

        if (texName.empty() || texFileName.empty())
        {
            DI_WARNING("Invalid sampler resource name.");
            return;
        }

        DiTexturePtr tex;
        if (!cubemap)
            tex = sm->WriteTexture2D(texName,texFileName);
        else
            tex = sm->WriteTextureCUBE(texName,texFileName);

        if (tex)
        {
            tex->SetAddressingU(addrU);
            tex->SetAddressingV(addrV);
            tex->SetMinFilter(filtMin);
            tex->SetMagFilter(filtMag);
            tex->SetMipFilter(filtMip);
        }
        else
        {
            DI_WARNING("Failed to load the resource of sampler : %s",texFileName.c_str());
        }
    }


    void DiMaterialSerializer::ParseMatOpNode(DiXMLElement matNode, DiMaterial* target)
    {
        DiString tag = matNode.GetName();
        DiString val = matNode.GetParent().ChildValue(tag);

        if (tag == "cullMode")
        {
            DiCullMode cm = StringToCullmode(val);
            target->SetCullMode(cm);
        }
        else if (tag == "wireframe")
        {
            target->SetWireframe(val.AsBool());
        }
        else if (tag == "vertcolor")
        {
            target->SetEnableVertColor(val.AsBool());
        }
        else if (tag == "blendMode")
        {
            DiBlendMode blm = StringToBlendMode(val);
            target->SetBlendMode(blm);
        }
        else if (tag == "depthCheck")
        {
            target->SetDepthCheck(val.AsBool());
        }
        else if (tag == "depthWrite")
        {
            target->SetDepthWrite(val.AsBool());
        }
        else if (tag == "ambient")
        {
            target->SetAmbient(val.AsColourValue());
        }
        else if (tag == "diffuse")
        {
            target->SetDiffuse(val.AsColourValue());
        }
        else if (tag == "specular")
        {
            target->SetSpecular(val.AsColourValue());
        }
        else if (tag == "opacity")
        {
            target->SetOpacity(val.AsFloat());
        }
        else if (tag == "shininess")
        {
            target->SetShininess(val.AsFloat());
        }
        else if (tag == "flag")
        {
            DiVector<DiString> flags = val.Tokenize(",");
            target->SetShaderFlag(DiShaderManager::GetInstancePtr()->CoverterFlags(flags));
        }
        else
        {
            DI_WARNING("Unknown material tag : %s",tag.c_str());
        }
    }


    void DiMaterialSerializer::InitTokensMaps()
    {
        if (!sCullModeMap.empty())
            return;

        sCullModeMap["CW"]                = CULL_CW;
        sCullModeMap["CCW"]                = CULL_CCW;
        sCullModeMap["NONE"]            = CULL_NONE;

        sFilterTypeMap["NONE"]            = FT_NONE;
        sFilterTypeMap["POINT"]            = FT_POINT;
        sFilterTypeMap["LINEAR"]        = FT_LINEAR;
        sFilterTypeMap["ANISOTROPIC"]   = FT_ANISOTROPIC;

        sAddrModeMap["WRAP"]            = AM_WRAP;
        sAddrModeMap["MIRROR"]            = AM_MIRROR;
        sAddrModeMap["CLAMP"]           = AM_CLAMP;
        sAddrModeMap["BORDER"]            = AM_BORDER;
        sAddrModeMap["MIRRORONCE"]        = AM_MIRRORONCE;

        sBlendModeMap["REPLACE"]        = BLEND_REPLACE;
        sBlendModeMap["ADD"]            = BLEND_ADD;
        sBlendModeMap["MULTIPLY"]       = BLEND_MULTIPLY;
        sBlendModeMap["ALPHA"]            = BLEND_ALPHA;
        sBlendModeMap["TRANS_COLOR"]    = BLEND_TRANS_COLOR;
        sBlendModeMap["ONE_INV_ALPHA"]    = BLEND_ONE_INV_ALPHA;
    }

    DiFilterType DiMaterialSerializer::StringToFilter( const DiString& str )
    {
        return sFilterTypeMap.find_def(str,DEFAULT_FILTER);
    }

    DiAddMode DiMaterialSerializer::StringToAddrMode( const DiString& str )
    {
        return sAddrModeMap.find_def(str,DEFAULT_ADDRESSING_MODE);
    }

    DiCullMode DiMaterialSerializer::StringToCullmode( const DiString& str )
    {
        return sCullModeMap.find_def(str,DEFAULT_CULL_MODE);
    }

    Demi::DiBlendMode DiMaterialSerializer::StringToBlendMode( const DiString& str )
    {
        return sBlendModeMap.find_def(str,DEFAULT_BLEND_MODE);
    }

    void DiMaterialSerializer::ResetDefaultValue( DiMaterial* target )
    {
        target->SetCullMode(DEFAULT_CULL_MODE);
        target->SetBlendMode(DEFAULT_BLEND_MODE);
        target->SetDepthWrite(DEFAULT_DEPTH_WRITE);
        target->SetDepthCheck(DEFAULT_DEPTH_CHECK);
        target->SetWireframe(FALSE);
    }

    DiMap<DiString,DiBlendMode>            DiMaterialSerializer::sBlendModeMap;
    DiMap<DiString,DiCullMode>            DiMaterialSerializer::sCullModeMap;
    DiMap<DiString,DiAddMode>           DiMaterialSerializer::sAddrModeMap;
    DiMap<DiString,DiFilterType>        DiMaterialSerializer::sFilterTypeMap;

    DiCullMode        DiMaterialSerializer::DEFAULT_CULL_MODE            = CULL_CW;
    DiAddMode       DiMaterialSerializer::DEFAULT_ADDRESSING_MODE    = AM_WRAP;
    DiFilterType     DiMaterialSerializer::DEFAULT_FILTER            = FT_LINEAR;
    DiBlendMode        DiMaterialSerializer::DEFAULT_BLEND_MODE        = BLEND_REPLACE;
    BOOL             DiMaterialSerializer::DEFAULT_DEPTH_WRITE         = TRUE;
    BOOL             DiMaterialSerializer::DEFAULT_DEPTH_CHECK         = TRUE;
    BOOL            DiMaterialSerializer::DEFAULT_USER_MATERIAL        = TRUE;
}