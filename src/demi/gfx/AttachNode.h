
/********************************************************************
    File:       AttachNode.h
    Creator:    demiwangya
*********************************************************************/
#pragma once

#include "Node.h"
#include "Bone.h"

namespace Demi
{
    class DI_GFX_API DiAttachNode : public DiNode
    {
    public:

        DiAttachNode(DiString strName);

        virtual                    ~DiAttachNode();

    public:

        DiAttachNode*              CreateChild(DiString strName);

        DiNode*                    CreateChild(const DiVec3& vecTranslate = DiVec3::ZERO, 
                                            const DiQuat& vecRotate = DiQuat::IDENTITY );

        DiNode*                    CreateChild(const DiString& strName, 
                                            const DiVec3& vecTranslate,
                                            const DiQuat& vecRotate);

        inline    const DiString&  GetAttachName()    {return mName;}

        inline    void             RenameAttach(const DiString& strNewName)    {mName = strNewName;}

        void                       SetBindingPose();
                                   
        void                       Reset();
                                   
        void                       GetOffsetTransform( DiMat4& matRes );
                                   
        void                       NeedUpdate(bool forceParentUpdate = false);

        inline    const DiVec3&    GetBindingPoseInverseScale(void) const { return mBindDerivedInverseScale; }

        inline    const DiVec3&    GetBindingPoseInversePosition(void) const { return mBindDerivedInversePosition; }

        inline    const DiQuat&    GetBindingPoseInverseOrientation(void) const { return mBindDerivedInverseOrientation; }

    protected:

        DiVec3                     mBindDerivedInverseScale;

        DiQuat                     mBindDerivedInverseOrientation;

        DiVec3                     mBindDerivedInversePosition;
    };
}
