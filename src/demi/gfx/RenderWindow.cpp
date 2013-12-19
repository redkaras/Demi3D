
#include "RenderWindow.h"
#include "GfxDriver.h"
#include "RenderTarget.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "GBuffer.h"
#include "RenderPipeline.h"
#include "PostEffectManager.h"
#include "Window.h"

namespace Demi 
{
    DiRenderWindow* DiRenderWindow::ActiveWindow = nullptr;

    DiRenderWindow::DiRenderWindow(const DiString& name)
        : mSceneManager(nullptr)
        , mSceneCanvas(nullptr)
        , mRenderBuffer(nullptr)
        , mMainCamera(nullptr)
        , mPostEffectMgr(nullptr)
        , mName(name)
        , mEnable(true)
        , mCurrentBatchesNum(0)
        , mCurrentPrimitivesNum(0)
        , mGBuffer(nullptr)
        , mWidth(0)
        , mHeight(0)
        , mWndHandle(nullptr)
        , mWindow(nullptr)
    {
    }

    DiRenderWindow::~DiRenderWindow()
    {
        Shutdown();
    }

    void DiRenderWindow::AddUpdateRenderTarget( DiRenderTarget* rt )
    {
        if (mUpdateRTs.contains(rt))
            return;
        mUpdateRTs.push_back(rt);
    }

    void DiRenderWindow::RemoveUpdateRenderTarget( DiRenderTarget* rt )
    {
        UpdateRTs::iterator it = mUpdateRTs.find(rt);
        if (it != mUpdateRTs.end())
            mUpdateRTs.erase(it);
    }

    void DiRenderWindow::Render()
    {
        if (!mEnable)
            return;

        DI_PROFILE(RenderWindow_Render);

        if (mWindow)
            mWindow->Update();

        mCurrentPrimitivesNum = 0;
        mCurrentBatchesNum = 0;

        // cull and update
        DiCamera* mainCam = mSceneManager->GetCamera();
        mSceneManager->PreUpdate();
        mSceneManager->Cull(mainCam);
        mSceneManager->GetVisibleObjects().UpdateAll(mainCam);

        Driver->BeginFrame();

        // TODO: update RTs
       
        DiRenderPipeline* rp = Driver->GetPipeline();
        rp->ClearGroup();
        mSceneManager->GetVisibleObjects().AddToBatch(rp);
        rp->Render(mSceneManager, mainCam, mSceneCanvas);

        Driver->EndFrame();
        
        SwapBuffer();
    }

    void DiRenderWindow::Create(uint32 width, uint32 height, const DiString& title, bool fullscreen)
    {
        mWindow = DiWindow::CreateWnd();
        mWindow->SetParentRenderWnd(this);
        mWindow->Create(width, height, title.c_str(), fullscreen);
        mWndHandle = mWindow->GetWndHandle();

        Init();
    }

    void DiRenderWindow::Create(DiWndHandle hwnd)
    {
        DI_ASSERT(hwnd);
        mWndHandle = hwnd;

        Init();
    }

    void DiRenderWindow::Init()
    {
        DI_LOG("Intializing render window : %s", mName.c_str());

        Driver->GetWindowDimension(mWndHandle, mWidth, mHeight);

        mSceneManager = DI_NEW DiSceneManager(this);
        Driver->CreateWindowTarget(mRenderBuffer, mWndHandle);
        mMainCamera = mSceneManager->GetCamera();

        DiString canvasName = "_canvas_" + mName;
        mCanvasTexture = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(canvasName);
        mCanvasTexture->SetDimensions(mWidth, mHeight);
        mCanvasTexture->SetFormat(PF_X8R8G8B8);
        mCanvasTexture->SetResourceUsage(RU_NONE);
        mCanvasTexture->SetUsage(TU_RENDER_TARGET);
        mCanvasTexture->CreateTexture();
        mSceneCanvas = mCanvasTexture->GetRenderTarget();
        mCanvasTexture->SetAdaptedRT(mRenderBuffer);
        mCanvasTexture->SetViewportScale(DiVec2::UNIT_SCALE);

        DiString depStencilName = "_can_ds_" + mName;
        mDepthStencil = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(depStencilName);
        mDepthStencil->SetDimensions(mWidth, mHeight);
        mDepthStencil->SetFormat(PF_D24S8);
        mDepthStencil->SetUsage(TU_DEPTH_STENCIL);
        mDepthStencil->SetResourceUsage(RU_NONE);
        mDepthStencil->CreateTexture();
        mDepthStencil->SetAdaptedRT(mRenderBuffer);

        mPostEffectMgr = DI_NEW DiPostEffectManager(this);
        mGBuffer = DI_NEW DiGBuffer(this);
    }

    void DiRenderWindow::Shutdown()
    {
        DI_LOG("Closing render window : %s", mName.c_str());

        DiAssetManager::GetInstance().DestroyAsset(mCanvasTexture->GetName());
        DiAssetManager::GetInstance().DestroyAsset(mDepthStencil->GetName());

        SAFE_DELETE(mGBuffer);
        SAFE_DELETE(mPostEffectMgr);
        SAFE_DELETE(mSceneManager);
        SAFE_DELETE(mRenderBuffer);
        SAFE_DELETE(mWindow);
    }

    void DiRenderWindow::OnDeviceLost()
    {
        Driver->GetWindowDimension(mWndHandle, mWidth, mHeight);
    }

    void DiRenderWindow::OnDeviceReset()
    {
        if (mSceneManager)
            mSceneManager->GetCamera()->SetAspectRatio(float(GetWidth())/float(GetHeight()));
    }

    uint32 DiRenderWindow::GetWidth()
    {
        return mWidth;
    }

    uint32 DiRenderWindow::GetHeight()
    {
        return mHeight;
    }

    void DiRenderWindow::SwapBuffer()
    {
        DI_PROFILE(RenderWindow_SwapBuffer);
        Driver->SwapBuffer(mRenderBuffer);
    }

    void DiRenderWindow::Closing()
    {
        Driver->DestroyRenderWindow(mWndHandle);
        mWndHandle = nullptr;
    }
}