#include "MiniginPCH.h"
#include "LevelParser.h"

void LevelParser::Parse()
{
	m_Reader.open(m_File);

	if (!m_Reader.is_open())
		return;

	std::string line;
	while (std::getline(m_Reader, line))
	{
		if (line.empty()) //skip empty lines
			continue;
		if (line.find("//") != std::string::npos) //skip comments
			continue;

		if (line.find("Level:") != std::string::npos)
		{
			LevelData level{};
			level.id = std::stoi(line.substr(7));
			for (int i{ 9 }; i >= 0; --i)
			{
				//read 10 lines
				std::getline(m_Reader, line);
				for (int j{}; j < 15; ++j)
				{
					level.data[j][i] = (LevelTypes)(line[j] - '0'); //to get int value, subtract ASCII value of 'zero' (== 48)
				}
			}
			m_Levels.push_back(level);
		}
	}
	m_Reader.close();
}

void LevelParser::SetFile(const std::string& file)
{
	m_File = file;
}

const std::vector<LevelData>& LevelParser::GetLevels() const
{
	return m_Levels;
}