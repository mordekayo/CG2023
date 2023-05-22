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
	
	Ground->CreatePlane(25.0f, "../KatamariDamacy/Textures/ice.png");
	Ground->TransformComponent->SetRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, -DirectX::XM_PIDIV2));
	
	FGameObject* Katamari = new FGameObject();
	Katamari->CreateMesh(50.0f, "../KatamariDamacy/Textures/katamari.png", "../KatamariDamacy/Models/katamari.obj");
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
	Statue->CreateMesh(0.02f, "../KatamariDamacy/Textures/bull.jpg", "../KatamariDamacy/Models/bull.obj");
	Statue->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	Statue->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		* 
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(Statue);

	FGameObject* Spider0 = new FGameObject();
	Spider0->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	Spider0->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 5.0f, 0.0f));
	Spider0->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(Spider0);

	FGameObject* Spider1 = new FGameObject();
	Spider1->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	Spider1->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(-10.0f, 5.0f, 0.0f));
	Spider1->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(Spider1);

	FGameObject* Spider2 = new FGameObject();
	Spider2->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	Spider2->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 5.0f, 10.0f));
	Spider2->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(Spider2);

	FGameObject* Spider3 = new FGameObject();
	Spider3->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	Spider3->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 5.0f, -10.0f));
	Spider3->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(Spider3);
	
	FGame::Initialize();
}