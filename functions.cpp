#include "common.h"

//? Hàm khởi tạo các thành phần của SDL
void initSDL(SDL_Window *&window, SDL_Renderer *&renderer)
{
    // Khởi tạo SDL và kiểm tra lỗi
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        closeSDL(window, renderer);
        exit(-1);
    }

    // Tạo cửa sổ và kiểm tra lỗi
    window = SDL_CreateWindow("Countdown Timer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        cerr << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        closeSDL(window, renderer);
        exit(-1);
    }

    // Tạo renderer SDL với cửa sổ đã tạo, sử dụng driver mặc định (-1), và sử dụng tăng tốc phần cứng
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
        closeSDL(window, renderer);
        exit(-1);
    }

    // Khởi tạo SDL_ttf và kiểm tra lỗi
    if (TTF_Init() == -1)
    {
        cerr << "TTF could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        closeSDL(window, renderer);
        exit(-1);
    }
    // Mở font chữ và kiểm tra lỗi
    TTF_Font *font = TTF_OpenFont("data/digital.ttf", 100);
    if (font == nullptr)
    {
        cerr << "Failed to load font: " << TTF_GetError() << endl;
        closeSDL(window, renderer);
        exit(-1);
    }
    TTF_CloseFont(font); // Đóng font chữ

    // Khởi tạo SDL_mixer và kiểm tra lỗi
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        closeSDL(window, renderer);
        exit(-1);
    }
    // Mở âm thanh và kiểm tra lỗi
    Mix_Chunk *alarm = Mix_LoadWAV("data/alarm.mp3");
    if (alarm == nullptr)
    {
        cerr << "Failed to load alarm sound: " << Mix_GetError() << endl;
        closeSDL(window, renderer);
        exit(-1);
    }
    Mix_FreeChunk(alarm); // Giải phóng âm thanh

    // Bắt đầu nhận đầu vào văn bản từ người dùng
    SDL_StartTextInput();
}

//? Hàm xử lý sự kiện cửa sổ
void handleWindowEvent(SDL_Event &e, SDL_Window *window)
{
    // Kiểm tra nếu sự kiện là thay đổi kích thước cửa sổ
    if (e.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        // Lấy kích thước mới của cửa sổ
        int newWidth = e.window.data1;
        int newHeight = e.window.data2;

        // Đảm bảo kích thước cửa sổ tối thiểu
        newWidth = max(newWidth, 500);
        newHeight = max(newHeight, 400);

        // Đặt lại kích thước cửa sổ với kích thước mới
        SDL_SetWindowSize(window, newWidth, newHeight);

        // Cập nhật giá trị kích thước cửa sổ toàn cục
        WINDOW_WIDTH = newWidth;
        WINDOW_HEIGHT = newHeight;
    }
}

// Hàm giải phóng các tài nguyên của SDL
void closeSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    // Giải phóng renderer nếu nó đã được khởi tạo
    if (renderer)
        SDL_DestroyRenderer(renderer);

    // Giải phóng cửa sổ nếu nó đã được khởi tạo
    if (window)
        SDL_DestroyWindow(window);

    // Kết thúc SDL_mixer
    Mix_Quit();
    // Kết thúc SDL_ttf
    TTF_Quit();
    // Kết thúc SDL
    SDL_Quit();
}

// Lưu thời gian vào file
void saveTimeToFile(int countdownTime)
{
    ofstream outFile("data/config.txt");
    if (outFile.is_open())
    {
        // Ghi giá trị thời gian vào file
        outFile << countdownTime;
        // Đóng file sau khi ghi xong
        outFile.close();
    }
}

// Đọc thời gian đếm ngược từ file
int loadTimeFromFile()
{
    ifstream inFile("data/config.txt");
    int countdownTime = 0;
    if (inFile.is_open())
    {
        // Đọc giá trị thời gian từ file
        inFile >> countdownTime;
        // Đóng file sau khi đọc xong
        inFile.close();
    }
    // Trả về giá trị thời gian đếm ngược đã đọc
    return countdownTime;
}

//? Kiểm tra click button
bool checkClickButton(int x, int y, const SDL_Rect &button)
{
    // Kiểm tra tọa độ x và y có nằm trong vùng button hay không
    return (x >= button.x && x <= button.x + button.w &&
            y >= button.y && y <= button.y + button.h);
}
