#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <ctime> //Generate random numbers
#include <cstdlib>
using namespace sf;
using namespace std;
int main()
{
	float width = sf::VideoMode::getDesktopMode().width;
	float height = sf::VideoMode::getDesktopMode().height;
	View mainView(FloatRect(0.0f, 0.0f, width, height));
	//width = 800; height = 600;
	// Create a video mode object
	VideoMode vm(width, height);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game!!", Style::Default);
	vector<Vector2f> vertices; // declare a vector to store the vertices
	vector<Vector2f> points; 
	int numVertices = 0; // to keep track of the number of vertices captured so far.

	Font f;
	if (!f.loadFromFile("./fonts/KOMIKAP_.ttf")) cout << "failed to load font..." << endl;
	Text instructions;
	instructions.setFont(f);
	instructions.setCharacterSize(24);
	instructions.setFillColor(Color::White);

	std::srand(std::time(0)); //  To seed the random number generator

	
	bool startGenerating = false; // Turns true after getting the three vertices. 


	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (numVertices < 3) // Capture only the first 3 vertices
					{
						std::cout << "the left button was pressed" << std::endl;
						std::cout << "mouse x: " << event.mouseButton.x << std::endl;
						std::cout << "mouse y: " << event.mouseButton.y << std::endl;

						// get the current mouse position in the window
						sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

						// convert it to world coordinates
						sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, mainView);
						vertices.push_back(worldPos);

						numVertices++; // Increment the count of vertices captured
						if (numVertices == 3)// To check wether the no of vertices is 3 or not
						{
							sf:: Vector2i pixelPos = sf::Mouse::getPosition(window);
							sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, mainView);
							points.push_back(worldPos);
							startGenerating = true; // Starts generating points inside the triangle
						}
					}
				}
			}
		}



		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		


		/*****************************************
			Update the scene
		*****************************************/
		//generate interior points

		if (startGenerating) { // generating the interior points of the triangle
			int iterations = 1000; //  iterations for generating points

			for (int i = 0; i < iterations; ++i) {
				int random_vertex_index = std::rand() % 3;// random numbers to select the three vertices
				sf::Vector2f random_vertex = vertices[random_vertex_index]; //selects the vertex to the random numbers generated
				sf::Vector2f last_point = points.back();// last points that was generated

				sf::Vector2f new_point;
				new_point.x = (last_point.x + random_vertex.x) / 2; // calculates the midpoint last generated and the randomly selected vertex.
				new_point.y = (last_point.y + random_vertex.y) / 2;

				points.push_back(new_point); // adds the newly generated points to the point vector. 
			}
		}

		ostringstream oss;
		//oss << "Click stuff and " << endl << "I will write stuff";
		//instructions.setString(oss.str());

		oss.str(""); // Clear the stringstream
		oss << "Click on any three points on the screen for the triangle" << endl << "\t\t\t\t\t\tPress Esc to quit"; // displays the instruction for user
		instructions.setString(oss.str());

		FloatRect textRect = instructions.getLocalBounds();
		instructions.setOrigin(textRect.left +
			textRect.width / 2.0f,
			textRect.top +
			textRect.height / 2.0f);

		instructions.setPosition(width / 2, 50);

		/*
		****************************************
		Draw the scene
		****************************************
		*/
		// Clear everything from the last run frame

		window.clear();
		// Draw our game scene here
		if (startGenerating) {
			CircleShape point_shape(1);
			point_shape.setFillColor(Color::White);

			for (const auto& point : points) {// A for loop that goes through each element of the points vector.
				point_shape.setPosition(point); // sets the position of circleShape to newPoints of point vector
				window.draw(point_shape);
			}
		}
		//RectangleShape r{ Vector2f{4,4} }; ///width, height.  Center uninitialized
		window.setView(mainView);
		CircleShape r(2);
		r.setFillColor(Color::Magenta);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			r.setPosition(Vector2f{ vertices.at(i).x, vertices.at(i).y });
			window.draw(r);
		}
		window.draw(instructions);
		// Show everything we just drew
		window.display();
	}

	return 0;
}