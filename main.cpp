#include "ComplexPlane.h"

int main() {

	srand(time(0));

	int screenWidth = VideoMode::getDesktopMode().width / 4;
	int screenHeight = VideoMode::getDesktopMode().height / 4;

	RenderWindow window(VideoMode(screenWidth,screenHeight),"Mandelbrot Set");
	ComplexPlane cPlane(screenWidth, screenHeight);

	Font font;
	font.loadFromFile("arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setFillColor(Color::Green);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed) {
				Vector2i mousePos = Mouse::getPosition(window);
				if (event.mouseButton.button == Mouse::Left) {
					cPlane.zoomIn();
					cPlane.setCenter(mousePos);
				}
				else if (event.mouseButton.button == Mouse::Right) {
					cPlane.zoomOut();
					cPlane.setCenter(mousePos);
				}
			}
			
			if (event.type == Event::MouseMoved) {
				Vector2i mousePos = Mouse::getPosition(window);
				cPlane.setMouseLocation(mousePos);
			}
		}

		cPlane.updateRender();
		cPlane.loadText(text);

		window.clear();
		window.draw(cPlane);
		window.draw(text);
		window.display();
	}
	return 0;
}