#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <chrono>
#include "readmap.cpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int PLAYFIELD_LEFT_BOUND = SCREEN_WIDTH / 3;
const int PLAYFIELD_RIGHT_BOUND = (2 * SCREEN_WIDTH) / 3;

const int KEY_AMOUNT = 4;
const float SCROLL_SPEED = 1.0;

const int OBJECT_SIZE = (SCREEN_WIDTH / 3) / KEY_AMOUNT;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
Mix_Music *gMusic = NULL;

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

        void move(double delta) {
            y += SCROLL_SPEED * delta;
        }

        void render() {
            SDL_Rect noteRect = {x, y, OBJECT_SIZE, OBJECT_SIZE / 2};
            SDL_RenderFillRect(gRenderer, &noteRect);
            SDL_RenderDrawRect(gRenderer, &noteRect);
        }
    
    private:
        float x, y;
};

void renderPlayfield() {
    SDL_RenderDrawLine(gRenderer, PLAYFIELD_LEFT_BOUND, 0, PLAYFIELD_LEFT_BOUND, SCREEN_HEIGHT);
    SDL_RenderDrawLine(gRenderer, PLAYFIELD_RIGHT_BOUND, 0, PLAYFIELD_RIGHT_BOUND, SCREEN_HEIGHT);
    SDL_RenderDrawLine(gRenderer, PLAYFIELD_LEFT_BOUND, SCREEN_HEIGHT - (SCREEN_HEIGHT / 8), PLAYFIELD_RIGHT_BOUND, SCREEN_HEIGHT - (SCREEN_HEIGHT / 8));
}

void init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("mania conversion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void close() {
    Mix_FreeMusic(gMusic);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    gMusic = NULL;
    Mix_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    init();

    bool quit = false;
    SDL_Event event;

    std::vector<hitObject> hitObjects;
    std::string fileLocation = "";
    hitObjects = getHitObjects(fileLocation);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    int currentIndex = -1;

    std::vector<Note> notes;
    std::vector<Note>::iterator it = notes.begin();

    gMusic = Mix_LoadMUS("");
    Mix_VolumeMusic(5);
    Mix_PlayMusic(gMusic, -1);

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double delta = 0;

    while(!quit) {
        last = now;
        now = SDL_GetPerformanceCounter();
        delta = ((now-last) * 1000 / (double)SDL_GetPerformanceFrequency());
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
            if(hitObjects[currentIndex + 1].time - ((SCREEN_HEIGHT - (SCREEN_HEIGHT / 8) - (-OBJECT_SIZE)) / (SCROLL_SPEED)) <= elapsedTime) {
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
            notes[i].move(delta);
            notes[i].render();
        }

        SDL_RenderPresent(gRenderer);
    }

    close();

}