#pragma once
#include<string>

class ImgLoader {
public:
	int height;
	int width;
	int nrChannels;
	unsigned char* data;
	bool IsValid();
	ImgLoader(std::string img_path);
	~ImgLoader();
	void free();
private:
	bool isValid;
};
