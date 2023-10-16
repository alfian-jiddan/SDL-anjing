#include <SDL.h>
#include <cmath>
#include <SDL_net.h>


const float MAX_BALL_SPEED = 8.0;
const float FRICTION = 0.98;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;



//draw the course

//ball
const int BALL_SIZE = 20;
const int BALL_RADIUS = 10;
SDL_Rect ballRect = { SCREEN_WIDTH / 3.58 - BALL_SIZE / 2, SCREEN_HEIGHT / 1.23 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE };
SDL_Point ballVelocity = { 0, 0 }; // Initial velocity

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

bool isCollidingWithHole() {
    int ballCenterX = ballRect.x + BALL_SIZE / 2;
    int ballCenterY = ballRect.y + BALL_SIZE / 2;
    int holeCenterX = holeRect.x + HOLE_SIZE / 2;
    int holeCenterY = holeRect.y + HOLE_SIZE / 2;

    int distance = sqrt((holeCenterX - ballCenterX) * (holeCenterX - ballCenterX) + (holeCenterY - ballCenterY) * (holeCenterY - ballCenterY));

    return distance < BALL_RADIUS + HOLE_SIZE / 2;
}

bool levelComplete = false;

bool checkCollision(SDL_Rect a, SDL_Rect b) {
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


bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    gWindow = SDL_CreateWindow("Golf Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
}

void drawGolfCourse() {
    // Set the background color to green for the golf course
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x80, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);

    // Draw the hole
    SDL_SetRenderDrawColor(gRenderer, 0x8B, 0x45, 0x13, 0xFF);
    SDL_RenderFillRect(gRenderer, &holeRect);

    // Draw the ball
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(gRenderer, &ballRect);

    // Draw the wall
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall1Rect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall2Rect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall3Rect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall4Rect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall5Rect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall6Rect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall7Rect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &wall8Rect);

    if (isAiming) {
        // Draw a line indicating the aiming direction
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawLine(gRenderer, aimStart.x, aimStart.y, aimEnd.x, aimEnd.y);
    }

    SDL_RenderPresent(gRenderer);

}

void handleCollision() {
    if (ballRect.x < 0 || ballRect.x + ballRect.w > SCREEN_WIDTH) {
        ballVelocity.x = -ballVelocity.x;
    }

    if (ballRect.y < 0 || ballRect.y + ballRect.h > SCREEN_HEIGHT) {
        ballVelocity.y = -ballVelocity.y;
    }

    if (isCollidingWithHole()) {
        levelComplete = true;
    }
}

bool handleWallCollision() {
    bool collided = false;

    // Check collision with each wall
    if (checkCollision(ballRect, wall1Rect)) {
        // Handle collision with wall1Rect
        ballVelocity.x = -ballVelocity.x;
        collided = true;
    }

    if (checkCollision(ballRect, wall2Rect)) {
        // Handle collision with wall2Rect
        ballVelocity.y = -ballVelocity.y;
        collided = true;
    }

    // Repeat this process for other walls

    return collided;
}

void update() {
    // Apply friction to slow down the ball
    ballVelocity.x *= FRICTION;
    ballVelocity.y *= FRICTION;

    // If the velocity falls below a small threshold, set it to zero to stop the ball.
    const float MIN_VELOCITY = 0.1;
    if (fabs(ballVelocity.x) < MIN_VELOCITY) {
        ballVelocity.x = 0.0;
    }
    if (fabs(ballVelocity.y) < MIN_VELOCITY) {
        ballVelocity.y = 0.0;
    }

    // Update the ball's position
    ballRect.x += ballVelocity.x;
    ballRect.y += ballVelocity.y;

    // Check for collisions with walls
    if (checkCollision(ballRect, wall1Rect)) {
        // Handle collision with wall1Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    if (checkCollision(ballRect, wall2Rect)) {
        // Handle collision with wall2Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    if (checkCollision(ballRect, wall3Rect)) {
        // Handle collision with wall3Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    if (checkCollision(ballRect, wall4Rect)) {
        // Handle collision with wall4Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    if (checkCollision(ballRect, wall5Rect)) {
        // Handle collision with wall5Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    if (checkCollision(ballRect, wall6Rect)) {
        // Handle collision with wall6Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    if (checkCollision(ballRect, wall7Rect)) {
        // Handle collision with wall7Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    if (checkCollision(ballRect, wall8Rect)) {
        // Handle collision with wall8Rect
        ballVelocity.x = -ballVelocity.x;
        ballVelocity.y = -ballVelocity.y;
    }

    // Handle general collision with screen edges
    handleCollision();
}


void startAim(int x, int y) {
    aimStart.x = ballRect.x + ballRect.w / 2;
    aimStart.y = ballRect.y + ballRect.h / 2;
    isAiming = true;
}

void endAim(int x, int y) {
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
    ballVelocity.x = (direction.x / distance) * (distance / MAX_POWER * MAX_BALL_SPEED);
    ballVelocity.y = (direction.y / distance) * (distance / MAX_POWER * MAX_BALL_SPEED);
}

void renderLevelComplete() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x80, 0x00, 0xFF); // Set background color to green
    SDL_RenderClear(gRenderer);

    SDL_Color textColor = { 255, 255, 255 };
    const char* levelCompleteText = "Level Complete! Press 'R' to restart";

    SDL_Surface* textSurface = SDL_CreateRGBSurface(0, 400, 100, 32, 0, 0, 0, 0);
    SDL_FillRect(textSurface, NULL, SDL_MapRGB(textSurface->format, 0, 0, 0));
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    int textWidth = 400;
    int textHeight = 100;

    SDL_FreeSurface(textSurface);

    SDL_Rect textRect = { (SCREEN_WIDTH - textWidth) / 2, (SCREEN_HEIGHT - textHeight) / 2, textWidth, textHeight };

    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_RenderPresent(gRenderer);
}


int main(int argc, char* args[]) {
    if (!init()) {
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    const int TARGET_FPS = 60;
    const int MS_PER_FRAME = 1000 / TARGET_FPS;

    Uint32 prevTime = SDL_GetTicks();

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - prevTime;

        if (deltaTime < MS_PER_FRAME) {
            SDL_Delay(MS_PER_FRAME - deltaTime);
            deltaTime = MS_PER_FRAME;
        }

        prevTime = currentTime;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    startAim(e.button.x, e.button.y);
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    endAim(e.button.x, e.button.y);
                }
            }
        }

        if (levelComplete) {
            renderLevelComplete();
        }
        else {
            update();
            drawGolfCourse();
            SDL_RenderPresent(gRenderer);
        }
    }

    close();
    return 0;
}
