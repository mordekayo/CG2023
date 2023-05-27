#include "KatamariGame.h"
#include "GameObjects/GameObject.h"
#include "Components/CameraComponent.h"
#include "Components/CameraControllerComponent.h"
#include "KatamariControllerComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Components/Light/DirectionalLightComponent.h"
#include "Components/Light/PointLightComponent.h"

KatamariGame* KatamariGame::Instance()
{
	if (!GameInstance)
	{
		GameInstance = new KatamariGame();
	}
	return static_cast<KatamariGame*>(GameInstance);
}

KatamariGame::KatamariGame()
{
	Initialize();
}

void KatamariGame::Initialize()
{
	FGameObject* Ground = new FGameObject();
	
	Ground->CreatePlane(400.0f, "../KatamariDamacy/Textures/white.jpg");
	Ground->TransformComponent->SetRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, -DirectX::XM_PIDIV2));
	
	FGameObject* Katamari = new FGameObject();
	Katamari->CreateMesh(1.0f, "../KatamariDamacy/Textures/8ball.jpg", "../KatamariDamacy/Models/8ball.obj");
	Katamari->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0, 1, 0));
	
	KatamariControllerComponent* KatamariController = new KatamariControllerComponent();
	KatamariController->KatamariSpeed = 5.0f;
	Katamari->AddComponent(KatamariController);
	
	Katamari->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 15.0f));
	
	FGameObject* Camera = new FGameObject();
	FCameraComponent* CameraComponent = new FCameraComponent();
	FCameraControllerComponent* ArmCameraController = new FCameraControllerComponent();
	Camera->AddComponent(CameraComponent);
	ArmCameraController->Target = Katamari->TransformComponent;
	Camera->AddComponent(ArmCameraController);
	FGame::Instance()->CurrentCamera = CameraComponent;
	KatamariController->CameraTransform = Camera->TransformComponent;
	
	FGameObject* DirectionalLight = new FGameObject();
	DirectionalLight->CreatePlane(0.001f, "../KatamariDamacy/Textures/white.png");
	FDirectionalLightComponent* DirectionalLightComponent = new FDirectionalLightComponent(1024, 40.0f, 40.0f, 0.1f, 200.0f);
	DirectionalLight->AddComponent(DirectionalLightComponent);
	DirectionalLight->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0, 10, 0));
	DirectionalLight->TransformComponent->SetRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2));
	FGame::Instance()->DirectionalLight = DirectionalLightComponent;

	FGameObject* PointLight0 = new FGameObject();
	PointLight0->CreateMesh(0.2f, "../Textures/LampAlbedo.png", "../Models/lamp.obj");
	PointLight0->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(15, 1, 15));
	FPointLightComponent* PointLightComponent = new FPointLightComponent(1.0f, 0.09f, 0.032f);
	PointLightComponent->Color = DirectX::SimpleMath::Vector3( 1.0f, 0.0f, 0.0f);
	PointLight0->AddComponent(PointLightComponent);
	PointLight0->MeshComponent->Material.DiffuseReflectionCoefficient  = { 1.0f, 1.0f, 1.0f };
	PointLight0->MeshComponent->Material.AbsorptionCoef = { 1.0f, 1.0f, 1.0f };
	PointLight0->MeshComponent->Material.AmbientConstant  = { 1.0f, 1.0f, 1.0f };
	FGame::Instance()->PointLights.push_back(PointLightComponent);

	FGameObject* PointLight1 = new FGameObject();
	PointLight1->CreateMesh(0.2f, "../Textures/LampAlbedo.png", "../Models/lamp.obj");
	PointLight1->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(-15, 1, 15));
	FPointLightComponent* PointLightComponent1 = new FPointLightComponent(1.0f, 0.09f, 0.032f);
	PointLightComponent->Color = DirectX::SimpleMath::Vector3( 1.0f, 0.0f, 0.0f);
	PointLight1->AddComponent(PointLightComponent);
	PointLight1->MeshComponent->Material.DiffuseReflectionCoefficient  = { 1.0f, 1.0f, 1.0f };
	PointLight1->MeshComponent->Material.AbsorptionCoef = { 1.0f, 1.0f, 1.0f };
	PointLight1->MeshComponent->Material.AmbientConstant  = { 1.0f, 1.0f, 1.0f };
	FGame::Instance()->PointLights.push_back(PointLightComponent1);
	
	FGame::Instance()->AddGameObject(Ground);      // 0
	FGame::Instance()->AddGameObject(Camera);      // 1
	FGame::Instance()->AddGameObject(Katamari);    // 2
	FGame::Instance()->AddGameObject(DirectionalLight); // 3
	FGame::Instance()->AddGameObject(PointLight0);
	FGame::Instance()->AddGameObject(PointLight1);
		
	FGameObject* Statue = new FGameObject();
	Statue->CreateMesh(1.0f, "../KatamariDamacy/Textures/pig.jpg", "../KatamariDamacy/Models/pig.fbx");
	Statue->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	Statue->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Left, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(Statue);

	FGameObject* Deer = new FGameObject();
	Deer->CreateMesh(0.1f, "../KatamariDamacy/Textures/skat.png", "../KatamariDamacy/Models/skat.fbx");
	Deer->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(15.0f, 15.0f, 15.0f));
	Deer->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0f, 120.0f, 0.0f)
	);
	FGame::Instance()->AddGameObject(Deer);
	
	FGame::Initialize();
}