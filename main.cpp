#include <iostream>
#include <raylib.h>

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

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

    Direction direction = NONE;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);

    while (!WindowShouldClose()){

        if(IsKeyDown(KEY_RIGHT)) direction = RIGHT;
        else if(IsKeyDown(KEY_LEFT)) direction = LEFT;
        else if(IsKeyDown(KEY_UP)) direction = UP;
        else if(IsKeyDown(KEY_DOWN)) direction = DOWN;

        switch(direction){
            case RIGHT:
                snakeHead.x += 5;
                break;
            case LEFT:
                snakeHead.x -= 5;
                break;
            case UP:
                snakeHead.y -= 5;
                break;
            case DOWN:
                snakeHead.y += 5;
                break;
            case NONE:
                // Do nothing
                break;
            default:
                break;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangle(snakeHead.x, snakeHead.y, snakeHead.width, snakeHead.height, GREEN);
        EndDrawing();
    }

    CloseWindow();
}
