#include <iostream>
#include <raylib.h>
#include <queue>

using namespace std;

const int GRID_CELL_SIZE = 20;

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

class SnakeObject {
    public:
        int x;
        int y;
        int width;
        int height;

    SnakeObject(int x, int y, int width, int height){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    bool IsCollidingWith(int otherX, int otherY){
        return x == otherX && y == otherY;
    }
};

void CreateSnake(queue<SnakeObject*> snake){
    while(!snake.empty()){
        SnakeObject* segment = snake.front();
        snake.pop();
        DrawRectangle(segment->x, segment->y, segment->width, segment->height, GREEN);
    }
} 

void UpdateSnake(Direction direction,queue<SnakeObject*> &snake){
    queue<SnakeObject*> tempSnake;

    SnakeObject* head = snake.front();
    snake.pop();

    int prevX = head->x;
    int prevY = head->y;

    switch(direction){
        case UP:
            head->y -= 1 * GRID_CELL_SIZE;
            break;
        case DOWN:
            head->y += 1 * GRID_CELL_SIZE;
            break;
        case LEFT:
            head->x -= 1 * GRID_CELL_SIZE;
            break;
        case RIGHT:
            head->x += 1 * GRID_CELL_SIZE;
            break;
        case NONE:
            break;
    }

    tempSnake.push(head);

    while(!snake.empty()){
        SnakeObject* segment = snake.front();
        snake.pop();

        int newLocX = prevX;
        int newLocY = prevY;

        prevX = segment->x;
        prevY = segment->y;

        segment->x = newLocX;
        segment->y = newLocY;

        tempSnake.push(segment);
    }
    snake = tempSnake;
}

bool HandleDeathCollisions(queue<SnakeObject*> snake, int screenWidth, int screenHeight) {
    SnakeObject* head = snake.front();
    snake.pop();

    if (head->x < 0 || head->x >= screenWidth || head->y < 0 || head->y >= screenHeight) {
        return true; // Collision with wall
    }

    while(!snake.empty()) {
        SnakeObject* segment = snake.front();
        snake.pop();
        if (head->IsCollidingWith(segment->x, segment->y)) {
            return true; // Collision with self
        }
    }

    return false;
}

int main () {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    int food_x = 600;
    int food_y = 300;

    queue<SnakeObject*> snake;

    SnakeObject snakeHead(0, 0, GRID_CELL_SIZE, GRID_CELL_SIZE);
    snake.push(&snakeHead);

    Direction direction = NONE;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(15);

    while (!WindowShouldClose()){
        int previousSnakeBackX = snake.back()->x;
        int previousSnakeBackY = snake.back()->y;

        if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) direction = RIGHT;
        else if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) direction = LEFT;
        else if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) direction = UP;
        else if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) direction = DOWN;

        UpdateSnake(direction,snake);

        if(snakeHead.IsCollidingWith(food_x, food_y)){
            SnakeObject* newSegment = new SnakeObject(previousSnakeBackX, previousSnakeBackY, GRID_CELL_SIZE, GRID_CELL_SIZE);
            snake.push(newSegment);

            food_x = GetRandomValue(0, (SCREEN_WIDTH / GRID_CELL_SIZE) - 1) * GRID_CELL_SIZE;
            food_y = GetRandomValue(0, (SCREEN_HEIGHT / GRID_CELL_SIZE) - 1) * GRID_CELL_SIZE;
        }

        if(HandleDeathCollisions(snake, SCREEN_WIDTH, SCREEN_HEIGHT)){
            cout << "Game Over!" << endl;
            break;
        }

        cout << snake.size() << "||" << previousSnakeBackX << "||" << snake.back()->x << endl;

        BeginDrawing();
            ClearBackground(BLACK);
            CreateSnake(snake);
            DrawRectangle(food_x, food_y, GRID_CELL_SIZE, GRID_CELL_SIZE, RED);
        EndDrawing();
    }

    CloseWindow();
}

