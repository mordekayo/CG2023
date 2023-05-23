#include "KatamariGame.h"
#include "GameObjects/GameObject.h"
#include "Components/CameraComponent.h"
#include "Components/CameraControllerComponent.h"
#include "KatamariControllerComponent.h"
#include "Components/TransformComponent.h"
#include "Components/Light/DirectionalLightComponent.h"

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
	
	FGameObject* RemoveLight = new FGameObject();
	FDirectionalLightComponent* DirectionalLightComponent = new FDirectionalLightComponent(1024, 40.0f, 40.0f, 0.1f, 200.0f);
	RemoveLight->AddComponent(DirectionalLightComponent);
	FGame::Instance()->CurrentLight = DirectionalLightComponent;
	
	FGame::Instance()->AddGameObject(Ground);      // 0
	FGame::Instance()->AddGameObject(Camera);      // 1
	FGame::Instance()->AddGameObject(Katamari);    // 2
	
	FGame::Instance()->AddGameObject(RemoveLight); // 3
	
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