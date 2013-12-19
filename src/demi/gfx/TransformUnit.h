
/********************************************************************
    File:       TransformUnit.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "RenderBatchGroup.h"
#include "XMLElement.h"

namespace Demi 
{
    typedef shared_ptr<DiTransformUnit> DiTransUnitPtr;

    class DI_GFX_API DiTransformUnit : public DiBase, public enable_shared_from_this<DiTransformUnit>
    {
    public:

        DiTransformUnit();

        DiTransformUnit(const DiString& name);

        virtual                    ~DiTransformUnit();

        friend class DiSceneNode;

    public:

        virtual void            SetSceneNode(DiSceneNode* node);

        virtual const DiMat4&   GetTransform(void) const;

        virtual void            NotifyAttached(DiNode* parent);

        DiNode*                 GetParentNode() const { return mParentNode; }

        DiCullNode*             GetParentCullNode() const;

        const DiString&         GetName(void) const { return mName; }

        bool                    IsAttached()const {return mParentNode!=NULL;}

        virtual void            SetVisible(bool visible);

        virtual bool            GetVisible() const;

        virtual void            SetBatchGroup(DiBatchGroupType gt);
        
        DiBatchGroupType        GetBatchGroup() const { return mGroupType; }

        virtual    void         CullingUpdate(DiRenderBatchGroup*,DiCamera*){}

        virtual void            Update(DiCamera* camera) {}

        virtual void            AddToBatchGroup(DiRenderBatchGroup* bg) {}

        virtual const DiAABB&   GetBoundingBox(void) const = 0;

        virtual const DiAABB&   GetWorldBoundingBox(bool derive = false) const;

        virtual const DiSphere& GetWorldBoundingSphere(bool derive = false) const;

        virtual const DiSphere& GetWorldBoundingSphere(DiNode* parentNode, bool derive = false) const;

        virtual void            NotifyCurrentCamera(DiCamera*){}

        virtual DiString&       GetType() = 0;

        virtual void            Save(DiXMLElement node);

        virtual void            Load(DiXMLElement node);

        virtual void            SetQueryFlags(uint32 flags) { mQueryFlags = flags; }

        virtual void            AddQueryFlags(uint32 flags) { mQueryFlags |= flags; }
            
        virtual void            RemoveQueryFlags(uint32 flags) { mQueryFlags &= ~flags; }
        
        virtual uint32          GetQueryFlags(void) const { return mQueryFlags; }

        static void             SetDefaultQueryFlags(uint32 flags) { sDefaultQueryFlags = flags; }

        static uint32           GetDefaultQueryFlags() { return sDefaultQueryFlags; }

        virtual void            SetVisibilityFlags(uint32 flags) { mVisibilityFlags = flags; }

        virtual void            AddVisibilityFlags(uint32 flags) { mVisibilityFlags |= flags; }
            
        virtual void            RemoveVisibilityFlags(uint32 flags) { mVisibilityFlags &= ~flags; }
        
        virtual uint32          GetVisibilityFlags(void) const { return mVisibilityFlags; }

        virtual bool            GetShadowCastEnable()const{return mShadowCastEnable;}

        virtual void            SetShadowCastEnable(bool val){mShadowCastEnable = val;}

        static void             SetDefaultVisibilityFlags(uint32 flags) { sDefaultVisibilityFlags = flags; }

        static uint32           GetDefaultVisibilityFlags() { return sDefaultVisibilityFlags; }

    protected:

        virtual void            OnNodeSet(DiSceneNode* node) {}

        virtual void            OnMarkedDirty(DiSceneNode* node){}

    protected:

        DiString                mName;

        DiNode*                 mParentNode;

        DiSceneNode*            mSceneNode;

        mutable DiAABB          mWorldAABB;

        mutable DiSphere        mWorldBoundingSphere;

        DiBatchGroupType        mGroupType;

        bool                    mVisible;

        bool                    mShadowCastEnable;

        uint32                  mQueryFlags;

        uint32                  mVisibilityFlags;

        static uint32           sDefaultQueryFlags;

        static uint32           sDefaultVisibilityFlags;
    };

}