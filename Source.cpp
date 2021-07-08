#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 8;


	RenderWindow window(VideoMode(1366, 768), "RaTATATATATA!", Style::Default, settings);
	//RenderWindow window(VideoMode(500, 500), "RaTATATATATA!", Style::Default, settings);

	Texture t;
	t.loadFromFile("RatSolo.png");

	Sprite s;
	s.setTexture(t);
	s.setPosition(0, 10);


	Texture cricleT;
	cricleT.loadFromFile("circle.png");
	Sprite cricleS(cricleT);
	cricleS.setPosition(250, 250);

	//
	//Тест зона------

	CircleShape circle(50.f);
	circle.setFillColor(Color::Red);

	circle.setOutlineThickness(8.f); //Обводка
	circle.setOutlineColor(Color(80, 220, 50, 100)); //Цвет и прозрачность обводки

	circle.move(50, 50); //Подвинуть обьект


	//-----------

	ConvexShape convex;
	convex.setPointCount(5);

	convex.setPoint(0, Vector2f(0.f, 0.f));
	convex.setPoint(1, Vector2f(150.f, 10.f));
	convex.setPoint(2, Vector2f(120.f, 90.f));
	convex.setPoint(3, Vector2f(30.f, 100.f));
	convex.setPoint(4, Vector2f(5.f, 50.f));

	convex.setOutlineColor(Color::White);
	convex.setOutlineThickness(3.f);

	convex.setFillColor(Color::Black);
	convex.move(70, 200);
	//---

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			s.move(-0.1, 0);

		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			s.move(0.1, 0);
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			s.move(0, -0.1);
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			s.move(0, 0.1);
		}




		//window.clear(Color::Yellow);
		window.clear(Color(250, 220, 100, 0));
		window.draw(s);
		window.draw(circle);

		window.draw(convex);
		window.draw(cricleS);
		window.display();
	}

	return 0;
}

