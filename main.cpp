#include <iostream>
#include <raylib.h>

using namespace std;

class GameObject {
    public:
        int x;
        int y;
        int width;
        int height;

    GameObject(int x, int y, int width, int height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
};

int main () {

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    GameObject snakeHead(400, 300, 20, 20);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangle(snakeHead.x, snakeHead.y, snakeHead.width, snakeHead.height, GREEN);
        EndDrawing();
    }

    CloseWindow();
}
