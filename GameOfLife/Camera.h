#pragma once
#include <SFML/Graphics.hpp>

/*
	A simple 2d camera supporting movement with WASD and zooming with the 
	mousewheel, provided the zoom method is called as follows:
			while (window.pollEvent(event))
				if (event.type == sf::Event::MouseWheelMoved)
					camera.zoom(event.mouseWheel.delta);
*/
class Camera {
public:
	Camera(const sf::Vector2u & window_size);

	void update(float dt, sf::RenderWindow & render_window);

	// Zooms the camera by delta steps. Zooms in when delta is negative
	void zoom(int delta);
private:
	sf::View view;
};