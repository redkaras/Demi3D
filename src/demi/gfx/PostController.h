
/********************************************************************
    File:           PostController.h
    Creator:        demiwangya
*********************************************************************/

#pragma once

namespace Demi
{
    class DI_GFX_API DiPostController : public DiBase
    {
    public:

        DiPostController(DiAnimModel* parent);

        virtual                 ~DiPostController();

    public:

        bool                    IsEnabled() const { return mEnabled; }

        void                    SetEnabled(bool val) { mEnabled = val; }

        virtual void            Update(float deltaTime) = 0;

    public:

        bool                    mEnabled;

        DiAnimModel*            mParent;
    };
}