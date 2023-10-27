#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <time.h>
#include <memory>

using namespace sf;



const ContextSettings& setSettings();
void setCellsPosition(std::vector<std::vector<RectangleShape>>& cells);
void stopAllSounds();
void rotateFunc();
void brickFalling();
void lineChecking();
void setWindowIcon(const std::string& iconPath);
void snowfallFunc(const std::string& texturePath);
void setBlockTexture(const std::string& texturePath);
void spawnBlocks(bool& beginGame);
void drawingSnowflakes();
void drawingBlocks();
void drawingText(const std::string& fontPath);
void run();

