//#include <CL/cl.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cassert>

using namespace sf;

const int Width = 1000;
const int Height = 500;

const int banch_size = 50;

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

	void rand(const T min, const T max) {
		T value_x = std::rand() / (max - min) + min;
		T value_y = std::rand() / (max - min) + min;
		x = value_x;
		y = value_y;
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
		 //pos.x += speed.x;
		 //pos.y += speed.y;
		 pos = pos + speed;
	 }

	 void draw(RenderWindow &window) {
		 circle.setFillColor(Color(250, 250, 250));
		 circle.setPosition(pos.x - rad, pos.y - rad);
		 window.draw(circle);
	 }

	 void compute_wall_collision() {
		 
		 if (pos.x - rad < 0 || pos.x + rad >= Width) {
			 speed.x = -speed.x;
		 }/*
		 if (pos.y - rad < 0 || pos.y + rad >= Height) {
			 speed.y = -speed.y;
		 }*/

		 if (pos.y - rad < 0) {
			 Vector2D<double> last_pos;
			 last_pos = pos - speed;
			 pos.y = 2*rad - speed.y - last_pos.y;
			 speed.y = -speed.y;
		 }
		 if (pos.y + rad >= Height) {
			 Vector2D<double> last_pos;
			 last_pos = pos - speed;
			 pos.y = (Height - 1) - speed.y + (Height - last_pos.y - rad);
			 speed.y = -speed.y;
		 }

	 }
};

class Field
{
private:
	std::vector <Particle*> particles; //two-dimensional array of particles, where std::vector represents the first direction, dinamic array - the second
	int particles_count = 20;


public:
	Field() {
		
		int banch_count = particles_count / banch_size + 1;

		//std::cout << "banch_count: " << banch_count << '\n';
		//reserve memory for <particles_count> particles
		particles.reserve(banch_count);
		for (int i = 0; i < banch_count; i++) {
			particles[i] = new Particle[banch_size];
		}

		//init particles
		int x_cord, y_cord; //in 2-dim array
		for (int i = 0; i < particles_count; i++) {
			x_cord = i / banch_size;
			y_cord = i % banch_size;

			//std::cout << "x_cord: " << x_cord << "\ty_cord: " << y_cord << '\n';

			particles[x_cord][y_cord].pos.rand(100.0, 300.0);
			particles[x_cord][y_cord].speed.x = (i - 5) * 1.0;
			particles[x_cord][y_cord].speed.y = -i * 1.0;
		}

	}

	~Field() {
		int banch_count = particles_count / banch_size + 1;

		for (int i = 0; i < banch_count; i++) {
			delete[] particles[i];
		}//particles(first dim) is std::vector, so it should not be deleted

	}

	void compute() {
		int x_cord, y_cord; //in 2-dim array
		for (int i = 0; i < particles_count; i++) {
			x_cord = i / banch_size;
			y_cord = i % banch_size;

			particles[x_cord][y_cord].compute_wall_collision();

			particles[x_cord][y_cord].computeStep();
		}
	}

	void draw(RenderWindow &window) {
		int x_cord, y_cord; //in 2-dim array
		for (int i = 0; i < particles_count; i++) {
			x_cord = i / banch_size;
			y_cord = i % banch_size;
			assert(y_cord < banch_size, "y_cord is out of banch_size");

			particles[x_cord][y_cord].draw(window);
		}
	}
};

int main()
{
	RenderWindow window(VideoMode(Width, Height), "Billiards simulation");
	window.setFramerateLimit(5);

	Field field;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear(Color(0, 0, 0, 0));

		field.compute();
		field.draw(window);
		


		window.display();
	}

	return 0;
}


