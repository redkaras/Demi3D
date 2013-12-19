
#include "DiPch.h"
#include "Application.h"
#include "Skybox.h"
#include "SimpleShape.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShaderManager.h"
#include "RenderWindow.h"
#include "CameraHelper.h"
#include "PostEffectManager.h"
#include "PostEffect.h"
#include "PostEffectPass.h"
#include "GfxDriver.h"

float timeElap = 0;
DiShaderParameter* shaderParam = nullptr;
DiCullNode* node = nullptr;

void AddMeshes()
{
	DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	DiMaterialPtr m0 = DiMaterial::QuickCreate("lava.vsh","lava.psh");
	m0->SetDiffuse(DiColor(1,0,0));
	m0->SetAmbient(DiColor(0.6f,0.2f,0));
	
	shaderParam = m0->GetShaderParameter();
	shaderParam->WriteFloat("fogDensity",0.45f);
	shaderParam->WriteFloat2("uvScale",DiVec2(3,1));
	shaderParam->WriteFloat3("fogColor",DiVec3(0,0,0));
	shaderParam->WriteTexture2D("texture1","cloud.dds");
	shaderParam->WriteTexture2D("texture2","lavatile.dds");

	DiSimpleShapePtr model = make_shared<DiSimpleShape>();
	model->CreateTorus(16,8,32,32);
	model->SetMaterial(m0);
	node = sm->GetRootNode()->CreateChild();
	node->AttachObject(model);
}

void SetupPostEffects()
{
	DiPostEffectManager* peMgr = DiBase::Driver->GetMainRenderWindow()->GetPostEffectManager();
	DiPostEffect* bloom = peMgr->GetEffect("Bloom");
	if (bloom)
		bloom->SetEnable(true);

	DiPostEffect* effectFilm = peMgr->CreatePostEffect("Film");
	effectFilm->SetEnable(true);

  	DiPostEffectPass* passFilm = effectFilm->CreatePass("output");
 	passFilm->SetQuadScale(1);
  	passFilm->BuildMaterial("screen_quad.vsh","post_film.psh");
	passFilm->SetInput("image",INPUT_PREV_EFFECT);
	passFilm->SetInput("nIntensity",0.35f);
	passFilm->SetInput("sIntensity",0.95f);
	passFilm->SetInput("sCount",2048);
}

void InitScene()
{
    DiSceneManager* sm = DiBase::Driver->GetSceneManager();

	DiApp::GetApp()->GetCameraHelper()->Enable(false);
	sm->GetCamera()->SetPosition(0,0,80);
	sm->GetCamera()->LookAt(0,0,0);

	AddMeshes();

	SetupPostEffects();
}

void UpdateScene()
{
	// update the material
    float delta = DiBase::Driver->GetDeltaSecond();
	timeElap += delta;
	shaderParam->WriteFloat("time",timeElap);

	node->Yaw(DiRadian(0.0625f * delta));
	node->Pitch(DiRadian(0.25f * delta));
}

int main()
{
	DiApp app;
	app.SetInitCallback(functor(InitScene));
	app.SetUpdateCallback(functor(UpdateScene));
	app.Open(1500, 800, "Demi3D Sample - Lava Material");

	while (app.IsOpen())
	{
		app.Update();
	}

	return 0;
}