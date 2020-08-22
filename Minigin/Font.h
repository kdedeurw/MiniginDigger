#pragma once

struct _TTF_Font;
class Font
{
public:
	explicit Font(const std::string& fullPath, unsigned int size);
	~Font();

	Font(const Font &) = delete;
	Font(Font &&) = delete;
	Font & operator= (const Font &) = delete;
	Font & operator= (const Font &&) = delete;

	_TTF_Font* GetFont() const;
	const unsigned int GetSize() const;

private:
	_TTF_Font* m_pFont;
	unsigned int m_Size;
};
