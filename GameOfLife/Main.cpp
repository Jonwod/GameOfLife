#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Camera.h"


const sf::Color live{255, 255, 255};
const sf::Color dead{0,0,0};


uint8_t numNeighbors(const sf::Image & image, uint16_t x, uint16_t y) {
	uint8_t num = 0;
	for (uint16_t adjx = x - 1; adjx < x + 2; ++adjx) {
		for (uint16_t adjy = y - 1; adjy < y + 2; ++adjy) {
			const bool valid_cell = adjx >= 0  &&  adjx < image.getSize().x  &&  adjy >= 0  &&  adjy < image.getSize().y;
			if (!(adjx == x  &&  adjy == y) && valid_cell  &&  image.getPixel(adjx, adjy) == live) {
				++num;
			}
		}
	}
	return num;
}


void updateGameOfLife(sf::Image & image) {
	struct PendingChange{
		sf::Vector2<uint16_t> cell;
		bool alive;
	};

	std::vector<PendingChange> pending;

	for (uint16_t x = 1; x < image.getSize().x - 1; ++x) {
		for (uint16_t y = 1; y < image.getSize().y - 1; ++y) {
			const uint8_t num_neigh = numNeighbors(image, x, y);
			if (image.getPixel(x, y) == live) {
				if (num_neigh < 2  || num_neigh > 3)
					pending.push_back({ {x,y}, false });
			}			
			else {
				if (num_neigh == 3)
					pending.push_back({ {x,y}, true });
			}
		}
	}

	for (const auto & change : pending) {
		image.setPixel(change.cell.x, change.cell.y, change.alive ? live : dead);
	}
}


// Returns a random float between 0 and 1
float frandUnit() {
	return float(rand()) / float(RAND_MAX);
}


/*
Randomly sets cells in the image to live or dead.
population_density should be between 0 (totally unpopulated) and 1 (completely populated)
*/
void randInit(sf::Image & image, float population_density = 0.1f) {
	srand(unsigned int(time(NULL)));
	for (uint16_t x = 1; x < image.getSize().x - 1; ++x) {
		for (uint16_t y = 1; y < image.getSize().y - 1; ++y) {
			image.setPixel(x, y, frandUnit() <= population_density ? sf::Color::White : sf::Color::Black);
		}
	}
}


// Initializes the image with a row of 10 live cells in the center. For testing purposes.
void lineInit(sf::Image & image) {
	for (uint16_t x = 500; x < 510; ++x) {
		image.setPixel(x, 400, sf::Color::White );		
	}
}


int main() {
	sf::Vector2u window_size{1000, 800};
	sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "SFML works!");

	// Create a 20x20 image filled with black color
	sf::Image image;
	image.create(window_size.x, window_size.y, sf::Color::Black);
	randInit(image, 0.05f);

	sf::Texture texture;
	texture.loadFromImage(image);

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);

	Camera camera(window_size);

	sf::Clock frame_clock;
	float dt = 1.f / 60.f;
	while (window.isOpen())	{
		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseWheelMoved) {
				camera.zoom(event.mouseWheel.delta);
			}
		}

		camera.update(dt, window);

		window.clear();
		window.draw(sprite);
		window.display();

		updateGameOfLife(image);
		texture.loadFromImage(image);

		dt = frame_clock.restart().asSeconds();
	}

	return 0;
}