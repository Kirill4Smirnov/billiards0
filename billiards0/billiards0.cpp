//#include <CL/cl.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <cassert>
#include <ctime>

using namespace sf;

const int Width = 500;
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
	}

	Vector2D operator + (const Vector2D &vec) const {
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D operator += (const Vector2D& vec) {
		x += vec.x;
		y += vec.y;
		return Vector2D(*this);
	}

	Vector2D operator - (const Vector2D& vec) const {
		return Vector2D(x - vec.x, y - vec.y);
	}

	Vector2D operator -= (const Vector2D& vec) {
		x = x - vec.x;
		y = y - vec.y;
		return Vector2D(*this);
	}

	Vector2D operator -() {
		x = -x;
		y = -y;
		return Vector2D(x, y);
	}

	Vector2D operator * (const T &scale) {
		x *= scale;
		y *= scale;
		return Vector2D(*this);
	}

	Vector2D operator / (const T& scale) {
		x /= scale;
		y /= scale;
		return Vector2D(*this);
	}

	T dot(const Vector2D& second) {
		return x * second.x + y * second.y;
		
	}

	T dot(const Vector2D &first, const Vector2D &second) {
		T res;
		res = first.x * second.x + first.y * second.y;
		return res;
	}

	T len() {
		return std::sqrt(x*x + y*y);
	}

	Vector2D norm() {
		Vector2D res;
		T length = this->len();
		res.x = this->x / length;
		res.y = this->y / length;
		return res;
	}

	void rand(const T min, const T max) {
		//std::srand(std::time(NULL));
		T value_x = std::rand() / (max - min) + min;
		T value_y = std::rand() / (max - min) + min;
		//T value_x = Randomizer::Random(min, max);
		//T value_y = Randomizer::Random(min, max);

		x = value_x;
		y = value_y;
	}

	T sqr() {
		return pow(x, 2) + pow(y, 2);
	}
};

double dotd(const Vector2D <double>& first, const Vector2D <double>& second) {
	return first.x * second.x + first.y * second.y;

}


class Particle { //uses SFML to draw
private:
	CircleShape circle;
	double mass = 1.0;

public:
	int rad = 30;
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
		 }
		 if (pos.y - rad < 0 || pos.y + rad >= Height) {
			 speed.y = -speed.y;
		 }

		 /*
		 if (pos.x + rad >= Width) {
			 Vector2D<double> last_pos;
			 last_pos = pos - speed;

			 pos.x += 2 * (speed.x - Width-1 + rad + last_pos.x);
			 speed.x = -speed.x;
		 }

		 if (pos.x - rad < 0) {
			 Vector2D<double> last_pos;
			 last_pos = pos - speed;
			 pos.x += 2 * (-speed.x - last_pos.x + rad);
			 speed.x = -speed.y;
		 }

		 if (pos.y - rad < 0) {
			 Vector2D<double> last_pos;
			 last_pos = pos - speed;
			 pos.y = 2*rad - speed.y - last_pos.y;
			 speed.y = -speed.y;
		 }
		 if (pos.y + rad >= Height) {
			 Vector2D<double> last_pos;
			 last_pos = pos - speed;
			 pos.y -= 2*(Height - 1 - rad - last_pos.y);
			 speed.y = -speed.y;
		 } */

	 }

	 void compute_particle_collision(Particle &particle) {
		 Vector2D<double> temp_speed(speed);
		 Vector2D<double> temp_pos(pos);
		 //speed = speed - ((pos - particle.pos).norm() / (pos - particle.pos).len() * 
		//	 dotd(temp_speed - particle.speed, pos - particle.pos)) * (2 * particle.mass / (mass + particle.mass));
		 
		 //particle.speed = particle.speed - ((particle.pos - pos).norm() / (particle.pos - pos).len() * 
		//	 dotd(particle.speed - temp_speed, particle.pos - pos)) * (2 * mass / (mass + particle.mass));

		 speed -= ((pos - particle.pos) * dotd(temp_speed - particle.speed, pos - particle.pos)
			 / (pos - particle.pos).sqr()) * (2 * particle.mass) / (mass + particle.mass);
		 particle.speed -= ((particle.pos - pos) * dotd(particle.speed - temp_speed, particle.pos - pos)
			 / (particle.pos - pos).sqr()) * (2 * mass) / (mass + particle.mass);


		 ////push the particles apart
		 Vector2D<double> pos2to1;
		pos2to1 = pos - particle.pos;
		 Vector2D<double> pos_change;
		 pos_change = pos2to1.norm() * (rad + particle.rad - pos2to1.len()) / 2;

		 pos += pos_change;
		 particle.pos -= pos_change;
	 }
};

class Field
{
private:
	std::vector <Particle*> particles; //two-dimensional array of particles, where std::vector represents the first direction, dinamic array - the second
	int particles_count = 5;

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

			particles[x_cord][y_cord].pos.rand(100, 400);
			particles[x_cord][y_cord].speed.x = (i - 5) / 1.0;
			particles[x_cord][y_cord].speed.y = -i / 1.0;
		}

		//particles[0][0].pos = Vector2D<double>(100, 200);
		//particles[0][1].pos = Vector2D<double>(300, 200);
	}

	~Field() {
		int banch_count = particles_count / banch_size + 1;

		for (int i = 0; i < banch_count; i++) {
			delete[] particles[i];
		}//particles(first dim) is std::vector, so it should not be deleted

	}

	void compute() {
		int x_cord, y_cord, x_cord_second, y_cord_second; //in 2-dim array
		for (int i = 0; i < particles_count; i++) {
			x_cord = i / banch_size;
			y_cord = i % banch_size;

			for (int j = i + 1; j < particles_count; j++) {
				x_cord_second = j / banch_size;
				y_cord_second = j % banch_size;

				if ((particles[x_cord][y_cord].pos - particles[x_cord_second][y_cord_second].pos).len() < particles[x_cord][y_cord].rad + particles[x_cord_second][y_cord_second].rad) {
					
					std::cout << "particle " << i << " colided with particle " << j << '\t' << (particles[x_cord][y_cord].pos - particles[x_cord_second][y_cord_second].pos).len() << '\n';
					particles[x_cord][y_cord].compute_particle_collision(particles[x_cord_second][y_cord_second]);

				}
				
			}
		}

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
	window.setFramerateLimit(60);

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


