#pragma once

#include <winnt.h>
#include <corecrt_malloc.h>
#include <string>

class TextHolder {
public:
	// union that allows for the same memory location to be accessed with different names (gonna copy paste this comment alot)
	union {
		char inlineText[16];  //0x0000
		char* pText;          //0x0000
	};

	// length of the text
	size_t textLength;         //0x0010

	// aligned length of the text
	size_t alignedTextLength;  //0x0018

	// default constructor that sets all member variables to 0
	TextHolder() {
		memset(this, 0, sizeof(TextHolder));
	}

	// copy constructor that copies the values of the specified TextHolder object
	TextHolder(TextHolder const& copy) {
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			memcpy(inlineText, copy.inlineText, 16);
		else {
			size_t size = textLength + 1;

			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0 && copy.pText != 0x0) {
				memcpy(pText, copy.pText, size);
				pText[size] = 0;
			}
		}
	}
	
	// assignment operator that copies the values from the specified TextHolder object to the current object
	TextHolder& operator=(TextHolder const& copy) {
		deleteText();
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			memcpy(inlineText, copy.inlineText, 16);
		else {
			size_t size = textLength + 1;

			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0 && copy.pText != 0x0) {
				memcpy(pText, copy.pText, textLength);
				pText[textLength] = 0;
			}
		}
		return *this;
	}

	// constructor that initializes the TextHolder object with the provided string
	TextHolder(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16) {
			memcpy(inlineText, str.c_str(), str.size());
			if (str.size() < 15)
				inlineText[str.size()] = 0;
		}
		else {
			size_t size = str.size();
			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0) {
				memcpy(pText, str.c_str(), str.size());
				pText[str.size()] = 0;
			}
		}
	}

	// constructor that allocates memory for the text with the specified size
	TextHolder(size_t allocSize) {
		memset(this, 0, sizeof(TextHolder));
		textLength = 0;
		alignedTextLength = allocSize | 0xF;
		if (alignedTextLength < 16) {
			memset(&inlineText, 0, 16);
		}
		else {
			size_t size = alignedTextLength;
			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;

				pText += 8;
				memset(pText, 0, alignedTextLength - 8);
			}
			else
				memset(pText, 0, alignedTextLength);
		}
	}

	// constructor that initializes the TextHolder object with the provided data pointer and size
	TextHolder(void* ptr, size_t sizeOfData) {
		memset(this, 0, sizeof(TextHolder));
		textLength = sizeOfData;
		alignedTextLength = sizeOfData;
		if (alignedTextLength < 16)
			memcpy(inlineText, ptr, sizeOfData);
		else
			pText = reinterpret_cast<char*>(ptr);
	}

	// frees memory allocated for pText if necessary and resets the object to its default state
	void deleteText() {
		if (alignedTextLength >= 16 && pText != nullptr) {
			if (alignedTextLength + 1 >= 0x1000) {
				pText = *reinterpret_cast<char**>(reinterpret_cast<__int64>(pText) - 8);
			}
			free(pText);
		}
	}

	// returns a pointer to the text data stored in the object
	~TextHolder() {
		this->deleteText();
	}

	// returns the length of the text data stored in the object
	char* getText() {
		if (alignedTextLength < 16)
			return this->inlineText;
		else
			return this->pText;
	}

	// sets the text data and length for the object
	size_t getTextLength() {
		return textLength;
	}

	// returns the length of the allocated memory for pText
	void setText(std::string str) {
		this->deleteText();
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16) {
			memcpy(inlineText, str.c_str(), str.size());
			if (str.size() < 15)
				inlineText[str.size()] = 0;
		}
		else {
			size_t size = str.size();
			if (size + 1 >= 0x1000)
				size += 8;

			pText = reinterpret_cast<char*>(malloc(size + 1));
			alignedTextLength = size;
			if (size + 1 >= 0x1000) {
				*reinterpret_cast<char**>(pText) = pText;
				pText += 8;
			}

			if (pText != 0x0) {
				memcpy(pText, str.c_str(), str.size());
				pText[str.size()] = 0;
			}
		}
	}

	// returns a pointer to the allocated memory for pText
	void resetWithoutDelete() {
		memset(this, 0, sizeof(TextHolder));
	}
};