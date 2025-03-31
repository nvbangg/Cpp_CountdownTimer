#include "common.h"

int WINDOW_WIDTH = 600; // Chiều rộng cửa sổ SDL
int WINDOW_HEIGHT = 400; // Chiều cao cửa sổ SDL

SDL_Window *window = NULL;     // Con trỏ tới cửa sổ SDL
SDL_Renderer *renderer = NULL; // Con trỏ tới renderer SDL

int main(int argc, char *argv[])
{
    // Khởi tạo các thành phần SDL
    initSDL(window, renderer);

    bool reset = false; // Biến để kiểm tra xem có cần reset đếm ngược không
    do
    {
        // Nhập thời gian và lấy thời gian nhập vào
        int countdownTime = inputTime(window, renderer);

        // Chạy đếm ngược và kiểm tra xem có cần reset không
        reset = runCountdown(window, renderer, countdownTime);

        // Nếu cần reset, đặt lại thời gian trong file về 0
        if (reset)
        {
            saveTimeToFile(0);
        }
    } while (reset); // Lặp lại nếu cần reset

    // Đóng SDL và giải phóng tài nguyên
    closeSDL(window, renderer);

    return 0; // Kết thúc chương trình
}