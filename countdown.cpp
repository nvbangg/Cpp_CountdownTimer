#include "common.h"

// Hàm định dạng thời gian từ giây thành chuỗi "HH:MM:SS"
string formatTime(int seconds)
{
    // Tính số giờ, phút giây
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    int secs = seconds % 60;
    // Trả về chuỗi định dạng "HH:MM:SS"
    return (hours < 10 ? "0" : "") + to_string(hours) + ":" +
           (minutes < 10 ? "0" : "") + to_string(minutes) + ":" +
           (secs < 10 ? "0" : "") + to_string(secs);
}

// Hàm thực hiện việc đếm ngược
bool runCountdown(SDL_Window *window, SDL_Renderer *renderer, int &countdownTime)
{
    // Mở font chữ với kích thước 100
    TTF_Font *font = TTF_OpenFont("data/digital.ttf", 100);
    // Mở font chữ với kích thước 50
    TTF_Font *textFont = TTF_OpenFont("data/digital.ttf", 50);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    // Mở âm thanh
    Mix_Chunk *alarm = Mix_LoadWAV("data/alarm.mp3");

    // Biến điều khiển vòng lặp
    bool running = true, paused = false, alarmPlayed = false;
    SDL_Event e;

    // Lấy thời gian bắt đầu
    Uint32 startTime = SDL_GetTicks(); // Thời gian bắt đầu của bộ đếm ngược
    Uint32 pauseStartTime = 0;         // Thời gian bắt đầu của trạng thái tạm dừng
    Uint32 elapsedPausedTime = 0;      // Tổng thời gian đã tạm dừng
    int lastRemainingTime = -1;        // Thời gian còn lại cuối cùng của bộ đếm ngược

    while (running)
    {
        // Định nghĩa vị trí và kích thước của nút "Pause"
        SDL_Rect pauseButton = {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 50, 80, 40};
        // Định nghĩa vị trí và kích thước của nút "Reset"
        SDL_Rect resetButton = {WINDOW_WIDTH / 2 + 20, WINDOW_HEIGHT / 2 + 50, 80, 40};

        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0)
        {
            // Kiểm tra sự kiện thoát
            if (e.type == SDL_QUIT)
            {
                closeSDL(window, renderer);
                exit(0);
            }
            // Xử lý sự kiện cửa sổ
            if (e.type == SDL_WINDOWEVENT)
                handleWindowEvent(e, window);
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Xử lý nhấn nút "Pause" hoặc "Repeat"
                if (checkClickButton(x, y, pauseButton))
                {
                    if (lastRemainingTime > 0)
                    {
                        // Đổi trạng thái tạm dừng
                        paused = !paused;
                        if (paused)
                            pauseStartTime = SDL_GetTicks(); // Lưu thời gian bắt đầu tạm dừng
                        else
                            elapsedPausedTime += SDL_GetTicks() - pauseStartTime; // Cộng thời gian tạm dừng
                    }
                    else 
                    {
                        // Khởi động lại countdown
                        startTime = SDL_GetTicks();
                        elapsedPausedTime = 0;
                        paused = false;
                        alarmPlayed = false;
                        // Dừng âm thanh ngay lập tức
                        Mix_HaltChannel(-1);
                    }
                }

                // Nếu nhấn nút "Reset", khởi động lại countdown
                if (checkClickButton(x, y, resetButton))
                {
                    // Dừng âm thanh ngay lập tức
                    Mix_HaltChannel(-1);
                    return true;
                }
            }
        }

        if (!paused)
        {
            // Tính toán thời gian còn lại
            int remainingTime = countdownTime - (SDL_GetTicks() - startTime - elapsedPausedTime) / 1000;
            if (remainingTime < 0)
                remainingTime = 0;

            lastRemainingTime = remainingTime;
            // Xóa renderer với màu nền
            clearRenderer(renderer, BACKGROUND_COLOR);

            // Vẽ thời gian còn lại hoặc "TIME OUT"
            string timeText = (remainingTime > 0) ? formatTime(remainingTime) : "TIME OUT";
            drawText(renderer, font, timeText.c_str(), TIME_COLOR, 0, -80);
            // Vẽ thanh tiến trình
            drawProgressBar(renderer, countdownTime, remainingTime);

            if (remainingTime > 0)
            {
                // Vẽ nút "Pause" hoặc "Resume"
                drawButton(renderer, paused ? "Resume" : "Pause", pauseButton);
            }
            else
            {
                // Khi hết thời gian, đổi nút "Pause" thành "Repeat"
                drawButton(renderer, "Repeat", pauseButton);
                if (!alarmPlayed)
                {
                    // Phát âm thanh báo động khi hết thời gian
                    Mix_PlayChannel(-1, alarm, 0);
                    // Đánh dấu đã phát âm thanh
                    alarmPlayed = true;
                }
            }
            // Vẽ nút "Reset"
            drawButton(renderer, "Reset", resetButton);
            // Hiển thị renderer
            SDL_RenderPresent(renderer);
        }
        else
        {
            // Xóa renderer với màu nền
            clearRenderer(renderer, BACKGROUND_COLOR);
            // Vẽ chữ "Paused"
            drawText(renderer, textFont, "Paused", PAUSE_TEXT_COLOR, 0, -150);
            // Vẽ thời gian còn lại
            drawText(renderer, font, formatTime(lastRemainingTime).c_str(), TIME_COLOR, 0, -80);
            // Vẽ nút "Resume"
            drawButton(renderer, "Resume", pauseButton);
            // Vẽ nút "Reset"
            drawButton(renderer, "Reset", resetButton);
            // Hiển thị renderer
            SDL_RenderPresent(renderer);
        }

        // Giảm tải CPU
        SDL_Delay(1000 / 60);
    }
    // Đóng font chữ và giải phóng âm thanh
    TTF_CloseFont(font);
    TTF_CloseFont(textFont);
    Mix_FreeChunk(alarm);

    // Kết thúc hàm
    return false;
}
