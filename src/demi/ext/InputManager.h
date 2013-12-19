
/********************************************************************
File:       Mouse.h
Creator:    InputManager.h
*********************************************************************/

#pragma once

#include "MyGUI_KeyCode.h"
#include "MyGUI_MouseButton.h"
#include "MyGUI_Types.h"
#include "OIS.h"

namespace Demi
{
    class DiInputManager : public OIS::MouseListener, public OIS::KeyListener, public DiBase
    {
    public:
        DiInputManager();
        virtual ~DiInputManager();

        typedef Functor1<const OIS::MouseEvent&> MouseMoveCallback;
        typedef Functor2<const OIS::MouseEvent&, OIS::MouseButtonID> MousePressCallback;
        typedef Functor2<const OIS::MouseEvent&, OIS::MouseButtonID> MouseReleaseCallback;
        typedef Functor1<const OIS::KeyEvent&> KeyPressCallback;
        typedef Functor1<const OIS::KeyEvent&> KeyReleaseCallback;

        void createInput(size_t _handle);
        void destroyInput();
        void captureInput();
        void setInputViewSize(int _width, int _height);

        bool registerMouseMoveEvent(const DiString& name, MouseMoveCallback cb);
        bool registerMousePressEvent(const DiString& name, MousePressCallback cb);
        bool registerMouseReleaseEvent(const DiString& name, MouseReleaseCallback cb);
        bool registerKeyPressEvent(const DiString& name, KeyPressCallback cb);
        bool registerKeyReleaseEvent(const DiString& name, KeyReleaseCallback cb);

        bool unregisterMouseMoveEvent(const DiString& name);
        bool unregisterMousePressEvent(const DiString& name);
        bool unregisterMouseReleaseEvent(const DiString& name);
        bool unregisterKeyPressEvent(const DiString& name);
        bool unregisterKeyReleaseEvent(const DiString& name);

        void injectMouseMove(int _absx, int _absy, int _absz);
        void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
        void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
        void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
        void injectKeyRelease(MyGUI::KeyCode _key);

        void setMousePosition(int _x, int _y);
        void updateCursorPosition();

        OIS::Keyboard* getKeyboard() { return mKeyboard; }
        OIS::Mouse* getMouse() { return mMouse; }

    private:
        bool mouseMoved(const OIS::MouseEvent& _arg);
        bool mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
        bool keyPressed(const OIS::KeyEvent& _arg);
        bool keyReleased(const OIS::KeyEvent& _arg);

        void checkPosition();

    private:
        OIS::InputManager* mInputManager;
        OIS::Keyboard* mKeyboard;
        OIS::Mouse* mMouse;

        DiMap<DiString, MouseMoveCallback> mMouseMoves;
        DiMap<DiString, MousePressCallback> mMousePresses;
        DiMap<DiString, MouseReleaseCallback> mMouseReleases;
        DiMap<DiString, KeyPressCallback> mKeyPresses;
        DiMap<DiString, KeyReleaseCallback> mKeyReleases;

        int mCursorX;
        int mCursorY;
    };

} 