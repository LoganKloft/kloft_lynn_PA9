#include "Button.h"

Button::Button(const sf::Vector2f& hitbox_size, const sf::Vector2f& hitbox_pos, const sf::Vector2f& size, const sf::Vector2f& pos, const std::string& texture) : sf::IntRect((int)hitbox_pos.x, (int)hitbox_pos.y, (int)hitbox_size.x, (int)hitbox_size.y), sf::RectangleShape(size)
{
	if (!m_texture.loadFromFile(texture))
	{
		std::cout << "Failed to load " << texture << " in class Button" << std::endl;
	}
	this->RectangleShape::setPosition(pos);
	this->RectangleShape::setTexture(&m_texture);
}