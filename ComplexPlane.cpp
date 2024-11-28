#include "ComplexPlane.h"


ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
	m_plane_center = { 0,0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}



void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
	target.draw(m_vArray);
}



void ComplexPlane::updateRender() {
	if (m_state == State::CALCULATING) {
		for (int i = 0; i < m_pixel_size.y; i++) {
			for (int j = 0; j < m_pixel_size.x; j++) {
				size_t index = j + i * m_pixel_size.x;
				Vector2f coord = mapPixelToCoords({ j,i });
				size_t iterations = countIterations(coord);
				Uint8 r, g, b;
				iterationsToRGB(iterations, r, g, b);
				m_vArray[index].position = { (float)j, (float)i };
				m_vArray[index].color = { r, g, b };
			}
		}
		m_state = State::DISPLAYING;
	}
}




void ComplexPlane::zoomIn() {
	m_zoomCount++;
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x,y };
	m_state = State::CALCULATING;
}




void ComplexPlane::zoomOut() {
	m_zoomCount--;
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x,y };
	m_state = State::CALCULATING;
}




void ComplexPlane::setCenter(Vector2i mousePixel) {
	m_plane_center = mapPixelToCoords(mousePixel);
	m_state = State::CALCULATING;
}




void ComplexPlane::setMouseLocation(Vector2i mousePixel) {
	m_mouseLocation = mapPixelToCoords(mousePixel);
}




void ComplexPlane::loadText(Text& text) {
	ostringstream o;
	o << "Mandelbrot Set" << endl;
	o << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl;
	o << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl;
	o << "Left-click to Zoom in" << endl;
	o << "Right-click to Zoom out" << endl;
	text.setString(o.str());
}



size_t ComplexPlane::countIterations(Vector2f coord) {
	complex<float> z(0, 0);
	complex<float> c(coord.x, coord.y);
	size_t iter = 0;

	while (abs(z) <= 2.0f && iter < MAX_ITER) {
		z = z * z + c;
		iter++;
	}
	return iter;
}




void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {

	if (count == MAX_ITER) {
		r = 0;
		g = 0;
		b = 0;
	}
	else {
		r = static_cast<Uint8>(rand() % 256);
		g = static_cast<Uint8>(rand() % 256);
		b = static_cast<Uint8>(rand() % 256);
	}
}



Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {
	float x = (static_cast<float>(mousePixel.x) / m_pixel_size.x - BASE_ZOOM)
		* m_plane_size.x + m_plane_center.x;
	float y = (static_cast<float>(mousePixel.y) / m_pixel_size.y - BASE_ZOOM)
		* m_plane_size.y + m_plane_center.y;
	return { x,y };
}