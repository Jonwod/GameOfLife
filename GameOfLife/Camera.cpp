#include "Camera.h"


int normal(int i) {
	return i == 0 ? 0 : i / abs(i);
}


Camera::Camera(const sf::Vector2u & window_size) {
	view.reset(sf::FloatRect(0.f, 0.f, float(window_size.x), float(window_size.y)));
	view.setCenter(float(window_size.x / 2), float(window_size.y / 2));
}


void Camera::update(float dt, sf::RenderWindow & render_window) {
	const float speed = 100.f * view.getViewport().width;
	sf::Vector2f offset{ 0.f, 0.f };
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		offset.y -= speed * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		offset.y += speed * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		offset.x += speed * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		offset.x -= speed * dt;
	}
	view.move(offset);
	render_window.setView(view);
}


// Zooms the camera by delta steps. Zooms in when delta is negative
void Camera::zoom(int delta) {
	constexpr float step_size = 0.1f;
	while (delta) {
		const auto norm = normal(delta);
		view.zoom(1.f - norm * step_size);
		delta -= norm;
	}
}