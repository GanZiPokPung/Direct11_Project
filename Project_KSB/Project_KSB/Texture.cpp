#include "pch.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filePath)
{
	int width = 0;
	int height = 0;

	if (!LoadTarga(filePath, height, width))
		return false;

	// 텍스처의 구조체를 설정
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// 빈 텍스처를 생성
	if (FAILED(device->CreateTexture2D(&textureDesc, NULL, &m_Texture)))
		return false;

	// targa 이미지 데이터의 너비 
	UINT rowPitch = (width * 4) * sizeof(BYTE);

	deviceContext->UpdateSubresource(m_Texture, 0, NULL, m_TargaData, rowPitch, 0);

	// 셰이더 리소스 뷰 구조체
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	if (FAILED(device->CreateShaderResourceView(m_Texture, &srvDesc, &m_TextureView)))
		return false;

	deviceContext->GenerateMips(m_TextureView);

	SAFE_DELETE_ARRAY(m_TargaData);

	return true;
}

void Texture::Shutdown()
{
	SAFE_RELEASE(m_TextureView);
	SAFE_RELEASE(m_Texture);
	SAFE_DELETE_ARRAY(m_TargaData);
}

bool Texture::LoadTarga(char* filePath, int& height, int& width)
{
	FILE* file;
	if (0 != fopen_s(&file, filePath, "rb"))
		return false;

	// 파일 헤더를 읽는다
	TargaHeader targaFileHeader;
	UINT count = (UINT)fread(&targaFileHeader, sizeof(TargaHeader), 1, file);
	if (1 != count)
		return false;

	// 파일 헤더에서 중요 정보를 얻어옴
	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	int bpp = (int)targaFileHeader.bpp;

	if (32 != bpp)
		return false;

	int imageSize = width * height * 4;

	BYTE* targaImage = new BYTE[imageSize];

	count = (UINT)fread(targaImage, sizeof(BYTE), imageSize, file);
	if (imageSize != count)
		return false;

	fclose(file);

	m_TargaData = new BYTE[imageSize];

	int index = 0;

	// targa 형식이 거꾸로 저장되어 있기 때문에 올바른 순서로 복사한다.
	// 맨 마지막 행에서 첫번째 열 위치
	int k = (width * height * 4) - (width * 4);
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			m_TargaData[index + 0] = targaImage[k + 2]; //R
			m_TargaData[index + 1] = targaImage[k + 1]; //G
			m_TargaData[index + 2] = targaImage[k + 0]; //B
			m_TargaData[index + 3] = targaImage[k + 3]; //A

			k += 4;
			index += 4;
		}

		// 현재 행의 이전 행 첫번째 열 위치
		k -= (width * 8);
	}

	SAFE_DELETE_ARRAY(targaImage);

	return true;
}
