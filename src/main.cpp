#include <SFML/Graphics.hpp>
#include<vector>

using namespace sf;


class Arrow
{
public:
	Sprite shape;

	Arrow(Texture* texture, Vector2f pos)
	{
		this->shape.setTexture(*texture);

		this->shape.setScale(0.16f, 0.16f);

		this->shape.setPosition(pos);
	}

	~Arrow() {}
};

class Bow
{
public:
	Sprite shape;
	Texture* texture;



	std::vector<Arrow> arrows;

	Bow(Texture* texture)
	{


		this->texture = texture;
		this->shape.setTexture(*texture);

		this->shape.setScale(0.3f, 0.3f);
		this->shape.setOrigin(300, 300);
	}

	~Bow() {}

};

class Target
{
public:
	Sprite shape;


	Target(Texture* texture)
	{
		this->shape.setTexture(*texture);
		this->shape.setScale(0.6f, 0.6f);
		this->shape.setPosition(Vector2f(0, 10.f));

	}

	~Target() {}
};

int main()
{


	RenderWindow window(VideoMode(800, 600), "Archary!", Style::Default);
	window.setFramerateLimit(60);


	//Init textures
	Texture bowTex;
	bowTex.loadFromFile("textures/white_bow.png");

	Texture targetTex;
	targetTex.loadFromFile("textures/white_target.png");

	Texture arrowTex;
	arrowTex.loadFromFile("textures/white_arrow.png");

	

	//Bow init
	int shootTimer = 20;
	Bow bow(&bowTex);


	//Target init
	Target target(&targetTex);


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		
			//Bow
			if (Keyboard::isKeyPressed(Keyboard::Up))
				bow.shape.move(0.f, -10.f);
			if (Keyboard::isKeyPressed(Keyboard::Left))
				bow.shape.move(-10.f, 0.f);
			if (Keyboard::isKeyPressed(Keyboard::Down))
				bow.shape.move(0.f, 10.f);
			if (Keyboard::isKeyPressed(Keyboard::Right))
				bow.shape.move(10.f, 0.f);

			//Collision with window
			if (bow.shape.getPosition().x <= 0) //Left
				bow.shape.setPosition(0.f, bow.shape.getPosition().y);
			if (bow.shape.getPosition().x >= window.getSize().x - bow.shape.getGlobalBounds().width) //Right
				bow.shape.setPosition(window.getSize().x - bow.shape.getGlobalBounds().width, bow.shape.getPosition().y);
			if (bow.shape.getPosition().y <= 0) //Top
				bow.shape.setPosition(bow.shape.getPosition().x, 0.f);
			if (bow.shape.getPosition().y >= window.getSize().y - bow.shape.getGlobalBounds().height) //Bottom
				bow.shape.setPosition(bow.shape.getPosition().x, window.getSize().y - bow.shape.getGlobalBounds().height);

			//Update Controls
			if (shootTimer < 15)
				shootTimer++;

			if (Keyboard::isKeyPressed(Keyboard::Enter) && shootTimer >= 15) //Shooting
			{
				bow.arrows.push_back(Arrow(&arrowTex, bow.shape.getPosition()));
				shootTimer = 0; //reset timer
			}

			//Arrows
			for (size_t i = 0; i < bow.arrows.size(); i++)
			{
				//Move
				bow.arrows[i].shape.move(40.f, 0.f);

				//Out of window bounds
				if (bow.arrows[i].shape.getPosition().x > window.getSize().x)
				{
					bow.arrows.erase(bow.arrows.begin() + i);
					break;
				}

				//Target collision
				for (size_t k = 0; k < bow.arrows.size(); k++)
				{
					if (bow.arrows[i].shape.getPosition().x > 200)
					{

						bow.arrows.erase(bow.arrows.begin() + i);
						break;
					}
				}
			}


			//Target

			target.shape.setPosition(500.f, 200.f);

		//Draw ===================================================================== DRAW
		window.clear(sf::Color::White);

		//Bow
		window.draw(bow.shape);

		//Arrow
		for (size_t i = 0; i < bow.arrows.size(); i++)
		{
			window.draw(bow.arrows[i].shape);
		}

		//Target

			window.draw(target.shape);

		window.display();
	}

	return 0;
}