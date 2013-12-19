
/********************************************************************
    File:       AttachSet.h
    Creator:    demiwangya
*********************************************************************/

#pragma once 

#include "AttachNode.h"

namespace Demi
{
    class DI_GFX_API DiAttachSet
    {
    public:

        typedef DiHashMap<DiString,DiAttachNode * >        AttachNodeMap;
        typedef DiMapIterator<AttachNodeMap>            AttachNodeMapIterator;

        typedef DiVector<DiAttachNode *>                RootAttachList;
        typedef DiVectorIterator<RootAttachList>        RootAttachListIterator;

        friend class DiAttachSetInstance;

    public:

        DiAttachSet();

        virtual                      ~DiAttachSet();

        void                         ClearAttaches();

        void                         Reset();

        const AttachNodeMap&         GetAttaches() const { return mMapAttachNodes; }

        AttachNodeMap&               GetAttaches()        { return mMapAttachNodes; }

        size_t                       GetNumAttaches() const { return mMapAttachNodes.size(); }

        DiAttachNode*                CreateAttachNode(const DiString& strAttach);

        DiAttachNode*                GetAttachNode(const DiString& strAttach);

        const DiAttachNode*          GetAttachNode(const DiString& strAttach) const;

        RootAttachListIterator       GetRootAttechesIterator(void);

        void                         UpdateTransforms(void);
                                     
        void                         SetBindingPose(void);
                                     
        void                         GetAttachMatrices(Demi::DiMat4 * pMatrices,size_t unArrayLen);
                                     
        bool                         HasAttachNode(const DiString& strAttach);

    protected:

        void                         DeriveRootAttaches() const;

    protected:

        AttachNodeMap                mMapAttachNodes;

        mutable RootAttachList       mVecRootAttaches;
    };


    class DiAttachSetInstance : public DiAttachSet
    {
    public:

        DiAttachSetInstance(DiAttachSet* pkTemplet,DiSkeletonInstance * pSkeleton);

        virtual             ~DiAttachSetInstance();

    protected:

        void                Init();

        void                CloneAttachNodes(DiAttachNode * pkSource, DiNode * pkParentNode);

    protected:

        DiAttachSet*            mTemplet;

        DiSkeletonInstance *    mSkeleton;
    };
}