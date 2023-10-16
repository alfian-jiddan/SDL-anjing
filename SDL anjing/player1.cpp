#include <SDL.h>
#include <cmath>


const float MAX_BALL_SPEED = 8.0;
const float FRICTION = 0.98;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


SDL_Window* gWindowPlayer1 = NULL;
SDL_Renderer* gRendererPlayer1 = NULL;

const int BALL_SIZE = 20;
const int BALL_RADIUS = 10;
SDL_Rect ballRectPlayer1 = { SCREEN_WIDTH / 3.58 - BALL_SIZE / 2, SCREEN_HEIGHT / 1.23 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE };
SDL_Point ballVelocityPlayer1 = { 0, 0 };

//hole
const int HOLE_SIZE = 20;
SDL_Rect holeRect = { SCREEN_WIDTH / 1.23 - HOLE_SIZE / 2, SCREEN_HEIGHT / 8 - HOLE_SIZE / 2, HOLE_SIZE, HOLE_SIZE };

//wall
const int WALL1_WIDTH = 20;
const int WALL1_HEIGHT = 370;
SDL_Rect wall1Rect = { SCREEN_WIDTH / 1.1 - WALL1_WIDTH / 2, SCREEN_HEIGHT / 2.85 - WALL1_HEIGHT / 2, WALL1_WIDTH, WALL1_HEIGHT };

const int WALL2_WIDTH = 20;
const int WALL2_HEIGHT = 250;
SDL_Rect wall2Rect = { SCREEN_WIDTH / 1.4 - WALL2_WIDTH / 2, SCREEN_HEIGHT / 4 - WALL2_HEIGHT / 2, WALL2_WIDTH, WALL2_HEIGHT };

const int WALL3_WIDTH = 150;
const int WALL3_HEIGHT = 20;
SDL_Rect wall3Rect = { SCREEN_WIDTH / 1.23 - WALL3_WIDTH / 2, SCREEN_HEIGHT / 17 - WALL3_HEIGHT / 2, WALL3_WIDTH, WALL3_HEIGHT };

const int WALL4_WIDTH = 430;
const int WALL4_HEIGHT = 20;
SDL_Rect wall4Rect = { SCREEN_WIDTH / 2.18 - WALL4_WIDTH / 2, SCREEN_HEIGHT / 2.2 - WALL4_HEIGHT / 2, WALL4_WIDTH, WALL4_HEIGHT };

const int WALL5_WIDTH = 450;
const int WALL5_HEIGHT = 20;
SDL_Rect wall5Rect = { SCREEN_WIDTH / 1.56 - WALL5_WIDTH / 2, SCREEN_HEIGHT / 1.5 - WALL5_HEIGHT / 2, WALL5_WIDTH, WALL5_HEIGHT };

const int WALL6_WIDTH = 20;
const int WALL6_HEIGHT = 250;
SDL_Rect wall6Rect = { SCREEN_WIDTH / 4.95 - WALL6_WIDTH / 2, SCREEN_HEIGHT / 1.5 - WALL6_HEIGHT / 2, WALL6_WIDTH, WALL6_HEIGHT };

const int WALL7_WIDTH = 20;
const int WALL7_HEIGHT = 135;
SDL_Rect wall7Rect = { SCREEN_WIDTH / 2.8 - WALL7_WIDTH / 2, SCREEN_HEIGHT / 1.312 - WALL7_HEIGHT / 2, WALL7_WIDTH, WALL7_HEIGHT };

const int WALL8_WIDTH = 144;
const int WALL8_HEIGHT = 20;
SDL_Rect wall8Rect = { SCREEN_WIDTH / 3.58 - WALL8_WIDTH / 2, SCREEN_HEIGHT / 1.13 - WALL8_HEIGHT / 2, WALL8_WIDTH, WALL8_HEIGHT };

//aim
bool isAiming = false;
SDL_Point aimStart;
SDL_Point aimEnd;

bool isCollidingWithHolePlayer1() {
    int ballCenterX = ballRectPlayer1.x + BALL_SIZE / 2;
    int ballCenterY = ballRectPlayer1.y + BALL_SIZE / 2;
    int holeCenterX = holeRect.x + HOLE_SIZE / 2;
    int holeCenterY = holeRect.y + HOLE_SIZE / 2;

    int distance = sqrt((holeCenterX - ballCenterX) * (holeCenterX - ballCenterX) + (holeCenterY - ballCenterY) * (holeCenterY - ballCenterY));

    return distance < BALL_RADIUS + HOLE_SIZE / 2;
}

bool levelComplete = false;

bool checkCollisionPlayer1(SDL_Rect a, SDL_Rect b) {
    // Check for collision between two SDL_Rect objects
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

// Wall information
struct Wall {
    SDL_Rect rect;
    bool solid;
};

Wall walls[] = {
    {wall1Rect, true},
    {wall2Rect, true},
    {wall3Rect, true},
    {wall4Rect, true},
    {wall5Rect, true},
    {wall6Rect, true},
    {wall7Rect, true},
    {wall8Rect, true}
};

bool initPlayer1() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    gWindowPlayer1 = SDL_CreateWindow("Player 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindowPlayer1 == NULL) {
        return false;
    }

    gRendererPlayer1 = SDL_CreateRenderer(gWindowPlayer1, -1, SDL_RENDERER_ACCELERATED);
    if (gRendererPlayer1 == NULL) {
        return false;
    }

    // Initialize other player-specific resources if needed

    return true;
}

void closePlayer1(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawGolfCoursePlayer1(SDL_Renderer* renderer) {
    // Set the background color to green for the golf course
    SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    // Draw the hole
    SDL_SetRenderDrawColor(renderer, 0x8B, 0x45, 0x13, 0xFF);
    SDL_RenderFillRect(renderer, &holeRect);

    // Draw the ball for Player 1
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &ballRectPlayer1);

    // Draw the walls
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (const Wall& wall : walls) {
        SDL_RenderFillRect(renderer, &wall.rect);
    }

    if (isAiming) {
        // Draw a line indicating the aiming direction
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawLine(renderer, aimStart.x, aimStart.y, aimEnd.x, aimEnd.y);
    }

    SDL_RenderPresent(renderer);
}

void handleCollisionPlayer1() {
    if (ballRectPlayer1.x < 0 || ballRectPlayer1.x + ballRectPlayer1.w > SCREEN_WIDTH) {
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
    }

    if (ballRectPlayer1.y < 0 || ballRectPlayer1.y + ballRectPlayer1.h > SCREEN_HEIGHT) {
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (isCollidingWithHolePlayer1()) {
        levelComplete = true;
    }
}

bool handleWallCollisionPlayer1(const SDL_Rect& ballRect, SDL_Point& ballVelocity) {
    bool collided = false;

    // Check collision with each wall
    for (const Wall& wall : walls) {
        if (checkCollisionPlayer1(ballRect, wall.rect)) {
            // Handle collision with the wall
            if (wall.solid) {
                ballVelocity.x = -ballVelocity.x;
                ballVelocity.y = -ballVelocity.y;
                collided = true;
            }
        }
    }

    return collided;
}

void updatePlayer1() {
    // Apply friction to slow down the ball
    ballVelocityPlayer1.x *= FRICTION;
    ballVelocityPlayer1.y *= FRICTION;

    // If the velocity falls below a small threshold, set it to zero to stop the ball.
    const float MIN_VELOCITY = 0.1;
    if (fabs(ballVelocityPlayer1.x) < MIN_VELOCITY) {
        ballVelocityPlayer1.x = 0.0;
    }
    if (fabs(ballVelocityPlayer1.y) < MIN_VELOCITY) {
        ballVelocityPlayer1.y = 0.0;
    }

    // Update the ball's position
    ballRectPlayer1.x += ballVelocityPlayer1.x;
    ballRectPlayer1.y += ballVelocityPlayer1.y;


    // Check for collisions with walls
    if (checkCollisionPlayer1(ballRectPlayer1, wall1Rect)) {
        // Handle collision with wall1Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (checkCollisionPlayer1(ballRectPlayer1, wall2Rect)) {
        // Handle collision with wall2Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (checkCollisionPlayer1(ballRectPlayer1, wall3Rect)) {
        // Handle collision with wall3Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (checkCollisionPlayer1(ballRectPlayer1, wall4Rect)) {
        // Handle collision with wall4Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (checkCollisionPlayer1(ballRectPlayer1, wall5Rect)) {
        // Handle collision with wall5Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (checkCollisionPlayer1(ballRectPlayer1, wall6Rect)) {
        // Handle collision with wall6Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (checkCollisionPlayer1(ballRectPlayer1, wall7Rect)) {
        // Handle collision with wall7Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    if (checkCollisionPlayer1(ballRectPlayer1, wall8Rect)) {
        // Handle collision with wall8Rect
        ballVelocityPlayer1.x = -ballVelocityPlayer1.x;
        ballVelocityPlayer1.y = -ballVelocityPlayer1.y;
    }

    // Check for collisions with walls
    handleWallCollisionPlayer1(ballRectPlayer1, ballVelocityPlayer1);

    // Handle general collision with screen edges
    handleCollisionPlayer1();
}

void startAimPlayer1(int x, int y) {
    aimStart.x = ballRectPlayer1.x + ballRectPlayer1.w / 2;
    aimStart.y = ballRectPlayer1.y + ballRectPlayer1.h / 2;
    isAiming = true;
}

void endAimPlayer1(int x, int y) {
    aimEnd.x = x;
    aimEnd.y = y;
    isAiming = false;

    // Calculate the direction vector from aimStart to aimEnd
    SDL_Point direction;
    direction.x = aimEnd.x - aimStart.x;
    direction.y = aimEnd.y - aimStart.y;

    // Calculate the distance (power) based on the direction vector
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    // Define a maximum power to avoid excessive speeds
    const float MAX_POWER = 100.0;

    if (distance > MAX_POWER) {
        // Limit the power to the maximum
        distance = MAX_POWER;
    }

    // Calculate the velocity based on the direction and the limited power
    ballVelocityPlayer1.x = (direction.x / distance) * (distance / MAX_POWER * MAX_BALL_SPEED);
    ballVelocityPlayer1.y = (direction.y / distance) * (distance / MAX_POWER * MAX_BALL_SPEED);
}

void renderLevelCompletePlayer1(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF); // Set background color to green
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255 };
    const char* levelCompleteText = "Level Complete! Press 'R' to restart";

    SDL_Surface* textSurface = SDL_CreateRGBSurface(0, 400, 100, 32, 0, 0, 0, 0);
    SDL_FillRect(textSurface, NULL, SDL_MapRGB(textSurface->format, 0, 0, 0));
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = 400;
    int textHeight = 100;

    SDL_FreeSurface(textSurface);

    SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_RenderPresent(renderer);
}