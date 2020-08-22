#pragma once
#include <string>
#include <vector>
#include <fstream>

//added to prevent LNK2005 errors
namespace
{
	class BinaryReader
	{
	public:
		BinaryReader() = default;
		~BinaryReader();

		void OpenFile(const std::string& file);
		bool IsGood() { return m_Reader.good(); };

		template <typename T>
		T Read();

	protected:
		using byte = unsigned char;

	private:
		std::ifstream m_Reader;

	};

	BinaryReader::~BinaryReader()
	{
		m_Reader.close();
	}

	void BinaryReader::OpenFile(const std::string& file)
	{
		m_Reader.open(file, std::ios::in | std::ios::binary);
	}

	template<typename T>
	T BinaryReader::Read()
	{
		if (m_Reader.is_open() && m_Reader.good())
		{
			char read[sizeof(T)];
			m_Reader.read(read, sizeof(T));
			return static_cast<T>(*read);
		}
		else
			throw std::exception("BinaryReader::Read > unable to read");
	}
}