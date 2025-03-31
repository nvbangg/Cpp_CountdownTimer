#include "common.h"

// Hàm chuyển đổi thời gian mặc định thành chuỗi định dạng HHMMSS
string convertDefaultTimeToString(int defaultTime)
{
    int defaultHours = defaultTime / 3600;
    int defaultMinutes = (defaultTime % 3600) / 60;
    int defaultSeconds = defaultTime % 60;
    return (defaultHours < 10 ? "0" : "") + to_string(defaultHours) +
           (defaultMinutes < 10 ? "0" : "") + to_string(defaultMinutes) +
           (defaultSeconds < 10 ? "0" : "") + to_string(defaultSeconds);
}

// Hàm chuyển đổi chuỗi thời gian thành số giây
int convertInputToTime(const string &inputText)
{
    // Chuyển đổi từng phần của chuỗi thành giờ, phút và giây
    int hours = stoi(inputText.substr(0, 2));
    int minutes = stoi(inputText.substr(2, 2));
    int seconds = stoi(inputText.substr(4, 2));
    // Tính tổng số giây
    return hours * 3600 + minutes * 60 + seconds;
}

// Hàm vẽ giao diện
void renderUI(SDL_Renderer *renderer, TTF_Font *font, const string &inputText, const SDL_Rect &startButton, const SDL_Rect &clearButton, const vector<SDL_Rect> &digitButtons)
{
    // Thiết lập màu nền cho renderer
    clearRenderer(renderer, BACKGROUND_COLOR);

    // Định dạng thời gian
    string formattedTime = inputText.substr(0, 2) + ":" + inputText.substr(2, 2) + ":" + inputText.substr(4, 2);
    // Vẽ thời gian lên màn hình
    drawText(renderer, font, formattedTime.c_str(), TIME_COLOR, 0, -80);

    // Vẽ nút Start và Clear
    drawButton(renderer, "Start", const_cast<SDL_Rect &>(startButton));
    drawButton(renderer, "Clear", const_cast<SDL_Rect &>(clearButton));

    // Vẽ các nút số
    for (int i = 0; i < 10; ++i)
    {
        drawButton(renderer, to_string(i), const_cast<SDL_Rect &>(digitButtons[i]));
    }

    // Hiển thị renderer lên cửa sổ
    SDL_RenderPresent(renderer);
}

// Hàm xử lý sự kiện chuột
void handleMouseEvent(SDL_Event &e, string &inputText, bool &done, const vector<SDL_Rect> &digitButtons, const SDL_Rect &startButton, const SDL_Rect &clearButton)
{
    int x, y;
    // Lấy vị trí chuột
    SDL_GetMouseState(&x, &y);

    // Kiểm tra nhấn nút số
    for (int i = 0; i < 10; ++i)
    {
        if (checkClickButton(x, y, digitButtons[i]))
        {
            // Cập nhật chuỗi thời gian
            inputText = inputText.substr(1) + to_string(i);
        }
    }
    // Kiểm tra nút Start
    if (checkClickButton(x, y, startButton))
    {
        // Chỉ cho phép kết thúc nhập liệu nếu inputText khác "000000"
        if (inputText != "000000")
        {
            done = true;
        }
    }
    // Kiểm tra nút Clear
    if (checkClickButton(x, y, clearButton))
    {
        // Đặt lại chuỗi thời gian
        inputText = "000000";
    }
}

// Hàm nhập thời gian
int inputTime(SDL_Window *window, SDL_Renderer *renderer)
{
    // Tài font chữ cỡ 100
    TTF_Font *font = TTF_OpenFont("data/digital.ttf", 100);

    // Tải thời gian mặc định từ file
    int defaultTime = loadTimeFromFile();
    if (defaultTime < 0)
    {
        defaultTime = 0;
    }
    // Chuyển đổi defaultTime thành chuỗi định dạng HHMMSS
    string inputText = convertDefaultTimeToString(defaultTime);

    // Biến kiểm soát vòng lặp nhập liệu
    bool done = false;
    // Biến sự kiện SDL
    SDL_Event e;

    // Vòng lặp nhập liệu
    while (!done)
    {
        // Nút Start và Clear nằm ngay dưới đồng hồ
        SDL_Rect startButton = {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 10, 80, 40};
        SDL_Rect clearButton = {WINDOW_WIDTH / 2 + 20, WINDOW_HEIGHT / 2 - 10, 80, 40};

        // Nút số từ 0 đến 4 ở hàng trên, từ 5 đến 9 ở hàng dưới
        vector<SDL_Rect> digitButtons(10);
        for (int i = 0; i < 5; ++i) // Hàng trên: 0-4
        {
            digitButtons[i] = {WINDOW_WIDTH / 2 - 240 + i * 100, WINDOW_HEIGHT / 2 + 50, 80, 40};
        }
        for (int i = 5; i < 10; ++i) // Hàng dưới: 5-9
        {
            digitButtons[i] = {WINDOW_WIDTH / 2 - 240 + (i - 5) * 100, WINDOW_HEIGHT / 2 + 110, 80, 40};
        }

        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0)
        {
            // Kiểm tra sự kiện thoát
            if (e.type == SDL_QUIT)
            {
                closeSDL(window, renderer);
                exit(0);
            }
            // Kiểm tra sự kiện cửa sổ
            else if (e.type == SDL_WINDOWEVENT)
            {
                handleWindowEvent(e, window);
            }
            // Kiểm tra sự kiện nhấn chuột
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                handleMouseEvent(e, inputText, done, digitButtons, startButton, clearButton);
            }
        }

        // Vẽ giao diện
        renderUI(renderer, font, inputText, startButton, clearButton, digitButtons);
    }

    // Chuyển đổi input thành thời gian
    int countdownTime = convertInputToTime(inputText);

    // Lưu thời gian vào file
    saveTimeToFile(countdownTime);

    // Close the font
    TTF_CloseFont(font);

    return countdownTime;
}
