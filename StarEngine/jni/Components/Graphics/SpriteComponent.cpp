#include "SpriteComponent.h"
#include "../../Logger.h"
#include "../../Assets/TextureManager.h"
#include "../TransformComponent.h"
#include "../../Graphics/SpriteBatch.h"

namespace star
{
	SpriteComponent::SpriteComponent(
		const tstring& filepath,
		const tstring& spriteName,
		uint32 widthSegments,
		uint32 heightSegments
		)
		: BaseComponent()
		, m_WidthSegments(widthSegments)
		, m_HeightSegments(heightSegments)
		, m_CurrentWidthSegment(0)
		, m_CurrentHeightSegment(0)
		, m_Width(0)
		, m_Height(0)
		, m_FilePath(filepath)
		, m_SpriteName(spriteName)
		, m_bIsHudElement(false)
		, m_SpriteInfo()
	{
	}

	void SpriteComponent::InitializeComponent()
	{	
		TextureManager::GetInstance()->LoadTexture(
			m_FilePath.GetAssetsPath(),
			m_SpriteName
			);

		m_Width = TextureManager::GetInstance()->
			GetTextureDimensions(m_SpriteName).x /
			m_WidthSegments;
		m_Height =  TextureManager::GetInstance()->
			GetTextureDimensions(m_SpriteName).y /
			m_HeightSegments;

		CreateVertices();
		CreateUVCoords();

		FillSpriteInfo();
	}

	void SpriteComponent::FillSpriteInfo()
	{
		m_SpriteInfo.spriteName = m_SpriteName;
		m_SpriteInfo.dimensions = vec2(m_Width, m_Height);
	}

	SpriteComponent::~SpriteComponent()
	{

	}

	void SpriteComponent::CreateVertices()
	{

		/*
		*  TL    TR
		*   0----1 
		*   |   /| 
		*   |  / |
		*   | /  |
		*   |/   |
		*   2----3
		*  BL    BR
		*/

		//0
		m_Vertices[0] = 0;
		m_Vertices[1] = float32(m_Height);
		m_Vertices[2] = 0;

		//1
		m_Vertices[3] = float32(m_Width);
		m_Vertices[4] = float32(m_Height);
		m_Vertices[5] = 0;

		//2
		m_Vertices[6] = 0;
		m_Vertices[7] = 0;
		m_Vertices[8] = 0;

		//3
		m_Vertices[9] = float32(m_Width);
		m_Vertices[10] = 0;
		m_Vertices[11] = 0;
	}

	void SpriteComponent::CreateUVCoords()
	{
		float32 startX =
			float32(m_CurrentWidthSegment) /
			float32(m_WidthSegments);
		float32 endX = 1.0f / m_WidthSegments;
		float32 startY =
			float32(m_CurrentHeightSegment) /
			float32(m_HeightSegments);
		float32 endY = 1.0f / m_HeightSegments;

		/*
		*  TL    TR
		*   0----1 
		*   |   /| 
		*   |  / |
		*   | /  |
		*   |/   |
		*   2----3
		*  BL    BR
		*/

		//0
		m_UvCoords[0] = startX;
		m_UvCoords[1] = startY + endY;

		//1
		m_UvCoords[2] = startX + endX;
		m_UvCoords[3] = startY + endY;

		//2
		m_UvCoords[4] = startX;
		m_UvCoords[5] = startY;

		//3
		m_UvCoords[6] = startX + endX;
		m_UvCoords[7] = startY;

		//left
		m_SpriteInfo.uvCoords[0]   = m_UvCoords[0];
		//bottom
		m_SpriteInfo.uvCoords[1]   = m_UvCoords[5];
		//right
		m_SpriteInfo.uvCoords[2]   = m_UvCoords[2];
		//top
		m_SpriteInfo.uvCoords[3]   = m_UvCoords[1];
	}

	void SpriteComponent::Draw()
	{
		m_SpriteInfo.transform = GetTransform()->GetWorldMatrix();
		SpriteBatch::GetInstance()->Draw(m_SpriteInfo);
	}

	void SpriteComponent::Update(const Context & context)
	{

	}
	
	bool SpriteComponent::CheckCulling(
		float left,
		float right,
		float top,
		float bottom
		) const
	{
		float32 spriteWidth, spriteHeight;

		pos objectPos = GetTransform()->GetWorldPosition();

		spriteWidth = float32(GetWidth()) * GetTransform()->GetWorldScale().x;
		spriteHeight = float32(GetHeight()) * GetTransform()->GetWorldScale().y;

		return
			(objectPos.x >= left || objectPos.x + spriteWidth <= right) &&
			(objectPos.y >= bottom || objectPos.y + spriteHeight <= top);
	}

	const tstring& SpriteComponent::GetFilePath() const
	{
		return m_FilePath.GetPath();
	}

	const tstring& SpriteComponent::GetName() const
	{
		return m_SpriteName;
	}

	int32 SpriteComponent::GetWidth() const
	{
		return m_Width;
	}

	int32 SpriteComponent::GetHeight() const
	{
		return m_Height; 
	}

	void SpriteComponent::SetCurrentSegment(uint32 widthSegment, uint32 heightSegment)
	{
		m_CurrentWidthSegment = widthSegment;
		m_CurrentHeightSegment = m_HeightSegments - heightSegment - 1;
		CreateUVCoords();
	}

	void SpriteComponent::SetCurrentHorizontalSegment(uint32 segment)
	{
		m_CurrentWidthSegment = segment;
		CreateUVCoords();
	}

	void SpriteComponent::SetCurrentVerticalSegment(uint32 segment)
	{
		m_CurrentHeightSegment = m_HeightSegments - segment - 1;
		CreateUVCoords();
	}

	void SpriteComponent::SetTexture(
		const tstring& filepath,
		const tstring& spriteName,
		uint32 widthSegments,
		uint32 heightSegments
		)
	{
		m_Width = 0;
		m_WidthSegments = widthSegments;
		m_CurrentWidthSegment = 0;
		m_Height = 0;
		m_HeightSegments = heightSegments;
		m_CurrentHeightSegment = 0;
		m_FilePath = filepath;
		m_SpriteName = spriteName;

		TextureManager::GetInstance()->LoadTexture(m_FilePath.GetAssetsPath(),m_SpriteName);
		m_Width = TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).x / m_WidthSegments;
		m_Height =  TextureManager::GetInstance()->GetTextureDimensions(m_SpriteName).y / m_HeightSegments;

		CreateVertices();
		CreateUVCoords();

		FillSpriteInfo();
	}

}
