#include "header.h"

// render settings

const ContextSettings& setSettings() {
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	return settings;
}

// random function

std::default_random_engine generator(std::random_device{}());
int random_number(std::default_random_engine& generator, const int& begin, const  int& end) {
	std::uniform_int_distribution<int> distribution(begin, end);
	return distribution(generator);
}

////////////////// classes

class Snowflake {
public:
	float x;
	float y;
	float speed;
	Sprite sprite;

	Snowflake() : x(0), y(0), speed(10.f) {}
};

//

class SnowyText : public Text {
private:
	std::unique_ptr<Text> textObj;
public:
	SnowyText(const Font& font, const int& fontSize, const std::string& textValue, const Vector2f& vector): textObj(std::make_unique<Text>()) {
		textObj->setFont(font);
		textObj->setCharacterSize(fontSize);
		textObj->setFillColor(Color::Black);
		textObj->setOutlineColor(Color(138, 202, 255, 180));
		textObj->setOutlineThickness(2.f);
		textObj->setString(textValue);
		textObj->setPosition(vector);
	}

	Text& getText() {
		return *textObj;
	}

	virtual void setString(const std::string& text) {
		textObj->setString(text);
	}
};

//

class SnowySound {
private:
	SoundBuffer buffer;
	Sound sound;
public:

	SnowySound(const std::string& path, bool isLooped) {
		buffer.loadFromFile(path);
		sound.setBuffer(buffer);
		sound.setLoop(isLooped);
	}

	void play() {
		sound.play();
	}

	void stop() {
		sound.stop();
	}

};

//////////////////////////////////////////

//////////////////// inits (better in class or func)

RenderWindow window(VideoMode(320, 420), "Snowy Stack", sf::Style::Close, setSettings());


constexpr int M = 20; // height
constexpr int N = 10; // width

int field[M][N] = { 0 }; // game field

constexpr int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

struct Point
{
	int x, y;
};

Font font;

const std::string oneLinePath("resources\\bells.wav");
const std::string bgSoundPath("resources\\bg_music.wav");

const std::string blockTexturePath("resources\\ice_cube.png");
const std::string snowflake_png{ "resources\\snowflake.png" };
const std::string cube_texture_png{ "resources\\ice_cube.png" };

SnowySound oneLineSound(oneLinePath, false);
SnowySound bgSound(bgSoundPath, true);

// better name... if only i knew what it does

std::vector<Point> a(4);
std::vector<Point> b(4);

int dx = 0; 
bool rotate = 0; 
int colorNum = 1; 
bool beginGame = true; 
// #include <random> in the future :)
int n = random_number(generator, 0, 6);

float timer = 0;
float delay = 0.3f;

int score = 0; // counter score

std::vector<Snowflake> snowflakes; // snowflakes vector
auto snowflakeTexture = std::make_unique<Texture>();
auto snowflake = std::make_unique<Snowflake>();

// better use std::array<std::array<RectangleShape, N>, M> if you do not change size in runtime. 
// THEN IT'S NOT WORKING

std::vector<std::vector<RectangleShape>> cells(M, std::vector<RectangleShape>(N));

Texture blockTexture;
Sprite sprite;

///////////////////////////////////////////



///////////////////// functions


const bool checkBorder(const std::vector<Point>& a)
{
	for (const Point& p : a) {
		if (p.x < 0 || p.x >= N || p.y >= M || field[p.y][p.x] != 0) {
			return false;
		}
	}
	return true;
}



void setCellsPosition(std::vector<std::vector<RectangleShape>>&cells ) {
	const float cellSize = 18.f;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			cells[i][j].setSize(Vector2f(cellSize, cellSize));
			cells[i][j].setOutlineColor(Color(138, 202, 255, 100));
			cells[i][j].setOutlineThickness(1);
			cells[i][j].setPosition(j * cellSize, i * cellSize);
			cells[i][j].setFillColor(Color(191, 226, 255, 100));
		}
	}
}



void stopAllSounds() {
	oneLineSound.stop();
	bgSound.stop();
}

constexpr int cell_count{ 4 };
// rotation
void rotateFunc() {
	if (rotate)
	{

		Point &p = a[1];
		for (int i = 0; i < cell_count; i++)
		{
			int x = a[i].y - p.y;
			int y = a[i].x - p.x;
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}

		if (!checkBorder(a)) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

	}
}

// brick falling 
void brickFalling() {
	if (timer > delay)
	{
		for (int i = 0; i < cell_count; i++) 
		{ 
			b[i] = a[i]; 
			a[i].y += 1; 
		}
		if (!checkBorder(a))
		{
			for (int i = 0; i < cell_count; i++) 
				field[b[i].y][b[i].x] = colorNum;

			colorNum = random_number(generator, 1, 7);
			n = random_number(generator, 0, 6);
			for (int i = 0; i < cell_count; i++)
			{
				a[i].x = figures[n][i] % 2;
				a[i].y = figures[n][i] / 2;
			}
		}
		timer = 0;
	}
}


// line checking
void lineChecking() {

	int k = M - 1;

	for (int i = M - 1; i >= 0; i--)
	{
		int count = 0;

		for (int j = 0; j < N; j++)
		{
			if (field[i][j]) { count++; }
			field[k][j] = field[i][j];

		}
		if (count < N) {
			k--;
		}
		else {
			score++;
			oneLineSound.play();
		}

	}
}



void setWindowIcon(const std::string& iconPath) {
	Image icon;
	if (icon.loadFromFile(iconPath)) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
}



void snowfallFunc(const std::string &texturePath) {
	constexpr int snowflake_count{ 60 };

	snowflakeTexture->loadFromFile(texturePath);

	// rand is bad??
	for (int i = 0; i < snowflake_count; i++) {
		snowflake->x = random_number(generator, 0, 320);
		snowflake->y = random_number(generator, 0, 420);
		snowflake->speed = 40.0f + random_number(generator, 0, 100);
		snowflake->sprite.setTexture(*snowflakeTexture);
		snowflake->sprite.setColor(Color(random_number(generator, 0, 255), random_number(generator, 200, 255), 255, 100));
		snowflake->sprite.setScale(0.4f, 0.4f);
		snowflake->sprite.setPosition(snowflake->x, snowflake->y);

		snowflakes.push_back(*snowflake);
	}
}



void setBlockTexture(const std::string& texturePath) {

	blockTexture.loadFromFile(blockTexturePath);
	sprite.setTexture(blockTexture);
	sprite.setTextureRect(IntRect(0, 0, 18, 18));
}


bool spawnBlocks(bool beginGame) {
	if (beginGame)
	{
		beginGame = false;
		n = random_number(generator, 0, 6);
		for (int i = 0; i < 4; i++)
		{
			a[i].x = figures[n][i] % 2;
			a[i].y = figures[n][i] / 2;
		}
	}
	dx = 0; 
	rotate = 0; 
	delay = 0.3;

	return beginGame;
}



void drawingSnowflakes() {
	for (const Snowflake& snowflake : snowflakes) {
		window.draw(snowflake.sprite);
	}
}



void drawingBlocks() {
	window.clear(Color::White); // set BG

	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			window.draw(cells[i][j]);
		}
	}

	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
		{
			if (field[i][j] == 0) continue;
			sprite.setPosition(j * 18, i * 18);
			window.draw(sprite);
		}
	drawingSnowflakes();

	for (int i = 0; i < cell_count; i++)
	{
		sprite.setPosition(a[i].x * 18, a[i].y * 18);
		//sprite.move(64, 0);
		window.draw(sprite);
	}
}



void drawingText(const std::string &fontPath) {
	font.loadFromFile(fontPath);
	// score title
	SnowyText scoreText(font, 24, std::string("Score"), Vector2f(210, 10));
	scoreText.setString("Score: " + std::to_string(score));
	window.draw(scoreText.getText());

	// keywords
	SnowyText a_text(font, 16, std::string("A - Left"), Vector2f(200, 50));
	window.draw(a_text.getText());

	SnowyText d_text(font, 16, std::string("D - Right"), Vector2f(200, 80));
	window.draw(d_text.getText());

	SnowyText s_text(font, 16, std::string("S - Speed Up"), Vector2f(200, 110));
	window.draw(s_text.getText());

	SnowyText space_text(font, 16, std::string("Space - Rotate"), Vector2f(200, 140));
	window.draw(space_text.getText());

	SnowyText y_text(font, 14, std::string("v. 0.1"), Vector2f(10, 400));
	window.draw(y_text.getText());

	SnowyText logo_text(font, 24, std::string("Snowy Stack"), Vector2f(100, 376));
	window.draw(logo_text.getText());
}

///////////////////////// run

const std::string font_otf{ "resources\\HomeChristmas.otf" };

void run() {
	Clock clock;
	Clock clockSnowflake;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		float deltaTime = clockSnowflake.restart().asSeconds();
		// Snowflakes falling
		for (Snowflake& snowflake : snowflakes) {
			snowflake.y += snowflake.speed * deltaTime;
			snowflake.x += 20.f * deltaTime;
			if (snowflake.y > 420) {
				snowflake.y = -20.f;
				snowflake.x = random_number(generator , -30,320);
			}
			snowflake.sprite.setPosition(snowflake.x, snowflake.y);
		}

		Event event;
		while (window.pollEvent(event))
		{

			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			}
			if (event.type == Event::KeyPressed) {
				switch (event.key.code) {
				case Keyboard::Space:
					rotate = true;
					break;
				case Keyboard::A:
					dx = -1;
					break;
				case Keyboard::D:
					dx = 1;
					break;

				}

			}
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
			delay = 0.05;


		for (int i = 0; i < 4; i++) { 
			b[i] = a[i]; 
			a[i].x += dx; 
		}

		if (!checkBorder(a)) 
			for (int i = 0; i < 4; i++) 
				a[i] = b[i];


		// rotation
		rotateFunc();

		// brick falling
		brickFalling();

		// line checking
		lineChecking();

		beginGame = spawnBlocks(beginGame);

		// canvas
		drawingBlocks();

		// drawing text
		drawingText(font_otf);

		window.display();

	}
}

//////////////////////////////////




int main()
{
	setWindowIcon(snowflake_png);
	bgSound.play();
	snowfallFunc(snowflake_png);
	setCellsPosition(cells);
	setBlockTexture(cube_texture_png);
	run();
	stopAllSounds();

	return 0;
}