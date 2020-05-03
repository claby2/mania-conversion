#include <SDL2/SDL.h>
#include <vector>
#include <chrono>
#include "readmap.cpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int PLAYFIELD_LEFT_BOUND = SCREEN_WIDTH / 3;
const int PLAYFIELD_RIGHT_BOUND = (2 * SCREEN_WIDTH) / 3;

const int KEY_AMOUNT = 4;
const float SCROLL_SPEED = 0.1;

const int OBJECT_SIZE = (SCREEN_WIDTH / 3) / KEY_AMOUNT;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

class Note {
    public:
        Note() {
            y = -OBJECT_SIZE;
        }

        void setXPosition(float standardX) {
            int positionIndex = (standardX - 1) / (SCREEN_WIDTH / KEY_AMOUNT);
            x = PLAYFIELD_LEFT_BOUND + (positionIndex * OBJECT_SIZE);
        }

        bool hitBottom() {
            return y > SCREEN_HEIGHT;
        }

        void move() {
            y += SCROLL_SPEED;
        }

        void render() {
            SDL_Rect noteRect = {x, y, OBJECT_SIZE, OBJECT_SIZE / 2};
            SDL_RenderDrawRect(gRenderer, &noteRect);
        }
    
    private:
        float x, y;
};

void renderPlayfield() {
    SDL_RenderDrawLine(gRenderer, PLAYFIELD_LEFT_BOUND, 0, PLAYFIELD_LEFT_BOUND, SCREEN_HEIGHT);
    SDL_RenderDrawLine(gRenderer, PLAYFIELD_RIGHT_BOUND, 0, PLAYFIELD_RIGHT_BOUND, SCREEN_HEIGHT);
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("osu! Map Reader", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
}

int main(int argc, char* args[]) {
    init();

    bool quit = false;
    SDL_Event event;

    std::vector<hitObject> hitObjects;
    std::string fileLocation = "maps/Vickeblanka - Black Rover (TV Size) (Sotarks) [Extreme].osu";
    hitObjects = getHitObjects(fileLocation);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int currentIndex = -1;

    std::vector<Note> notes;
    std::vector<Note>::iterator it = notes.begin();

    while(!quit) {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);

        if(currentIndex + 1 <= hitObjects.size()) {
            if(hitObjects[currentIndex + 1].time <= elapsedTime) {
                hitObject object = hitObjects[currentIndex + 1];
                currentIndex++;
                Note note;
                note.setXPosition(object.x);
                notes.push_back(note);
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        renderPlayfield();

        for(int i = 0; i < notes.size(); i++) {
            if(notes[i].hitBottom()) {
                notes.erase(notes.begin() + i);
                continue;
            }
            notes[i].move();
            notes[i].render();
        }

        SDL_RenderPresent(gRenderer);
    }

    close();

}