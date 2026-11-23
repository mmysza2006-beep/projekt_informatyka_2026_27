
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "Pilka.h"
#include "Paletka.h"
#include "Stone.h"



int main() {


	const float WIDTH = 640.f;
	const float HEIGHT = 480.f;
	sf::RenderWindow window(sf::VideoMode({ (unsigned)WIDTH, (unsigned)HEIGHT }), "Arcanoid");
	window.setFramerateLimit(60);

	Paletka pal(320.f, 440.f, 100.f, 20.f, 8.f);
	Pilka pilka(320.f, 245.f, 4.f, 3.f, 8.f);

	int klatka = 0;

	std::vector<Stone> bloki;
	const float SZEROKOSC = WIDTH;
	const int ILOSC_KOLUMN = 6;
	const int ILOSC_WIERSZY = 7;
	const float ROZMIAR_BLOKU_Y = 25.f;
	const float ROZMIAR_BLOKU_X = (SZEROKOSC - (ILOSC_KOLUMN - 1) * 2.f) / ILOSC_KOLUMN;

	bloki.reserve(ILOSC_KOLUMN * ILOSC_WIERSZY);

	for (int y = 0; y < ILOSC_WIERSZY; y++)
	{
		for (int x = 0; x < ILOSC_KOLUMN; x++)
		{
			float posX = x * (ROZMIAR_BLOKU_X + 2.f);
			float posY = y * (ROZMIAR_BLOKU_Y + 2.f) + 50.f;

			int L = (y < 1) ? 3 : (y < 3) ? 2 : 1;

			sf::Vector2f pos(posX, posY);
			sf::Vector2f size(ROZMIAR_BLOKU_X, ROZMIAR_BLOKU_Y);
			bloki.emplace_back(pos, size, L);
		}
	}


	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			pal.moveLeft();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			pal.moveRight();
		}
		pal.clampToBounds(WIDTH);


		pilka.move();
		pilka.collideWalls(WIDTH, HEIGHT);
		if (pilka.collidePaddle(pal)) {
			std::cout << "HIT PADDLE\n";
		}

		for (auto& stone : bloki) {
			if (stone.isDestroyed() == false) {
				float pilkaX = pilka.getX();
				float pilkaY = pilka.getY();
				float r = pilka.getRadius();

				float blokLewo = stone.getPosition().x;
				float blokPrawo = stone.getPosition().x + stone.getSize().x;
				float blokGora = stone.getPosition().y;
				float blokDol = stone.getPosition().y + stone.getSize().y;
				if (pilkaX + r > blokLewo && pilkaX - r < blokPrawo &&
					pilkaY + r > blokGora && pilkaY - r < blokDol)
				{
					stone.trafienie();
					pilka.bounceY();
					break;
				}
			}
		}

		bloki.erase(
			std::remove_if(bloki.begin(), bloki.end(),
				[](const Stone& s) { return s.isDestroyed(); }),
			bloki.end()
		);
		

		if (pilka.getY() - pilka.getRadius() > HEIGHT) {
			std::cout << "MISS! KONIEC GRY\n";
			window.close();
		}

		klatka++;
		if (klatka % 10 == 0) {
			std::cout << "x=" << pilka.getX() << " y=" << pilka.getY()
				<< " vx=" << pilka.getVx() << " vy=" << pilka.getVy() << std::endl;
		}

		window.clear(sf::Color(20, 20, 30));
		for (auto& stone : bloki)
		{
			stone.draw(window);
		}
		pal.draw(window);
		pilka.draw(window);
		window.display();

	}
}

