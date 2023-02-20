#include "TriangleComponent.h"

TriangleComponent::TriangleComponent() {

	vertexBufDesc = std::make_shared<D3D11_BUFFER_DESC>();
	vertexData = std::make_shared<D3D11_SUBRESOURCE_DATA>();

	indexBufDesc = std::make_shared<D3D11_BUFFER_DESC>();
	indexData = std::make_shared<D3D11_SUBRESOURCE_DATA>();

	rastDesc = std::make_shared<CD3D11_RASTERIZER_DESC>();

	constBufDesc = std::make_shared<D3D11_BUFFER_DESC>();

	viewport = std::make_shared<D3D11_VIEWPORT>();

}

void TriangleComponent::Initialize()
{
	ID3DBlob* errorVertexCode = nullptr;
	Game::GetInstance()->SetRes(
		D3DCompileFromFile(
			L"./Shaders/MyVeryFirstShader.hlsl",
			nullptr /*macros*/,
			nullptr /*include*/,
			"VSMain",
			"vs_5_0", // Shader target (Pixel / Vertex)
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Different flags (for example code translation)
			0,
			vertexShaderByteCode.GetAddressOf(),
			&errorVertexCode
		)
	);
	if (FAILED(Game::GetInstance()->GetRes()))
	{
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode)
		{
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(Game::GetInstance()->GetDisplay()->GetHWnd(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}
		return;
	}
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(1.0f, 1.0f, 1.0f, 1.0f)", nullptr, nullptr };
	Game::GetInstance()->GetDevice()->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, vertexShader.GetAddressOf()
	);

	ID3DBlob* errorPixelCode = nullptr;
	Game::GetInstance()->SetRes(
		D3DCompileFromFile(
			L"./Shaders/MyVeryFirstShader.hlsl",
			Shader_Macros /*macros*/,
			nullptr /*include*/,
			"PSMain",
			"ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			pixelShaderByteCode.GetAddressOf(),
			&errorPixelCode
		)
	);
	Game::GetInstance()->GetDevice()->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, pixelShader.GetAddressOf()
	);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, // Per vertex or per instance
			0
		}
	};
	Game::GetInstance()->GetDevice()->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		layout.GetAddressOf()
	);

	pointsAmount = 8;
	points = new DirectX::XMFLOAT4[pointsAmount]; // ������� - ����
	points[0] = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);   points[1] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	points[2] = DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f); points[3] = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	points[4] = DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f);  points[5] = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	points[6] = DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f);  points[7] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	vertexBufDesc.get()->Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.get()->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.get()->CPUAccessFlags = 0;
	vertexBufDesc.get()->MiscFlags = 0;
	vertexBufDesc.get()->StructureByteStride = 0;
	vertexBufDesc.get()->ByteWidth = sizeof(DirectX::XMFLOAT4) * pointsAmount;

	vertexData.get()->pSysMem = points;
	vertexData.get()->SysMemPitch = 0;
	vertexData.get()->SysMemSlicePitch = 0;
	Game::GetInstance()->GetDevice()->CreateBuffer(vertexBufDesc.get(), vertexData.get(), vb.GetAddressOf());
	strides[0] = 32;
	offsets[0] = 0;

	int indeces[] = { 0, 1, 2, 1, 0, 3 };

	indexBufDesc.get()->Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.get()->BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.get()->CPUAccessFlags = 0;
	indexBufDesc.get()->MiscFlags = 0;
	indexBufDesc.get()->StructureByteStride = 0;
	indexBufDesc.get()->ByteWidth = sizeof(int) * std::size(indeces);

	indexData.get()->pSysMem = indeces;
	indexData.get()->SysMemPitch = 0;
	indexData.get()->SysMemSlicePitch = 0;
	Game::GetInstance()->GetDevice()->CreateBuffer(indexBufDesc.get(), indexData.get(), ib.GetAddressOf());

	rastDesc.get()->CullMode = D3D11_CULL_NONE;
	rastDesc.get()->FillMode = D3D11_FILL_SOLID;
	Game::GetInstance()->SetRes(Game::GetInstance()->GetDevice()->CreateRasterizerState(rastDesc.get(), rastState.GetAddressOf()));

	Game::GetInstance()->GetContext()->RSSetState(rastState.Get());

	// MOVEMENT //
	constBufDesc.get()->Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.get()->BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.get()->CPUAccessFlags = 0;
	constBufDesc.get()->MiscFlags = 0;
	constBufDesc.get()->StructureByteStride = 0;
	constBufDesc.get()->ByteWidth = sizeof(DirectX::SimpleMath::Vector4);
	Game::GetInstance()->GetDevice()->CreateBuffer(constBufDesc.get(), nullptr, constBuf.GetAddressOf());
	// MOVEMENT //
}

void TriangleComponent::Update()
{
	// MOVEMENT //
	Game::GetInstance()->GetContext()->UpdateSubresource(constBuf.Get(), 0, nullptr, Game::GetInstance()->GetOffset(), 0, 0);
	// MOVEMENT //

	Game::GetInstance()->GetContext()->ClearState();

	Game::GetInstance()->GetContext()->RSSetState(rastState.Get());

	viewport.get()->Width = static_cast<float>(Game::GetInstance()->GetDisplay()->GetScreenWidth());
	viewport.get()->Height = static_cast<float>(Game::GetInstance()->GetDisplay()->GetScreenHeight());
	viewport.get()->MinDepth = 0;
	viewport.get()->MaxDepth = 1.0f;
	viewport.get()->TopLeftX = 0;
	viewport.get()->TopLeftY = 0;

	Game::GetInstance()->GetContext()->RSSetViewports(1, viewport.get());

	Game::GetInstance()->GetContext()->IASetInputLayout(layout.Get());
	Game::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Game::GetInstance()->GetContext()->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R32_UINT, 0);
	Game::GetInstance()->GetContext()->IASetVertexBuffers(0, 1, vb.GetAddressOf(), strides, offsets);
	Game::GetInstance()->GetContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
	Game::GetInstance()->GetContext()->PSSetShader(pixelShader.Get(), nullptr, 0);

	// MOVEMENT //
	Game::GetInstance()->GetContext()->VSSetConstantBuffers(0, 1, constBuf.GetAddressOf());
	// MOVEMENT //
}

void TriangleComponent::Draw()
{
	Game::GetInstance()->GetContext()->DrawIndexed(6, 0, 0); // Main function for draw (DrawCall)
}

void TriangleComponent::DestroyResources()
{
	// there're nothing to release in TriangleComponent class
}