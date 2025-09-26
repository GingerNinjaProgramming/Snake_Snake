#include <iostream>
#include <raylib.h>

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

class SnakeObject {
    public:
        int x;
        int y;
        int width;
        int height;
        Direction direction;

    SnakeObject(int x, int y, int width, int height, Direction dir){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->direction = dir;
    }
};

int main () {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const int GRID_CELL_SIZE = 20;

    int food_x = 600;
    int food_y = 300;

    SnakeObject snakeHead(0, 0, GRID_CELL_SIZE, GRID_CELL_SIZE, NONE);

    Direction direction = NONE;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(15);

    while (!WindowShouldClose()){

        if(IsKeyDown(KEY_RIGHT)) direction = RIGHT;
        else if(IsKeyDown(KEY_LEFT)) direction = LEFT;
        else if(IsKeyDown(KEY_UP)) direction = UP;
        else if(IsKeyDown(KEY_DOWN)) direction = DOWN;

        switch(direction){
            case RIGHT:
                snakeHead.x += 1 * GRID_CELL_SIZE;
                break;
            case LEFT:
                snakeHead.x -= 1 * GRID_CELL_SIZE;
                break;
            case UP:
                snakeHead.y -= 1 * GRID_CELL_SIZE;
                break;
            case DOWN:
                snakeHead.y += 1 * GRID_CELL_SIZE;
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
            DrawRectangle(food_x, food_y, GRID_CELL_SIZE, GRID_CELL_SIZE, RED);
        EndDrawing();
    }

    CloseWindow();
}
