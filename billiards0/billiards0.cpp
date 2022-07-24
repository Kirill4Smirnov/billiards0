//#include <CL/cl.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>

using namespace sf;

const int Width = 500;
const int Height = 500;

const int banch_size = 100;

template <class T>
class Vector2D
{
public:
	T x;
	T y;
	Vector2D() {
		x = 0;
		y = 0;
	}
	
	Vector2D(T x, T y) {
		this->x = x;
		this->y = y;
	}
	//copy
	Vector2D(const Vector2D &vec) {
		this->x = vec.x;
		this->y = vec.y;
		return *this;
	}

	Vector2D operator + (const Vector2D &vec) const {
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D operator - (const Vector2D& vec) const {
		return Vector2D(x - vec.x, y - vec.y);
	}

	Vector2D operator -() {
		x = -x;
		y = -y;
		return Vector2D(x, y);
	}

	Vector2D operator * (const T &scale) {
		x *= scale;
		y *= scale;
	}

	Vector2D operator / (const T& scale) {
		x /= scale;
		y /= scale;
	}

	T dot(const Vector2D &first, const Vector2D &second) {
		T res;
		res.x = first.x * second.x + first.y * second.y;
		return res;
	}

	T len(const Vector2D& vec) {
		return std::sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	Vector2D norm(const Vector2D& vec) {
		Vector2D res;
		T length = len(vec);
		res.x = vec.x / length;
		res.y = vec.y / length;
		return res;
	}
};

class Particle { //uses SFML to draw
private:
	CircleShape circle;
	double mass = 1.0;
	
public:
	int rad = 15;
	 Vector2D <double> pos;
	 Vector2D <double> speed; //change of pos in next step
	 

	 Particle() {
		 pos.x = 0;
		 pos.y = 0;
		 speed.x = 0;
		 speed.y = 0;
		 circle.setRadius(rad);
	 }

	 Particle(const Vector2D <double> &newpos) {
		 pos.x = newpos.x;
		 pos.y = newpos.y;
		 circle.setRadius(rad);
	 }

	 Particle(const Vector2D <double>& newpos, const Vector2D <double>& newspeed) {
		 pos.x = newpos.x;
		 pos.y = newpos.y;
		 speed.x = newspeed.x;
		 speed.y = newspeed.y;
		 circle.setRadius(rad);
	 }

	 void computeStep() {
		 pos.x += speed.x;
		 pos.y += speed.y;
	 }

	 void draw(RenderWindow &window) {
		 circle.setFillColor(Color(250, 250, 250));
		 circle.setPosition(pos.x - rad, pos.y - rad);
		 window.draw(circle);
	 }

};

class Field
{
private:
	std::vector <Particle*> particles; //two-dimensional array of particles, where std::vector represents the first direction, dinamic array - the second
	int particles_count = 10;


public:
	Field() {
		particles.reserve(10);
		for

	}


};

int main()
{
	RenderWindow window(VideoMode(Width, Height), "Billiards simulation");
	window.setFramerateLimit(60);

	Particle particle(Vector2D <double> (100, 100), Vector2D <double>(5, 2.5));

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear(Color(0, 0, 0, 0));


		particle.computeStep();
		particle.draw(window);

		if (particle.pos.x - particle.rad < 0 || particle.pos.x + particle.rad >= Width) {
			particle.speed.x = -particle.speed.x;
		}
		if (particle.pos.y - particle.rad < 0 || particle.pos.y + particle.rad >= Height) {
			particle.speed.y = -particle.speed.y;
		}


		window.display();
	}

	return 0;
}


