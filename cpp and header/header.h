#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <time.h>
#include <memory>
#include <random>

using namespace sf;

const ContextSettings& setSettings();

int random_number(const int& begin, const  int& end);

class Snowflake {
public:
	float x;
	float y;
	float speed;
	Sprite sprite;

	Snowflake();
};

class SnowyText : public Text {
protected:
	std::unique_ptr<Text> textObj;
public:
	SnowyText(const Font& font, const int& fontSize, const std::string& textValue, const Vector2f& vector);

	Text& getText();

	virtual void setString(const std::string& text);
};

class SnowySound {
private:
	SoundBuffer buffer;
	Sound sound;
public:

	SnowySound(const std::string& path, bool isLooped);

	void play();

	void stop();
};


struct Point
{
	int x, y;
};

Font font;

extern std::vector<Point> a;
extern std::vector<Point> b;

Texture blockTexture;
Sprite sprite;

bool checkBorder(const std::vector<Point>& a);
void setCellsPosition(std::vector<std::vector<RectangleShape>>& cells);
void stopAllSounds();
void rotateFunc();
void brickFalling();
void lineChecking();
void setWindowIcon(const std::string& iconPath);
void snowfallFunc(const std::string& texturePath);
void setBlockTexture(const std::string& texturePath);
bool spawnBlocks(bool beginGame);
void drawingSnowflakes();
void drawingBlocks();
void drawingText(const std::string& fontPath);
void run();



