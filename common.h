#pragma once // Đảm bảo file này chỉ được include một lần trong quá trình biên dịch

#include <SDL.h> // Thư viện hỗ trợ các hàm cơ bản của SDL
#include <SDL_ttf.h> // Thư viện hỗ trợ vẽ chữ
#include <SDL_mixer.h> // Thư viện hỗ trợ âm thanh
#include <iostream>
#include <string>
#include <vector> 
#include <fstream> // Thư viện hỗ trợ đọc ghi file
using namespace std;

//! Màu sắc
const SDL_Color BACKGROUND_COLOR = {0, 0, 0, 255};   // Màu nền: đen
const SDL_Color TEXT_COLOR = {0, 0, 0, 255};         // Màu chữ: Đen
const SDL_Color TIME_COLOR = {0, 255, 0, 255};       // Màu thời gian: xanh lá
const SDL_Color BUTTON_COLOR = {255, 255, 255, 255}; // Màu nút: trắng
const SDL_Color PAUSE_TEXT_COLOR = {255, 0, 0, 255}; // Màu chữ tạm dừng: đỏ

extern int WINDOW_WIDTH; // Kích thước chiều rộng cửa sổ
extern int WINDOW_HEIGHT; // Kích thước chiều cao cửa sổ

//! Trong functions.cpp
// Hàm khởi tạo các thành phần của SDL
void initSDL(SDL_Window *&window, SDL_Renderer *&renderer);
// Hàm xử lý sự kiện cửa sổ
void handleWindowEvent(SDL_Event &e, SDL_Window *window);
// Hàm giải phóng tài nguyên của SDL
void closeSDL(SDL_Window *window, SDL_Renderer *renderer);
// Hàm lưu thời gian vào file
void saveTimeToFile(int countdownTime);
// Hàm tải thời gian từ file
int loadTimeFromFile();
// Hàm kiểm tra xem người dùng có click vào nút không
bool checkClickButton(int x, int y, const SDL_Rect &button);

//! Trong draw.cpp
// Hàm thiết lập màu nền cho renderer
void clearRenderer(SDL_Renderer *renderer, SDL_Color color);
// Hàm vẽ chữ lên renderer
void drawText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int xOffset, int yOffset);
// Hàm vẽ nút
void drawButton(SDL_Renderer *renderer, const string &label, SDL_Rect &buttonRect);
// Hàm vẽ thanh tiến trình
void drawProgressBar(SDL_Renderer *renderer, int totalTime, int remainingTime);

//! Trong file khác
//Hàm nhập thời gian
int inputTime(SDL_Window *window, SDL_Renderer *renderer);
//Hàm đếm ngược
bool runCountdown(SDL_Window *window, SDL_Renderer *renderer, int &countdownTime);
