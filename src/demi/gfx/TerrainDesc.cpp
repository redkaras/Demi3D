
#include "TerrainDesc.h"

bool Demi::DiTerrainDesc::CheckValid()
{
    if (mGridSize < 0)
    {
        return false;
    }

    if (mSizeX <= 0 || mSizeX > 200)
    {
        return false;
    }

    if (mSizeY <= 0 || mSizeY > 200)
    {
        return false;
    }

    return true;
}

void Demi::DiTerrainDesc::Release()
{
    SAFE_ARRAY_DELETE(mHeightData);    
    SAFE_ARRAY_DELETE(mCliffData);
    SAFE_ARRAY_DELETE(mColorData);
    SAFE_ARRAY_DELETE(mLayers[0].DemiureId);
    SAFE_ARRAY_DELETE(mLayers[1].DemiureId);

    SAFE_ARRAY_DELETE(mWaterMap);
    SAFE_ARRAY_DELETE(mWaterHeightMap);

    FoliageLayers::iterator it;
    FoliageLayers::iterator itEnd = mFoliageLayers.end();
    for (it = mFoliageLayers.begin(); it != itEnd; ++it)
    {
        SAFE_DELETE(*it);
    }
    mFoliageLayers.clear();
}

Demi::DiTerrainDesc::DiTerrainDesc()
{
    mGridSize            = 32;
    mSizeX            = 5;
    mSizeY            = 5;
    
    mCliffSpace        = 4;

    DemiureScale     = 0.2f;

    mHeightData        = NULL;
    mColorData        = NULL;    
    mCliffData        = NULL;
    mWaterMap        = NULL;
    mWaterHeightMap    = NULL;
}

Demi::DiTerrainDesc::~DiTerrainDesc()
{
    Release();
}

uint32 Demi::DiTerrainDesc::GetVertNum()
{
    uint32 vertSize = (CHUNK_GRID_SIZE*mSizeX + 1) * 
        (CHUNK_GRID_SIZE*mSizeY + 1);
    return vertSize;    
}

uint32 Demi::DiTerrainDesc::GetGridNum()
{
    uint32 gridSize = CHUNK_GRID_SIZE*mSizeX *
        CHUNK_GRID_SIZE*mSizeY;
    return gridSize;
}

uint32 Demi::DiTerrainDesc::GetCliffVertNum()
{
    uint32 cliffGridX = (CHUNK_GRID_SIZE*mSizeX)/mCliffSpace;
    uint32 cliffGridY = (CHUNK_GRID_SIZE*mSizeY)/mCliffSpace;
    
    uint32 vertSize = (cliffGridX + 1) * (cliffGridY + 1);
    
    return vertSize;
}

uint32 Demi::DiTerrainDesc::GetCliffGridNum()
{
    uint32 cliffGridX = (CHUNK_GRID_SIZE*mSizeX)/mCliffSpace;
    uint32 cliffGridY = (CHUNK_GRID_SIZE*mSizeY)/mCliffSpace;
    
    uint32 gridSize = cliffGridX * cliffGridY;
    return gridSize;    
}

uint32 Demi::DiTerrainDesc::GetCliffGridXNum()
{
    uint32 cliffGridX = (CHUNK_GRID_SIZE*mSizeX)/mCliffSpace;
    return cliffGridX;
}

uint32 Demi::DiTerrainDesc::GetCliffGridYNum()
{
    uint32 cliffGridY = (CHUNK_GRID_SIZE*mSizeY)/mCliffSpace;
    return cliffGridY;
}

DiFoliageLayerDesc::DiFoliageLayerDesc()
{
    mRenderTechnique    = 0;
    mAnimate            = TRUE;
    mLighting            = FALSE;
    mColor            = 0xffffffff;

    mMinWidth            = 20;
    mMinHeight        = 60;
    mMaxWidth            = 25;
    mMaxHeight        = 100;
    mAnimFreq            = 10;
    mAnimMag            = 20;
    mDensity            = 0.01f;
    mAnimSpeed        = 0.2f;

    ClearDensityMap();
}

void Demi::DiFoliageLayerDesc::ClearDensityMap()
{
    memset(mDensityMap,0,FOLIAGE_DENSITY_MAP_SIZE*FOLIAGE_DENSITY_MAP_SIZE*sizeof(BYTE));
}

void Demi::DiFoliageLayerDesc::Load( DiDataStreamPtr ds )
{
    DemiureName = ds->GetLine();

    ds->Read(&mRenderTechnique,    sizeof(mRenderTechnique));
    ds->Read(&mDensity,            sizeof(mDensity));
    ds->Read(&mAnimate,            sizeof(mAnimate));
    ds->Read(&mLighting,            sizeof(mLighting));
    ds->Read(&mAnimMag,            sizeof(mAnimMag));
    ds->Read(&mAnimSpeed,            sizeof(mAnimSpeed));
    ds->Read(&mAnimFreq,            sizeof(mAnimFreq));
    ds->Read(&mMinWidth,            sizeof(mMinWidth));
    ds->Read(&mMaxWidth,            sizeof(mMaxWidth));
    ds->Read(&mMinHeight,            sizeof(mMinHeight));
    ds->Read(&mMaxHeight,            sizeof(mMaxHeight));
    ds->Read(&mColor,            sizeof(mColor));
    ds->Read(&mDensityMap,        sizeof(BYTE)*FOLIAGE_DENSITY_MAP_SIZE*FOLIAGE_DENSITY_MAP_SIZE);
}

void Demi::DiFoliageLayerDesc::Save( DiDataStreamPtr ds )
{
    ds->Write(DemiureName.c_str(), DemiureName.length());
    char terminator = '\n';
    ds->Write(&terminator, 1);

    ds->Write(&mRenderTechnique,    sizeof(mRenderTechnique));
    ds->Write(&mDensity,            sizeof(mDensity));
    ds->Write(&mAnimate,            sizeof(mAnimate));
    ds->Write(&mLighting,            sizeof(mLighting));
    ds->Write(&mAnimMag,            sizeof(mAnimMag));
    ds->Write(&mAnimSpeed,        sizeof(mAnimSpeed));
    ds->Write(&mAnimFreq,            sizeof(mAnimFreq));
    ds->Write(&mMinWidth,            sizeof(mMinWidth));
    ds->Write(&mMaxWidth,            sizeof(mMaxWidth));
    ds->Write(&mMinHeight,        sizeof(mMinHeight));
    ds->Write(&mMaxHeight,        sizeof(mMaxHeight));
    ds->Write(&mColor,            sizeof(mColor));
    ds->Write(&mDensityMap,        sizeof(BYTE)*FOLIAGE_DENSITY_MAP_SIZE*FOLIAGE_DENSITY_MAP_SIZE);
}