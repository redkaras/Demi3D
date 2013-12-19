
/********************************************************************
    Date:        2012-3-31
    File:       MaterialPredefine.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_GFX_API DiMaterialDefine
    {
    public:

        // default material
        static DiString        DEFAULT_MATERIAL;

        // some additional parameter about shader macro

        static DiString        PARAM_VERTEX_COLOR;
        static DiString        PARAM_VERTEX_INSTANCE;

        static DiString        PARAM_INSTANCE_NONE;
        static DiString        PARAM_INSTANCE_SHADER;
        static DiString        PARAM_INSTANCE_HARDWARE;


        // for font rendering
        static DiString        TEXT_SHADER_NAME;    

        // simple static mesh
        static DiString        STATIC_MESH_SHADER_NAME;

        // terrain shaders
        static DiString        TERRAIN_VERTEX_SHADER;
        static DiString        TERRAIN_PIXEL_SHADER;
        static DiString        WATER_PIXEL_SHADER;
        static DiString        WATER_VERTEX_SHADER;

        // for sky box
        static DiString        SKY_PIXEL_SHADER;

        // hardware skinning shader
        static DiString        SKINING_1_WEIGHT_SHADER;
        static DiString        SKINING_2_WEIGHTS_SHADER;
        static DiString        SKINING_4_WEIGHTS_SHADER;

        // general pixel shader
        static DiString        DIFFUSE_PIXEL_SHADER;
        static DiString        DIFFUSE_NORMAL_PIXEL_SHADER;
        static DiString        DIFFUSE_GLOW_PIXEL_SHADER;
        static DiString        DIFFUSE_NORMAL_SPECULAR_PIXEL_SHADER;
        static DiString        DIFFUSE_NORMAL_GLOW_PIXEL_SHADER;
        static DiString        DIFFUSE_NORMAL_SPECULAR_GLOW_PIXEL_SHADER;
        static DiString        DIFFUSE_LIGHTMAP_SHADER;

        // for the output of the post effect
        static DiString        SCREEN_QUAD_VERT_SHADER;
        static DiString        SCREEN_QUAD_PIXEL_SHADER;
    };
}