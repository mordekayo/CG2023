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
	FDirectionalLightComponent* DirectionalLightComponent = new FDirectionalLightComponent(1024, 40.0f, 40.0f, 0.1f, 200.0f);
	DirectionalLightComponent->Direction = DirectX::SimpleMath::Vector4(DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.3f));
	DirectionalLight->AddComponent(DirectionalLightComponent);
	FGame::Instance()->DirectionalLight = DirectionalLightComponent;
	
	FGameObject* pointLight0 = new FGameObject();
	pointLight0->CreateMesh(1.0f, "../KatamariDamacy/Textures/LampAlbedo.png", "../KatamariDamacy/Models/lamp.obj");
	pointLight0->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(4, 4, 4));
	pointLight0->MeshComponent->Material.DiffuseReflectionCoefficient  = { 1.0f, 1.0f, 1.0f };
	pointLight0->MeshComponent->Material.AbsorptionCoef = { 1.0f, 1.0f, 1.0f };
	pointLight0->MeshComponent->Material.AmbientConstant  = { 1.0f, 1.0f, 1.0f };
	FPointLightComponent* pointLightComponent0 = new FPointLightComponent(1.0f, 0.09f, 0.032f);
	pointLightComponent0->Color = DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
	pointLightComponent0->PoiAddMesh(5.0f, "../KatamariDamacy/Models/lightSphere.fbx");
	FGameObject* pointLightSphere0 = new FGameObject();
	pointLightSphere0->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(4, 4, 4));
	pointLightSphere0->AddComponent(pointLightComponent0);
	FGame::Instance()->PointLights.push_back(pointLightComponent0);

	FGameObject* pointLight1 = new FGameObject();
	pointLight1->CreateMesh(1.0f, "../KatamariDamacy/Textures/LampAlbedo.png", "../KatamariDamacy/Models/lamp.obj");
	pointLight1->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(-15, 4, 15));
	pointLight1->MeshComponent->Material.DiffuseReflectionCoefficient = { 1.0f, 1.0f, 1.0f };
	pointLight1->MeshComponent->Material.AbsorptionCoef = { 1.0f, 1.0f, 1.0f };
	pointLight1->MeshComponent->Material.AmbientConstant = { 1.0f, 1.0f, 1.0f };
	FPointLightComponent* pointLightComponent1 = new FPointLightComponent(1.0f, 0.09f, 0.032f);
	pointLightComponent1->Color = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	pointLightComponent1->PoiAddMesh(5.0f, "../KatamariDamacy/Models/lightSphere.fbx");
	FGameObject* pointLightSphere1 = new FGameObject();
	pointLightSphere1->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(-15, 4, 15));
	pointLightSphere1->AddComponent(pointLightComponent1);
	FGame::Instance()->PointLights.push_back(pointLightComponent1);
	
	FGame::Instance()->AddGameObject(Ground);
	FGame::Instance()->AddGameObject(Camera);
	FGame::Instance()->AddGameObject(Katamari);
	FGame::Instance()->AddGameObject(DirectionalLight);
	FGame::Instance()->AddGameObject(pointLight0);
	FGame::Instance()->AddGameObject(pointLightSphere0);
	FGame::Instance()->AddGameObject(pointLight1);
	FGame::Instance()->AddGameObject(pointLightSphere1);
		
	FGameObject* Pig = new FGameObject();
	Pig->CreateMesh(1.0f, "../KatamariDamacy/Textures/pig.jpg", "../KatamariDamacy/Models/pig.fbx");
	Pig->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	Pig->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Left, DirectX::XM_PI)
	);
	FGame::Instance()->AddGameObject(Pig);

	FGameObject* Skat = new FGameObject();
	Skat->CreateMesh(0.1f, "../KatamariDamacy/Textures/skat.png", "../KatamariDamacy/Models/skat.fbx");
	Skat->TransformComponent->SetPosition(DirectX::SimpleMath::Vector3(15.0f, 15.0f, 15.0f));
	Skat->TransformComponent->SetRotation(
		DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.0f, 120.0f, 0.0f)
	);
	FGame::Instance()->AddGameObject(Skat);
	
	FGame::Initialize();
}