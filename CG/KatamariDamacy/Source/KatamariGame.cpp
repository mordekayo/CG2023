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
	FGameObject* ground = new FGameObject();
	
	ground->CreatePlane(25.0f, "../KatamariDamacy/Textures/ice.png");
	ground->TransformComponent->SetRotation(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, -DirectX::XM_PIDIV2));
	
	FGameObject* katamari = new FGameObject();
	katamari->CreateMesh(50.0f, "../KatamariDamacy/Textures/katamari.png", "../KatamariDamacy/Models/katamari.obj");
	katamari->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0, 1, 0));
	KatamariControllerComponent* katamariController = new KatamariControllerComponent();
	katamariController->katamariSpeed = 5.0f;
	katamari->AddComponent(katamariController);
	katamari->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 15.0f));
	
	FGameObject* camera = new FGameObject();
	FCameraComponent* cameraComponent = new FCameraComponent();
	FCameraControllerComponent* armCameraController = new FCameraControllerComponent();
	camera->AddComponent(cameraComponent);
	armCameraController->aim = katamari->TransformComponent;
	camera->AddComponent(armCameraController);
	FGame::Instance()->currentCamera = cameraComponent;
	katamariController->cameraTransform = camera->TransformComponent;
	
	FGameObject* removeLight = new FGameObject();
	FDirectionalLightComponent* directionalLightComponent = new FDirectionalLightComponent(1024, 40.0f, 40.0f, 0.1f, 200.0f);
	removeLight->AddComponent(directionalLightComponent);
	FGame::Instance()->currentLight = directionalLightComponent;
	
	FGame::Instance()->AddGameObject(ground);      // 0
	FGame::Instance()->AddGameObject(camera);      // 1
	FGame::Instance()->AddGameObject(katamari);    // 2
	
	FGame::Instance()->AddGameObject(removeLight); // 3
	
	FGameObject* statue = new FGameObject();
	statue->CreateMesh(0.02f, "../KatamariDamacy/Textures/bull.jpg", "../KatamariDamacy/Models/bull.obj");
	statue->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	statue->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		* 
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(statue);

	FGameObject* spider0 = new FGameObject();
	spider0->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	spider0->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(10.0f, 5.0f, 0.0f));
	spider0->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(spider0);

	FGameObject* spider1 = new FGameObject();
	spider1->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	spider1->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(-10.0f, 5.0f, 0.0f));
	spider1->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(spider1);

	FGameObject* spider2 = new FGameObject();
	spider2->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	spider2->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 5.0f, 10.0f));
	spider2->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(spider2);

	FGameObject* spider3 = new FGameObject();
	spider3->CreateMesh(15.0f, "../KatamariDamacy/Textures/infrared.jpg", "../KatamariDamacy/Models/spider.fbx");
	spider3->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 5.0f, -10.0f));
	spider3->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Right, DirectX::XM_PIDIV2)
		*
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(spider3);
	
	FGame::Initialize();
}