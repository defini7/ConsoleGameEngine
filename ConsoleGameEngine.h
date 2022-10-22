#pragma once

#pragma region consolegameengine_license
/***
*	BSD 3-Clause License

	Copyright (c) 2021, 2022 Alex
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from
	   this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***/
#pragma endregion

#pragma region consolegameengine_sample
/**
* Example (engine only supports .spr files, check [this](https://github.com/defini7/SpriteEditor) for editing .spr files):
	#include "ConsoleGameEngine.h"

	using namespace def;

	class Example : public def::ConsoleGameEngine
	{
	public:
		Example()
		{
			sAppName = L"Example";
		}

	protected:
		bool OnUserCreate() override
		{
			return true;
		}

		bool OnUserUpdate(float fDeltaTime) override
		{
			for (int i = 0; i < GetScreenWidth(); i++)
				for (int j = 0; j < GetScreenHeight(); j++)
					Draw(i, j, def::PIXEL_SOLID, rand() % 15);

			return true;
		}
	};

	int main()
	{
			Example demo;
			def::rcode err = demo.ConstructConsole(120, 40, 12, 12);

			if (err.ok)
				demo.Run();
			else
				std::cerr << err.info << "\n";

			return 0;
	}
**/
#pragma endregion

#ifndef UNICODE
#error Enable Unicode in settings
#endif

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include <vector>
#include <chrono>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <thread>
#include <string>
#include <list>
#include <cstdint>

#pragma comment(lib, "winmm.lib")

#ifdef XBOX_CONTROLLER
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#endif

namespace def
{
	enum FG_COLORS : short
	{
		FG_BLACK,
		FG_DARK_BLUE,
		FG_DARK_GREEN,
		FG_DARK_CYAN,
		FG_DARK_RED,
		FG_DARK_MAGENTA,
		FG_DARK_YELLOW,
		FG_GREY,
		FG_DARK_GREY,
		FG_BLUE,
		FG_GREEN,
		FG_CYAN,
		FG_RED,
		FG_MAGENTA,
		FG_YELLOW,
		FG_WHITE
	};

	enum BG_COLORS : short
	{
		BG_BLACK = 0x0000,
		BG_DARK_BLUE = 0x0010,
		BG_DARK_GREEN = 0x0020,
		BG_DARK_CYAN = 0x0030,
		BG_DARK_RED = 0x0040,
		BG_DARK_MAGENTA = 0x0050,
		BG_DARK_YELLOW = 0x0060,
		BG_GREY = 0x0070,
		BG_DARK_GREY = 0x0080,
		BG_BLUE = 0x0090,
		BG_GREEN = 0x00A0,
		BG_CYAN = 0x00B0,
		BG_RED = 0x00C0,
		BG_MAGENTA = 0x00D0,
		BG_YELLOW = 0x00E0,
		BG_WHITE = 0x00F0
	};

	enum PIXEL_TYPE : short
	{
		PIXEL_SOLID = 0x2588,
		PIXEL_THREEQUARTERS = 0x2593,
		PIXEL_HALF = 0x2592,
		PIXEL_QUARTER = 0x2591
	};

	enum COMMON_LVB : int
	{
		CL_GRID_HORIZONTAL = 0x400,
		CL_GRID_LVERTICAL = 0x0800,
		CL_GRID_RVERTICAL = 0x1000,
		CL_UNDERSCORE = 0x8000
	};

	struct rcode
	{
		rcode() = default;

		rcode(bool bOk)
		{
			ok = bOk;
			info = "ok";
		}

		rcode(bool bOk, const std::string& sInfo)
		{
			ok = bOk;
			info = sInfo;
		}

		operator bool()
		{
			return ok;
		}

		bool ok;
		std::string info;
	};

	namespace Font
	{
		std::wstring CONSOLAS = L"Consolas";
		std::wstring COURIER_NEW = L"Courier New";
		std::wstring LUCIDA_CONSOLE = L"Lucida Console";
		std::wstring LUCIDA_SANS_TYPEWRITER = L"Lucida Sans Typewriter";
		std::wstring MS_GOTHIC = L"MS Gothic";
		std::wstring NSIMSUN = L"NSimSum";
		std::wstring SIMSUM_EXTB = L"SimSun-ExtB";
	}

	template <class T>
	struct vec2d_basic
	{
		vec2d_basic() = default;
		vec2d_basic(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		T x;
		T y;

		friend vec2d_basic<T> operator+(const vec2d_basic<T>& v1, const vec2d_basic<T>& v2) { return { v1.x + v2.x, v1.y + v2.y }; }
		friend vec2d_basic<T> operator-(const vec2d_basic<T> v1, const vec2d_basic<T>& v2) { return { v1.x - v2.x, v1.y - v2.y }; }
		friend vec2d_basic<T> operator*(const vec2d_basic<T> v1, const vec2d_basic<T>& v2) { return { v1.x * v2.x, v1.y * v2.y }; }
		friend vec2d_basic<T> operator/(const vec2d_basic<T> v1, const vec2d_basic<T>& v2) { return { v1.x / v2.x, v1.y / v2.y }; }
		friend vec2d_basic<T> operator+(const vec2d_basic<T> v1, const T v) { return { v1.x + v, v1.y + v }; }
		friend vec2d_basic<T> operator-(const vec2d_basic<T> v1, const T v) { return { v1.x - v, v1.y - v }; }
		friend vec2d_basic<T> operator*(const vec2d_basic<T> v1, const T v) { return { v1.x * v, v1.y * v }; }
		friend vec2d_basic<T> operator/(vec2d_basic<T> v1, const T v) { return { v1.x / v,	v1.y / v }; }

		friend vec2d_basic<T> operator+=(vec2d_basic<T>& v1, const vec2d_basic<T>& v2)
		{
			v1.x += v2.x;
			v1.y += v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator-=(vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			v1.x -= v2.x;
			v1.y -= v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator*=(vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			v1.x *= v2.x;
			v1.y *= v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator/=(vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			v1.x /= v2.x;
			v1.y /= v2.y;
			return v1;
		}

		friend vec2d_basic<T> operator+=(vec2d_basic<T> v1, const T v)
		{
			v1.x += v;
			v1.y += v;
			return v1;
		}

		friend vec2d_basic<T> operator-=(vec2d_basic<T> v1, const T v)
		{
			v1.x -= v;
			v1.y -= v;
			return v1;
		}

		friend vec2d_basic<T> operator*=(vec2d_basic<T> v1, const T v)
		{
			v1.x *= v;
			v1.y *= v;
			return v1;
		}

		friend vec2d_basic<T> operator/=(vec2d_basic<T> v1, const T v)
		{
			v1.x /= v;
			v1.y /= v;
			return v1;
		}

		friend bool operator<(const vec2d_basic<T> v1, const vec2d_basic<T> v) { return v1.x < v.x&& v1.y < v.y; }
		friend bool operator>(const vec2d_basic<T> v1, const vec2d_basic<T> v) { return v1.x > v.x && v1.y > v.y; }
		friend bool operator<=(const vec2d_basic<T> v1, const vec2d_basic<T> v) { return v1.x <= v.x && v1.y <= v.y; }
		friend bool operator>=(const vec2d_basic<T> v1, const vec2d_basic<T> v) { return v1.x >= v.x && v1.y >= v.y; }
		friend bool operator==(const vec2d_basic<T> v1, const vec2d_basic<T> v) { return v1.x == v.x && v1.y == v.y; }
		friend bool operator!=(const vec2d_basic<T> v1, const vec2d_basic<T> v) { return v1.x != v.x || v1.y != v.y; }

		friend bool operator<(const vec2d_basic<T> v1, const T v) { return v1.x < v&& v1.y < v; }
		friend bool operator>(const vec2d_basic<T> v1, const T v) { return v1.x > v && v1.y > v; }
		friend bool operator<=(const vec2d_basic<T> v1, const T v) { return v1.x <= v && v1.y <= v; }
		friend bool operator>=(const vec2d_basic<T> v1, const T v) { return v1.x >= v && v1.y >= v; }
		friend bool operator==(const vec2d_basic<T> v1, const T v) { return v1.x == v && v1.y == v; }
		friend bool operator!=(const vec2d_basic<T> v1, const T v) { return v1.x != v || v1.y != v; }

		template <typename T1>
		vec2d_basic<T1> to() { return vec2d_basic<T1>((T1)this->x, (T1)this->y); }

		float dot(vec2d_basic<T> v)
		{
			return this->x * v.x + this->y * v.y;
		}

		float length()
		{
			return sqrtf(dot(*this));
		}

		vec2d_basic<T> norm()
		{
			float l = length();
			return vec2d_basic<T>(this->x / l, this->y / l);
		}

		T mag() { return T(sqrtf(this->x * this->x + this->y * this->y)); }
		T mag2() { return T(this->x * this->x + this->y * this->y); }
		
		vec2d_basic<T> abs() { return vec2d_basic<T>(std::abs(this->x), std::abs(this->y)); }
		vec2d_basic<T> perp() { return vec2d_basic<T>(-this->y, this->x); }
		vec2d_basic<T> floor() { return vec2d_basic<T>(std::floor(this->x), std::floor(this->y)); }
		vec2d_basic<T> ceil() { return vec2d_basic<T>(std::ceil(this->x), std::ceil(this->y)); }
		vec2d_basic<T> cart() { return vec2d_basic<T>(cos(this->y) * x, sin(this->y) * this->x); }
		vec2d_basic<T> polar() { return vec2d_basic<T>(mag(), atan2(this->y, this->x)); }
		vec2d_basic<T>& ref() { return *this; }
	};

	typedef vec2d_basic<int> vi2d;
	typedef vec2d_basic<float> vf2d;

	struct sKeyState
	{
		bool bHeld;
		bool bReleased;
		bool bPressed;
	};

	const float PI = 2.0f * acosf(0.0f);

	class Sprite
	{
	public:
		Sprite()
		{
			Create(8, 8);
		}

		Sprite(int w, int h)
		{
			Create(w, h);
		}

		Sprite(std::wstring sFileName)
		{
			if (!Load(sFileName))
				Create(8, 8);
		}

	private:
		short* m_Glyphs = nullptr;
		short* m_Colours = nullptr;

	public:
		int nWidth = 0;
		int nHeight = 0;

	private:
		void Create(int w, int h)
		{
			nWidth = w;
			nHeight = h;

			m_Glyphs = new short[w * h];
			m_Colours = new short[w * h];

			for (int i = 0; i < w * h; i++)
			{
				m_Glyphs[i] = L' ';
				m_Colours[i] = FG_BLACK;
			}
		}

	public:
		void SetGlyph(int x, int y, short c)
		{
			if (x > 0 || x < nWidth || y > 0 || y < nHeight)
				m_Glyphs[y * nWidth + x] = c;
		}

		void SetColour(int x, int y, short c)
		{
			if (x > 0 || x < nWidth || y > 0 || y < nHeight)
				m_Colours[y * nWidth + x] = c;
		}

		short GetGlyph(int x, int y)
		{
			if (x > 0 || x < nWidth || y > 0 || y < nHeight)
				return m_Glyphs[y * nWidth + x];

			return L' ';
		}

		short GetColour(int x, int y)
		{
			if (x > 0 || x < nWidth || y > 0 || y < nHeight)
				return m_Colours[y * nWidth + x];

			return FG_BLACK;
		}

		bool Save(std::wstring sFile)
		{
			FILE* f = nullptr;

			_wfopen_s(&f, sFile.c_str(), L"wb");

			if (f == nullptr)
				return false;

			fwrite(&nWidth, sizeof(int), 1, f);
			fwrite(&nHeight, sizeof(int), 1, f);

			fwrite(m_Colours, sizeof(short), nWidth * nHeight, f);
			fwrite(m_Glyphs, sizeof(short), nWidth * nHeight, f);

			fclose(f);

			return true;
		}

		bool Load(std::wstring sFile)
		{
			delete[] m_Glyphs;
			delete[] m_Colours;

			nWidth = 0;
			nHeight = 0;

			FILE* f = nullptr;

			_wfopen_s(&f, sFile.c_str(), L"rb");

			if (f == nullptr)
				return false;

			std::fread(&nWidth, sizeof(int), 1, f);
			std::fread(&nHeight, sizeof(int), 1, f);

			Create(nWidth, nHeight);

			std::fread(m_Colours, sizeof(short), nWidth * nHeight, f);
			std::fread(m_Glyphs, sizeof(short), nWidth * nHeight, f);

			std::fclose(f);

			return true;
		}
	};

	class ConsoleGameEngine
	{
	public:
		ConsoleGameEngine()
		{
			hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
			hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

			hWnd = GetConsoleWindow();

			hDC = GetDC(hWnd);

			sAppName = L"Undefined";
			sFont = L"Consolas";
		}

		virtual ~ConsoleGameEngine()
		{
			delete[] pScreen;
		}

	public:
		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float fDeltaTime) = 0;

		rcode ConstructConsole(int width = 120, int height = 40, int fontw = 4, int fonth = 4)
		{
			if (width <= 0 || height <= 0 || fontw <= 0 || fonth <= 0)
				return rcode(false, "Invalid width or height");

			nScreenWidth = width;
			nScreenHeight = height;

			nFontW = fontw;
			nFontH = fonth;

			hConsoleOut = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

			if (hConsoleOut == INVALID_HANDLE_VALUE)
				return rcode(false, "Can't create console Screen buffer for output");

			rRectWindow = { 0, 0, 1, 1 };
			SetConsoleWindowInfo(hConsoleOut, TRUE, &rRectWindow);
			
			COORD coord = { (short)nScreenWidth, (short)nScreenHeight };
			if (!SetConsoleScreenBufferSize(hConsoleOut, coord))
				return rcode(false, "Too large or to small Screen width or height");

			if (!SetConsoleActiveScreenBuffer(hConsoleOut))
				return rcode(false, "Can't set console Screen buffer");

			CONSOLE_FONT_INFOEX cfi;
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = nFontW;
			cfi.dwFontSize.Y = nFontH;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;

			wcscpy_s(cfi.FaceName, sFont.c_str());
			if (!SetCurrentConsoleFontEx(hConsoleOut, false, &cfi))
				return rcode(false, "Can't set font");

			if (!SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
				return rcode(false, "Could not set console mode (ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)");

			CONSOLE_SCREEN_BUFFER_INFO csbi;
			if (!GetConsoleScreenBufferInfo(hConsoleOut, &csbi))
				return rcode(false, "Could not get console Screen buffer info");

			if (nScreenHeight > csbi.dwMaximumWindowSize.Y)
				return rcode(false, "Specified Screen height larger than maximum window height");

			if (nScreenWidth > csbi.dwMaximumWindowSize.X)
				return rcode(false, "Specified Screen width larger than maximum window width");

			rRectWindow = { 0, 0, short(nScreenWidth - 1), short(nScreenHeight - 1) };
			SetConsoleWindowInfo(hConsoleOut, TRUE, &rRectWindow);
			
			pScreen = new CHAR_INFO[nScreenWidth * nScreenHeight];
			memset(pScreen, 0, sizeof(CHAR_INFO) * nScreenWidth * nScreenHeight);

			return rcode(true, "Ok");
		}

		void Run()
		{
			bGameThreadActive = true;

			std::thread t = std::thread(&def::ConsoleGameEngine::AppThread, this);
			t.join();
		}

	public:
		template <typename T>
		void Draw(vec2d_basic<T> pos, short c = 0x2588, short col = 0x000F);
		virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void DrawLine(vec2d_basic<T> pos1, vec2d_basic<T> pos2, short c = 0x2588, short col = 0x000F);
		virtual void DrawLine(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void DrawTriangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, vec2d_basic<T> pos3, short c = 0x2588, short col = 0x000F);
		virtual void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void FillTriangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, vec2d_basic<T> pos3, short c = 0x2588, short col = 0x000F);
		virtual void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void DrawRectangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, short c = 0x2588, short col = 0x000F);
		virtual void DrawRectangle(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void DrawRectangleS(vec2d_basic<T> pos, vec2d_basic<T> size, short c = 0x2588, short col = 0x000F);
		virtual void DrawRectangleS(int x, int y, int size_x, int size_y, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void FillRectangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, short c = 0x2588, short col = 0x000F);
		virtual void FillRectangle(int x1, int y1, int x2, int y2, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void FillRectangleS(vec2d_basic<T> pos, vec2d_basic<T> size, short c = 0x2588, short col = 0x000F);
		virtual void FillRectangleS(int x, int y, int size_x, int size_y, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void DrawCircle(vec2d_basic<T> pos, int radius, short c = 0x2588, short col = 0x000F);
		virtual void DrawCircle(int x, int y, int radius, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void FillCircle(vec2d_basic<T> pos, int radius, short c = 0x2588, short col = 0x000F);
		virtual void FillCircle(int x, int y, int radius, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void DrawSprite(vec2d_basic<T> pos, Sprite* sprite);
		virtual void DrawSprite(int x, int y, Sprite* sprite);

		template <typename T>
		void DrawPartialSprite(vec2d_basic<T> pos, vec2d_basic<T> fpos1, vec2d_basic<T> fpos2, Sprite* sprite);
		virtual void DrawPartialSprite(int x, int y, int fx1, int fy1, int fx2, int fy2, Sprite* sprite);

		template <typename T>
		void DrawPartialSpriteS(vec2d_basic<T> pos, vec2d_basic<T> fpos1, vec2d_basic<T> fpos2, Sprite* sprite);
		virtual void DrawPartialSpriteS(int x, int y, int fx1, int fy1, int fx2, int fy2, Sprite* sprite);

		virtual void DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short c = 0x2588, short col = 0x000F);

		template <typename T>
		void DrawString(vec2d_basic<T> pos, std::wstring text, short col = 0x000F);
		virtual void DrawString(int x, int y, std::wstring text, short col = 0x000F);

		virtual void Clear(short c = 0x2588, short col = 0x000F);

		bool MakeSound(std::wstring sFilename, bool bLoop = false);
		bool Focused();

		template <typename T>
		inline vec2d_basic<T> GetMouse() const;

		inline int GetMouseX() const;
		inline int GetMouseY() const;

		inline sKeyState GetMouse(short button) const;
		inline sKeyState GetKey(short key) const;

		inline int GetCharacter(bool bHeld = true, bool bPressed = false, bool bReleased = false);

		inline int GetScreenWidth() const;
		inline int GetScreenHeight() const;

		template <typename T>
		inline vec2d_basic<T> GetScreenSize() const;

		inline float GetDeltaTime() const;

	private:
		void AppThread()
		{
			if (!OnUserCreate())
				bGameThreadActive = false;

			if (bGameThreadActive)
			{
				auto tp1 = std::chrono::system_clock::now();
				auto tp2 = std::chrono::system_clock::now();

				for (int i = 0; i < 256; i++)
					vKeys.push_back({ false, false, false });

				for (int i = 0; i < 5; i++)
					vMouse.push_back({ false, false, false });

				while (bGameThreadActive)
				{
					tp2 = std::chrono::system_clock::now();
					std::chrono::duration<float> elapsedTime = tp2 - tp1;
					tp1 = tp2;
					fDeltaTime = elapsedTime.count();

					wchar_t buffer_title[256];
					swprintf_s(buffer_title, 256, L"github.com/defini7 - Consola Prod - %s - FPS: %3.2f", sAppName.c_str(), 1.0f / fDeltaTime);
					SetConsoleTitleW(buffer_title);

					if (!OnUserUpdate(fDeltaTime))
						bGameThreadActive = false;

					INPUT_RECORD inBuf[32];
					DWORD events = 0;
					GetNumberOfConsoleInputEvents(hConsoleIn, &events);
					if (events > 0)
						ReadConsoleInputW(hConsoleIn, inBuf, events, &events);

					for (DWORD i = 0; i < events; i++)
					{
						switch (inBuf[i].EventType)
						{
						case FOCUS_EVENT:
							bFocused = inBuf[i].Event.FocusEvent.bSetFocus;
							break;

						case MOUSE_EVENT:
						{
							switch (inBuf[i].Event.MouseEvent.dwEventFlags)
							{
							case MOUSE_MOVED:
							{
								nMousePosX = inBuf[i].Event.MouseEvent.dwMousePosition.X;
								nMousePosY = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
							}
							break;

							case 0:
							{
								for (int m = 0; m < 5; m++)
									bMouseNewState[m] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
							}
							break;

							default:
								break;
							}
						}
						break;

						default:
							break;
						}
					}

					for (int i = 0; i < 256; i++)
					{
						nKeyNewState[i] = GetAsyncKeyState(i);

						vKeys[i].bPressed = false;
						vKeys[i].bReleased = false;

						if (nKeyNewState[i] != nKeyOldState[i])
						{
							if (nKeyNewState[i] & 0x8000)
							{
								vKeys[i].bPressed = !vKeys[i].bHeld;
								vKeys[i].bHeld = true;
							}
							else
							{
								vKeys[i].bReleased = true;
								vKeys[i].bHeld = false;
							}
						}

						nKeyOldState[i] = nKeyNewState[i];
					}

					for (int m = 0; m < 5; m++)
					{
						vMouse[m].bPressed = false;
						vMouse[m].bReleased = false;

						if (bMouseNewState[m] != bMouseOldState[m])
						{
							if (bMouseNewState[m])
							{
								vMouse[m].bPressed = true;
								vMouse[m].bHeld = true;
							}
							else
							{
								vMouse[m].bReleased = true;
								vMouse[m].bHeld = false;
							}
						}

						bMouseOldState[m] = bMouseNewState[m];
					}

					WriteConsoleOutputW(hConsoleOut, pScreen, { (short)nScreenWidth, (short)nScreenHeight }, { 0,0 }, &rRectWindow);
				}
			}
		}

	protected:
		std::wstring sAppName;
		std::wstring sFont;

	private:
		CHAR_INFO* pScreen = nullptr;
		HANDLE hConsoleOut;
		HANDLE hConsoleIn;
		SMALL_RECT rRectWindow;
		HWND hWnd;
		HDC hDC;

		std::vector<sKeyState> vKeys;
		std::vector<sKeyState> vMouse;

		short nKeyOldState[256];
		short nKeyNewState[256];

		bool bMouseOldState[5];
		bool bMouseNewState[5];

		int nMousePosX;
		int nMousePosY;
		
		int nScreenWidth;
		int nScreenHeight;

		int nFontW;
		int nFontH;

		float fDeltaTime;

		bool bGameThreadActive;
		bool bFocused;
	};

	bool ConsoleGameEngine::MakeSound(std::wstring sFilename, bool bLoop)
	{
		DWORD f = SND_ASYNC | SND_FILENAME;

		if (bLoop)
			f |= SND_LOOP;

		return (bool)PlaySoundW(sFilename.c_str(), nullptr, f);
	}

	bool ConsoleGameEngine::Focused()
	{
		return bFocused;
	}

	template <typename T>
	void ConsoleGameEngine::FillRectangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, short c, short col)
	{
		for (int i = pos1.x; i <= pos2.x; i++)
			for (int j = pos1.y; j <= pos2.y; j++)
			{
				pScreen[j * nScreenWidth + i].Char.UnicodeChar = c;
				pScreen[j * nScreenWidth + i].Attributes = col;
			}
	}

	void ConsoleGameEngine::FillRectangle(int x1, int y1, int x2, int y2, short c, short col)
	{
		FillRectangle<int>({ x1, y1 }, { x2, y2 }, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::FillRectangleS(vec2d_basic<T> pos, vec2d_basic<T> size, short c, short col)
	{
		FillRectangle(pos, { pos.x + size.x, pos.y + size.y }, c, col);
	}

	void ConsoleGameEngine::FillRectangleS(int x, int y, int size_x, int size_y, short c, short col)
	{
		FillRectangle<int>({ x, y }, { x + size_x, y + size_y }, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::DrawCircle(vec2d_basic<T> pos, int radius, short c, short col)
	{
		if (!radius) return;

		int x = 0;
		int y = radius;
		int p = 3 - 2 * radius;

		while (y >= x)
		{
			Draw(pos.x - x, pos.y - y, c, col);	// upper left left
			Draw(pos.x - y, pos.y - x, c, col);	// upper upper left
			Draw(pos.x + y, pos.y - x, c, col);	// upper upper right
			Draw(pos.x + x, pos.y - y, c, col);	// upper right right
			Draw(pos.x - x, pos.y + y, c, col);	// lower left left
			Draw(pos.x - y, pos.y + x, c, col);	// lower lower left
			Draw(pos.x + y, pos.y + x, c, col);	// lower lower right
			Draw(pos.x + x, pos.y + y, c, col);	// lower right right

			if (p < 0)
			{
				p += 4 * x + 6;

				x++;
			}
			else
			{
				p += 4 * (x - y) + 10;

				x++;
				y--;
			}
		}
	}

	void ConsoleGameEngine::DrawCircle(int x, int y, int radius, short c, short col)
	{
		DrawCircle<int>({ x, y }, radius, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::FillCircle(vec2d_basic<T> pos, int radius, short c, short col)
	{
		if (!radius) return;

		int x = 0;
		int y = radius;
		int p = 3 - 2 * radius;

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, c, col);
		};

		while (y >= x)
		{
			drawline(pos.x - x, pos.x + x, pos.y - y);
			drawline(pos.x - y, pos.x + y, pos.y - x);
			drawline(pos.x - x, pos.x + x, pos.y + y);
			drawline(pos.x - y, pos.x + y, pos.y + x);
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}
	}

	void ConsoleGameEngine::FillCircle(int x, int y, int radius, short c, short col)
	{
		FillCircle<int>({ x, y }, radius, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::Draw(vec2d_basic<T> pos, short c, short col)
	{
		if (pos.x >= 0 && pos.x < nScreenWidth && pos.y >= 0 && pos.y < nScreenHeight)
		{
			pScreen[pos.y * nScreenWidth + pos.x].Char.UnicodeChar = c;
			pScreen[pos.y * nScreenWidth + pos.x].Attributes = col;
		}
	}

	void ConsoleGameEngine::Draw(int x, int y, short c, short col)
	{
		Draw<int>({ x, y }, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::DrawLine(vec2d_basic<T> pos1, vec2d_basic<T> pos2, short c, short col)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

		dx = pos2.x - pos1.x;
		dy = pos2.y - pos1.y;

		dx1 = abs(dx);
		dy1 = abs(dy);

		px = 2 * dy1 - dx1;
		py = 2 * dx1 - dy1;

		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = pos1.x;
				y = pos1.y;
				xe = pos2.x;
			}
			else
			{
				x = pos2.x;
				y = pos2.y;
				xe = pos1.x;
			}

			Draw(x, y, c, col);

			for (i = 0; x < xe; i++)
			{
				x++;

				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						y = y + 1;
					else
						y = y - 1;

					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, c, col);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = pos1.x;
				y = pos1.y;
				ye = pos2.y;
			}
			else
			{
				x = pos2.x;
				y = pos2.y;
				ye = pos1.y;
			}

			Draw(x, y, c, col);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;

				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
						x = x + 1;
					else
						x = x - 1;

					py = py + 2 * (dx1 - dy1);
				}

				Draw(x, y, c, col);
			}
		}
	}

	void ConsoleGameEngine::DrawLine(int x1, int y1, int x2, int y2, short c, short col)
	{
		DrawLine<int>({ x1, y1 }, { x2, y2 }, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::DrawTriangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, vec2d_basic<T> pos3, short c, short col)
	{
		DrawLine(pos1, pos2, c, col);
		DrawLine(pos2, pos3, c, col);
		DrawLine(pos3, pos1, c, col);
	}

	void ConsoleGameEngine::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col)
	{
		DrawTriangle<int>({ x1, y1 }, { x2, y2 }, { x3, y3 }, c, col);
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	template <typename T>
	void ConsoleGameEngine::FillTriangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, vec2d_basic<T> pos3, short c, short col)
	{
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, c, col); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;

		bool changed1 = false;
		bool changed2 = false;

		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;

		// Sort vertices
		if (pos1.y > pos2.y) { std::swap(pos1.y, pos2.y); std::swap(pos1.x, pos2.x); }
		if (pos1.y > pos3.y) { std::swap(pos1.y, pos3.y); std::swap(pos1.x, pos3.x); }
		if (pos2.y > pos3.y) { std::swap(pos2.y, pos3.y); std::swap(pos2.x, pos3.x); }

		t1x = t2x = pos1.x; y = pos1.y;   // Starting points
		dx1 = (int)(pos2.x - pos1.x);

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = (int)(pos2.y - pos1.y);

		dx2 = (int)(pos3.x - pos1.x);

		if (dx2 < 0)
		{
			dx2 = -dx2;
			signx2 = -1;
		}
		else
			signx2 = 1;

		dy2 = (int)(pos3.y - pos1.y);

		if (dy1 > dx1)
		{   // swap values
			std::swap(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2)
		{   // swap values
			std::swap(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);

		// Flat top, just process the second half
		if (pos1.y == pos2.y)
			goto next;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;)
		{
			t1xp = 0; t2xp = 0;
			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}

			// process first line until y value is about to change
			while (i < dx1)
			{
				i++;
				e1 += dy1;

				while (e1 >= dx1)
				{
					e1 -= dx1;

					if (changed1)
						t1xp = signx1;
					else
						goto next1;
				}

				if (changed1)
					break;
				else
					t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1)
			{
				e2 += dy2;
				while (e2 >= dx2)
				{
					e2 -= dx2;
					if (changed2)
						t2xp = signx2;
					else
						goto next2;
				}
				if (changed2)
					break;
				else
					t2x += signx2;
			}
		next2:
			if (minx > t1x)
				minx = t1x;

			if (minx > t2x)
				minx = t2x;

			if (maxx < t1x)
				maxx = t1x;

			if (maxx < t2x)
				maxx = t2x;

			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
			// Now increase y
			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y += 1;

			if (y == pos2.y)
				break;

		}
	next:
		// Second half
		dx1 = (int)(pos3.x - pos2.x);

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = (int)(pos3.y - pos2.y);
		t1x = pos2.x;

		if (dy1 > dx1)
		{   // swap values
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else
			changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++)
		{
			t1xp = 0;
			t2xp = 0;

			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}

			// process first line until y value is about to change
			while (i < dx1)
			{
				e1 += dy1;

				while (e1 >= dx1)
				{
					e1 -= dx1;
					if (changed1)
					{
						t1xp = signx1;
						break;
					}
					else
						goto next3;
				}

				if (changed1)
					break;
				else
					t1x += signx1;

				if (i < dx1)
					i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != pos3.x)
			{
				e2 += dy2;

				while (e2 >= dx2)
				{
					e2 -= dx2;

					if (changed2)
						t2xp = signx2;
					else
						goto next4;
				}

				if (changed2)
					break;
				else
					t2x += signx2;
			}
		next4:

			if (minx > t1x)
				minx = t1x;

			if (minx > t2x)
				minx = t2x;

			if (maxx < t1x)
				maxx = t1x;

			if (maxx < t2x)
				maxx = t2x;

			drawline(minx, maxx, y);

			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y += 1;

			if (y > pos3.y)
				return;
		}
	}

	void ConsoleGameEngine::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, short c, short col)
	{
		FillTriangle<int>({ x1, y1 }, { x2, y2 }, { x3, y3 }, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::DrawRectangle(vec2d_basic<T> pos1, vec2d_basic<T> pos2, short c, short col)
	{
		for (int x = 0; x <= pos2.x - pos1.x; x++)
		{
			Draw(pos1.x + x, pos1.y, c, col);
			Draw(pos1.x + x, pos2.y, c, col);
		}

		for (int y = 0; y <= pos2.y - pos1.y; y++)
		{
			Draw(pos1.x, pos1.y + y, c, col);
			Draw(pos2.x, pos1.y + y, c, col);
		}
	}

	void ConsoleGameEngine::DrawRectangle(int x1, int y1, int x2, int y2, short c, short col)
	{
		DrawRectangle<int>({ x1, y1 }, { x2, y2 }, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::DrawRectangleS(vec2d_basic<T> pos, vec2d_basic<T> size, short c, short col)
	{
		DrawRectangle<int>(pos, { pos.x + size.x, pos.y + size.y }, c, col);
	}

	void ConsoleGameEngine::DrawRectangleS(int x, int y, int size_x, int size_y, short c, short col)
	{
		DrawRectangle<int>({ x, y }, { x + size_x, y + size_y }, c, col);
	}

	template <typename T>
	void ConsoleGameEngine::DrawSprite(vec2d_basic<T> pos, Sprite* sprite)
	{
		if (sprite == nullptr)
			return;

		for (int i = 0; i < sprite->nWidth; i++)
		{
			for (int j = 0; j < sprite->nHeight; j++)
			{
				if (sprite->GetGlyph(i, j) != L' ')
					Draw(pos.x + i, pos.y + j, sprite->GetGlyph(i, j), sprite->GetColour(i, j));
			}
		}
	}

	void ConsoleGameEngine::DrawSprite(int x, int y, Sprite* sprite)
	{
		DrawSprite<int>({ x, y }, sprite);
	}

	template <typename T>
	void ConsoleGameEngine::DrawPartialSprite(vec2d_basic<T> pos, vec2d_basic<T> fpos1, vec2d_basic<T> fpos2, Sprite* sprite)
	{
		if (sprite == nullptr || fpos1.x < 0 || fpos1.y < 0 || fpos2.x > sprite->nWidth || fpos2.y > sprite->nHeight)
			return;

		for (int i = fpos1.x, x = 0; i < fpos2.x; i++, x++)
		{
			for (int j = fpos1.y, y = 0; j < fpos2.y; j++, y++)
			{
				if (sprite->GetGlyph(i, j) != L' ')
					Draw(pos.x + x, pos.y + y, sprite->GetGlyph(i, j), sprite->GetColour(i, j));
			}
		}
	}

	void ConsoleGameEngine::DrawPartialSprite(int x, int y, int fx1, int fy1, int fx2, int fy2, Sprite* sprite)
	{
		DrawPartialSprite<int>({ x, y }, { fx1, fy1 }, { fx2, fy2 }, sprite);
	}

	template <typename T>
	void ConsoleGameEngine::DrawPartialSpriteS(vec2d_basic<T> pos, vec2d_basic<T> fpos1, vec2d_basic<T> fpos2, Sprite* sprite)
	{
		DrawPartialSprite<int>({ pos.x, pos.y }, { fpos1.x, fpos1.y }, { fpos1.x + fpos2.x, fpos1.y + fpos2.y }, sprite);
	}

	void ConsoleGameEngine::DrawPartialSpriteS(int x, int y, int fx1, int fy1, int fx2, int fy2, Sprite* sprite)
	{
		DrawPartialSprite<int>({ x, y }, { fx1, fy1 }, { fx1 + fx2, fy1 + fy2 }, sprite);
	}

	void ConsoleGameEngine::DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r, float s, short c, short col)
	{
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordinate pairs
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		// Draw Closed Polygon
		for (int i = 0; i < verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second,
				(int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second, c, col);
		}
	}

	template <typename T>
	void ConsoleGameEngine::DrawString(vec2d_basic<T> pos, std::wstring text, short col)
	{
		for (size_t i = 0; i < text.size(); i++)
		{
			pScreen[pos.y * nScreenWidth + pos.x + i].Char.UnicodeChar = text[i];
			pScreen[pos.y * nScreenWidth + pos.x + i].Attributes = col;
		}
	}

	void ConsoleGameEngine::DrawString(int x, int y, std::wstring text, short col)
	{
		DrawString<int>({ x, y }, text, col);
	}

	void ConsoleGameEngine::Clear(short c, short col)
	{
		FillRectangle(0, 0, nScreenWidth - 1, nScreenHeight - 1, c, col);
	}

	template <typename T>
	inline vec2d_basic<T> ConsoleGameEngine::GetMouse() const
	{
		return { nMousePosX, nMousePosY };
	}

	inline int ConsoleGameEngine::GetMouseX() const
	{
		return nMousePosX;
	}

	inline int ConsoleGameEngine::GetMouseY() const
	{
		return nMousePosY;
	}

	inline sKeyState ConsoleGameEngine::GetMouse(short button) const
	{
		return vMouse[button];
	}

	inline sKeyState ConsoleGameEngine::GetKey(short key) const
	{
		return vKeys[key];
	}

	inline int ConsoleGameEngine::GetScreenWidth() const
	{
		return nScreenWidth;
	}

	inline int ConsoleGameEngine::GetScreenHeight() const
	{
		return nScreenHeight;
	}

	inline int ConsoleGameEngine::GetCharacter(bool bHeld, bool bPressed, bool bReleased)
	{
		for (int i = ' '; i <= '~'; i++)
		{
			if (bHeld)
			{
				if (vKeys[i].bHeld)
					return i;
			}
			else if (bPressed)
			{
				if (vKeys[i].bPressed)
					return i;
			}
			else if (bReleased)
			{
				if (vKeys[i].bReleased)
					return i;
			}

		}

		return -1;
	}

	template <typename T>
	inline vec2d_basic<T> ConsoleGameEngine::GetScreenSize() const
	{
		return { nScreenWidth, nScreenHeight };
	}

	inline float ConsoleGameEngine::GetDeltaTime() const
	{
		return fDeltaTime;
	}
}
