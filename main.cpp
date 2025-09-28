#include <iostream>
#include <raylib.h>
#include <queue>
#include "main.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int GRID_CELL_SIZE = 20;
const int MOVE_FRAME_DELAY = 10;

class SnakeObject {
    public:
        int x;
        int y;
        int width = GRID_CELL_SIZE;
        int height = GRID_CELL_SIZE;

    SnakeObject(int x, int y){
        this->x = x;
        this->y = y;
    }

    bool IsCollidingWith(int otherX, int otherY){
        return x == otherX && y == otherY;
    }
};

//The difficulty enum values represent the number of frames between each snake movement
enum Difficulty { EASY = 10, MEDIUM = 5, HARD = 1};

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };
enum GameState { START, PLAYING, GAMEOVER, PAUSED};

queue<SnakeObject*> snake;

Direction direction = NONE;
GameState gameState = PLAYING;

bool inputBuffing = true; 
bool gameStarted = false; // To initialize the snake only once on first frame of PLAYING state
bool shouldMove = false; 
bool shouLdClose = false;

int food_x = 600;
int food_y = 300;

int score = 0;
int frameCounter = 0;

SnakeObject GetSnakeHead(){
    return *snake.front();
}

void CreateSnake(queue<SnakeObject*> _snake){
    bool isHead = true;

    while(!_snake.empty()){
        SnakeObject* segment = _snake.front();
        _snake.pop();

        Color color = isHead ? BLUE : (_snake.size() % 2 == 0 ? DARKGREEN : GREEN);

        DrawRectangle(segment->x, segment->y, segment->width, segment->height, color);
        isHead = false;
    }
} 

void UpdateSnake(bool shouldMove){
    if(!shouldMove) return;

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

bool HandleDeathCollisions(queue<SnakeObject*> snake) {
    SnakeObject* head = snake.front();
    snake.pop();

    if (head->x < 0 || head->x >= SCREEN_WIDTH || head->y < 0 || head->y >= SCREEN_HEIGHT) {
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

void HandlePlayingState() {
    // Implement the logic for the PLAYING state here
    if(snake.empty()){
        SnakeObject head(0,0);
        snake.push(&head);
    }

    int previousSnakeBackX = snake.back()->x;
    int previousSnakeBackY = snake.back()->y;

    shouldMove = frameCounter == MOVE_FRAME_DELAY;

    if(shouldMove) frameCounter = 0;
    else frameCounter++;

    HandleMovement();

    if(GetSnakeHead().IsCollidingWith(food_x, food_y)){
        SnakeObject* newSegment = new SnakeObject(previousSnakeBackX, previousSnakeBackY);
        snake.push(newSegment);

        food_x = GetRandomValue(0, (SCREEN_WIDTH / GRID_CELL_SIZE) - 1) * GRID_CELL_SIZE;
        food_y = GetRandomValue(0, (SCREEN_HEIGHT / GRID_CELL_SIZE) - 1) * GRID_CELL_SIZE;
    }

    if(HandleDeathCollisions(snake)){
        cout << "Game Over!" << endl;
        gameState = GAMEOVER;
        return;
    }

    score = snake.size() - 1;

    BeginDrawing();
        ClearBackground(BLACK);
        CreateSnake(snake);
        DrawRectangle(food_x, food_y, GRID_CELL_SIZE, GRID_CELL_SIZE, RED);
        DrawText(("Score: " + to_string(score)).c_str(), 10, 10, 20, WHITE);
    EndDrawing();
}

void HandleMovement()
{
    if (inputBuffing)
    {
        queue<Direction> inputBuffer;

        if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && direction != LEFT)
            inputBuffer.push(RIGHT);
        else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && direction != RIGHT)
            inputBuffer.push(LEFT);
        else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && direction != DOWN)
            inputBuffer.push(UP);
        else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && direction != UP)
            inputBuffer.push(DOWN);

        if (!inputBuffer.empty())
        {
            direction = inputBuffer.front();
            inputBuffer.pop();

            UpdateSnake(shouldMove);
        }
        else
        {
            UpdateSnake(shouldMove);
        }
    }
    else
    {

        if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && direction != LEFT)
            direction = RIGHT;
        else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && direction != RIGHT)
            direction = LEFT;
        else if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && direction != DOWN)
            direction = UP;
        else if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && direction != UP)
            direction = DOWN;

        UpdateSnake(shouldMove);
    }
}
int main () {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
    SetTargetFPS(60);

    gameStarted = true;
    while (!shouLdClose){
        switch(gameState){
            case START:
                break;
            case PLAYING:
                HandlePlayingState();
                break;
            case GAMEOVER:
                if(IsKeyDown(KEY_R)){
                    // Reset game state
                    ResetValues();
                }

                BeginDrawing();
                    ClearBackground(BLACK);
                    DrawText("Game Over! Press R to Restart or ESC to Exit", 100, SCREEN_HEIGHT / 2 - 20, 20, WHITE);
                    DrawText(("Final Score: " + to_string(score)).c_str(), 100, SCREEN_HEIGHT / 2 + 20, 20, WHITE);
                EndDrawing();
                break;
            case PAUSED:
                break;
        }

        if(WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) {
            shouLdClose = true;
            continue;
        }
    }

    CloseWindow();
}

void ResetValues()
{
    while (!snake.empty())
    {
        snake.pop();
    }

    gameStarted = false;
    direction = NONE;
    score = 0;
    frameCounter = 0;
    food_x = 600;
    food_y = 300;
    gameState = PLAYING;
}
