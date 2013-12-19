
#include "TransformUnit.h"
#include "CullNode.h"
#include "SceneNode.h"

namespace Demi 
{
    DiTransformUnit::DiTransformUnit()
        : mQueryFlags(sDefaultQueryFlags)
        , mVisibilityFlags(sDefaultVisibilityFlags)
        , mShadowCastEnable(true)
        , mSceneNode(nullptr)
    {
        mParentNode = NULL;
        mGroupType = BATCH_MODEL;
        mVisible = true;
        mVisibilityFlags = std::numeric_limits<uint32>::max();
    }

    DiTransformUnit::DiTransformUnit( const DiString& name )
        : mSceneNode(nullptr)
    {
        mParentNode = NULL;
        mName = name;
        mGroupType = BATCH_MODEL;
        mVisible = true;
        mVisibilityFlags = std::numeric_limits<uint32>::max();
    }

    DiTransformUnit::~DiTransformUnit()
    {
        if (mParentNode)
        {
            DiCullNode* cn = dynamic_cast<DiCullNode*>(mParentNode);
            if (cn)
                cn->DetachObject(shared_from_this());
        }
    }

    void DiTransformUnit::SetSceneNode(DiSceneNode* node)
    {
        mSceneNode = node;
        OnNodeSet(mSceneNode);
    }

    const DiMat4& DiTransformUnit::GetTransform( void ) const
    {
        if (mSceneNode)
            return mSceneNode->GetWorldTransform();
        else if (mParentNode)
            return mParentNode->GetFullTransform();
        else
            return DiMat4::IDENTITY;
    }

    void DiTransformUnit::NotifyAttached( DiNode* parent )
    {
        mParentNode = parent;
    }

    const DiAABB& DiTransformUnit::GetWorldBoundingBox( bool derive /*= false*/ ) const
    {
        if (derive)
        {
            mWorldAABB = this->GetBoundingBox();
            mWorldAABB.TransformAffine(GetTransform());
        }

        return mWorldAABB;
    }

    void DiTransformUnit::SetBatchGroup( DiBatchGroupType gt )
    {
        mGroupType = gt;
    }

    void DiTransformUnit::SetVisible( bool visible )
    {
        mVisible = visible;
    }

    bool DiTransformUnit::GetVisible() const
    {
        return mVisible;
    }

    DiCullNode* DiTransformUnit::GetParentCullNode() const
    {
        DiCullNode* cn = dynamic_cast<DiCullNode*>(mParentNode);
        return cn;
    }

    void DiTransformUnit::Save( DiXMLElement node )
    {
        node.SetAttribute("type",GetType());

        if (!mVisible)
            node.SetBool("visible", mVisible);
        if (mVisibilityFlags != sDefaultVisibilityFlags)
            node.SetUInt("visibleFlag", mVisibilityFlags);
    }

    void DiTransformUnit::Load( DiXMLElement node )
    {
        if (node.HasAttribute("visible"))
            mVisible = node.GetBool("visible");

        if (node.HasAttribute("visibleFlag"))
            mVisibilityFlags = node.GetUint("visibleFlag");
    }

    const DiSphere& DiTransformUnit::GetWorldBoundingSphere( bool derive /*= false*/ ) const
    {
        return GetWorldBoundingSphere(mParentNode, derive);
    }

    const DiSphere& DiTransformUnit::GetWorldBoundingSphere(DiNode* parentNode, bool derive /*= false*/) const
    {
        DI_ASSERT(parentNode);
        if (derive)
        {
            DiAABB t = this->GetBoundingBox();
            DiVec3 hs = t.GetHalfSize();
            float radius = DiMath::Max(DiMath::Max(hs.x, hs.y), hs.z);

            const DiVec3& scl = parentNode->GetDerivedScale();
            float factor = DiMath::Max(DiMath::Max(scl.x, scl.y), scl.z);
            mWorldBoundingSphere.setRadius(radius * factor);
            mWorldBoundingSphere.setCenter(parentNode->GetDerivedPosition());
        }
        return mWorldBoundingSphere;
    }

    uint32 DiTransformUnit::sDefaultVisibilityFlags = 0xFFFFFFFF;
    uint32 DiTransformUnit::sDefaultQueryFlags = 0xFFFFFFFF;

}