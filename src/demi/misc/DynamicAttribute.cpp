
#include "DynamicAttribute.h"

namespace Demi
{
    DiAttributeFixed::DiAttributeFixed() : mValue(0)
    {
        mType = DiDynamicAttribute::DAT_FIXED;
    }

    DiAttributeFixed::DiAttributeFixed( const DiAttributeFixed& rhs )
    {
        mType = DiDynamicAttribute::DAT_FIXED;
        mValue = rhs.mValue;
    }

    DiAttributeFixed::~DiAttributeFixed()
    {

    }

    float DiAttributeFixed::GetValue( float x /*= 0*/ )
    {
        return mValue;
    }

    void DiAttributeFixed::SetValue( float value )
    {
        mValue = value;
    }

    void DiAttributeFixed::CopyTo( DiDynamicAttribute* att )
    {
        if (!att || att->GetType() != DiDynamicAttribute::DAT_FIXED)
        {
            return;
        }

        DiAttributeFixed* dynAttr = static_cast<DiAttributeFixed*>(att);
        dynAttr->mValue = mValue;
    }


    DiAttributeRandom::DiAttributeRandom() : mMin(0),mMax(0)
    {
        mType = DiDynamicAttribute::DAT_RANDOM;
    }

    DiAttributeRandom::DiAttributeRandom( const DiAttributeRandom& rhs )
    {
        mType = DiDynamicAttribute::DAT_RANDOM;
        mMin = rhs.mMin;
        mMax = rhs.mMax;
    }

    DiAttributeRandom::~DiAttributeRandom()
    {

    }

    float DiAttributeRandom::GetValue( float x /*= 0*/ )
    {
        return DiMath::RangeRandom(mMin, mMax);
    }

    void DiAttributeRandom::SetMin( float min )
    {
        mMin = min;
    }

    float DiAttributeRandom::GetMin( void ) const
    {
        return mMin;
    }

    void DiAttributeRandom::SetMax( float max )
    {
        mMax = max;
    }

    float DiAttributeRandom::GetMax( void ) const
    {
        return mMax;
    }

    void DiAttributeRandom::SetMinMax( float min, float max )
    {
        mMin = min;
        mMax = max;
    }

    void DiAttributeRandom::CopyTo( DiDynamicAttribute* att )
    {
        if (!att || att->GetType() != DiDynamicAttribute::DAT_RANDOM)
        {
            return;
        }

        DiAttributeRandom* dynAttr = static_cast<DiAttributeRandom*>(att);
        dynAttr->mMin = mMin;
        dynAttr->mMax = mMax;
    }

    DiAttributeCurved::DiAttributeCurved( void ):
        mInterpolationType(IT_LINEAR), 
        mRange(0)
    {
        mType = DiDynamicAttribute::DAT_CURVED;
    }

    DiAttributeCurved::DiAttributeCurved( InterpolationType interpolationType ):
        mInterpolationType(interpolationType), 
        mRange(0)
    {
        mType = DiDynamicAttribute::DAT_CURVED;
    }

    void DiAttributeCurved::SetInterpolationType( InterpolationType interpolationType )
    {
        if (interpolationType != mInterpolationType)
        {
            RemoveAllControlPoints();
            mInterpolationType = interpolationType;
        }
    }

    InterpolationType DiAttributeCurved::GetInterpolationType( void ) const
    {
        return mInterpolationType;
    }

    float DiAttributeCurved::GetValue( float x /*= 0*/ )
    {
        switch (mInterpolationType)
        {
        case IT_LINEAR:
            {
                if (mControlPoints.empty())
                {
                    return 0;
                }

                ControlPointList::iterator it1 = FindNearestControlPointIterator(x);
                ControlPointList::iterator it2 = it1 + 1;
                if (it2 != mControlPoints.end())
                {
                    // y = y1 + ((y2 - y1) * (x - x1)/(x2 - x1))
                    return (*it1).y + (((*it2).y - (*it1).y) * (x - (*it1).x)/((*it2).x - (*it1).x));
                }
                else
                {
                    return (*it1).y;
                }
            }
            break;

        case IT_SPLINE:
            {
                if (mSpline.GetNumPoints() < 1)
                {
                    return 0;
                }

                float fraction = x / mRange;
                return (mSpline.Interpolate(fraction < 1.0f ? fraction : 1.0f)).y;
            }
            break;
        }

        return 0;
    }

    void DiAttributeCurved::AddControlPoint( float x, float y )
    {
        mControlPoints.push_back(DiVec2(x, y));
    }

    const DiAttributeCurved::ControlPointList& DiAttributeCurved::GetControlPoints( void ) const
    {
        return mControlPoints;
    }

    void DiAttributeCurved::ProcessControlPoints( void )
    {
        if (mControlPoints.empty())
        {
            return;
        }

        Sort(mControlPoints.begin(), mControlPoints.end(), ControlPointSorter());
        mRange = (*GetLastValidIterator()).x - (*GetFirstValidIterator()).x;

        if (mInterpolationType == IT_SPLINE)
        {
            DiAttributeCurved::ControlPointList::iterator it;
            mSpline.Clear();
            for (it = mControlPoints.begin(); it != mControlPoints.end(); ++it)
            {
                mSpline.AddPoint(DiVec3((*it).x, (*it).y, 0));
            }
        }
    }

    size_t DiAttributeCurved::GetNumControlPoints( void ) const
    {
        return mControlPoints.size();
    }

    void DiAttributeCurved::RemoveAllControlPoints( void )
    {
        mControlPoints.clear();
    }

    void DiAttributeCurved::CopyTo( DiDynamicAttribute* dynamicAttribute )
    {
        if (!dynamicAttribute || dynamicAttribute->GetType() != DiDynamicAttribute::DAT_CURVED)
        {
            return;
        }

        DiAttributeCurved* dynAttr = static_cast<DiAttributeCurved*>(dynamicAttribute);
        dynAttr->mInterpolationType = mInterpolationType;
        dynAttr->mSpline = mSpline;
        dynAttr->mRange = mRange;

        DiAttributeCurved::ControlPointList::const_iterator it;
        DiAttributeCurved::ControlPointList::const_iterator itEnd = mControlPoints.end();
        for (it = mControlPoints.begin(); it != itEnd; ++it)
        {
            DiVec2 controlPoint = *it;
            dynAttr->mControlPoints.push_back(controlPoint);
        }
        dynAttr->ProcessControlPoints();
    }

    DiAttributeCurved::DiAttributeCurved( const DiAttributeCurved& dynamicAttributeCurved )
    {
        mType = DiDynamicAttribute::DAT_CURVED;
        mInterpolationType = dynamicAttributeCurved.mInterpolationType;
        mSpline = dynamicAttributeCurved.mSpline;
        mRange    = dynamicAttributeCurved.mRange;

        DiAttributeCurved::ControlPointList::const_iterator it;
        DiAttributeCurved::ControlPointList::const_iterator itEnd = dynamicAttributeCurved.mControlPoints.end();
        for (it = dynamicAttributeCurved.mControlPoints.begin(); it != itEnd; ++it)
        {
            DiVec2 controlPoint = *it;
            mControlPoints.push_back (controlPoint);
        }
        ProcessControlPoints();
    }

    float DiDynamicAttributeHelper::Calculate( DiDynamicAttribute* dyn, float x,float defaultValue /*= 0.0f*/ )
    {
        if (dyn)
        {
            return dyn->GetValue(x);
        }

        return defaultValue;
    }
}