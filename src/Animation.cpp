#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0)
{
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
	: m_Name(name),
	m_Sprite(t),
	m_FrameCount(frameCount),
	m_CurrentFrame(0),
	m_Speed(speed)
{
	m_Size = Vec2(t.getSize().x / frameCount, (float)t.getSize().y);
	m_Sprite.setOrigin(m_Size.x / 2.0f, m_Size.y / 2.0f);
	m_Sprite.setTextureRect(sf::IntRect(std::floor(m_CurrentFrame) * m_Size.x, 0, m_Size.x, m_Size.y));
}

// Update the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update()
{
	m_CurrentFrame += 1;

	// TODO: 1) calculate the correct frame of animation to play based on currentFrame and speed
	//		 2) set the texture rectangle properly (see constructor for sample)
}

const Vec2& Animation::getSize() const
{
	return m_Size;
}

const std::string& Animation::getName() const
{
	return m_Name;
}

sf::Sprite& Animation::getSprite()
{
	return m_Sprite;
}

bool Animation::hasEnded() const
{
	// TODO: detect when animation has ened (last frame was played) and return true
	return false;
}