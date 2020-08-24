#pragma once
#include <fstream>
#include <vector>

enum class LevelTypes : unsigned char //uint8_t
{
	tile,
	hole,
	gem,
	goldbag,
	spawner,
	player,
};

struct LevelData
{
	const static int TileWidth = 20;
	const static int TileHeight = 5;
	const static int Columns = 15;
	const static int Rows = 10;
	int id;
	LevelTypes data[Columns][Rows];
};

class LevelParser final
{
public:
	void Parse();
	void SetFile(const std::string& file);
	const std::vector<LevelData>& GetLevels() const;

private:
	std::ifstream m_Reader;
	std::string m_File;
	std::vector<LevelData> m_Levels;
};