#include <iostream>

#include "sre/SDLRenderer.hpp"
#include "SDL_mixer.h"


using namespace sre;
using namespace std;

// Based on http://lazyfoo.net/SDL_tutorials/lesson11/index.php
// Including the wave files
class MixerExample{
public:
    MixerExample(){
        r.init();
        r.setWindowTitle("SDL2_mixer example");

        // Initialize SDL_mixer. Lazy Foo's description: http://lazyfoo.net/SDL_tutorials/lesson11/index.php
        // Mix_OpenAudio()'s first argument is the sound frequency we use, and in this case it's 22050 which is what's
        // recommended. The second argument is the sound format used which we set to the default. The third argument is how
        // many channels we plan to use. We set it to 2 so we have stereo sound, if it was set to one we'd have mono sound.
        // The last argument is the sample size, which is set to 4096.
        if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
        {
            cout << "Cannot initialize audio output"<< endl;
            return;
        }

        //Load the music
        music = Mix_LoadMUS( "project_data/beat.wav" );

        //If there was a problem loading the music
        if( music == nullptr)
        {
            cout << "Cannot load music"<< endl;
            return;
        }

        //Load the sound effects
        scratch = Mix_LoadWAV( "project_data/scratch.wav" );
        high = Mix_LoadWAV( "project_data/high.wav" );
        med = Mix_LoadWAV( "project_data/medium.wav" );
        low = Mix_LoadWAV( "project_data/low.wav" );

        if( ( scratch == nullptr ) || ( high == nullptr ) || ( med == nullptr ) || ( low == nullptr ) )
        {
            cout << "there was a problem loading the sound effects"<< endl;
            return;
        }

        r.frameRender = [&](){
            RenderPass rp = RenderPass::create().build();

            static bool musicPlaying = false;
            if (ImGui::Checkbox("Play music",&musicPlaying)){
                if( Mix_PlayingMusic() == 0 )
                {
                    // Start the music
                    Mix_PlayMusic( music, -1 );
                } else {
                    if (musicPlaying){
                        Mix_ResumeMusic();
                    } else {
                        Mix_PauseMusic();
                    }
                }
            }
            if (ImGui::Button("Play scratch")){
                Mix_PlayChannel( -1, scratch, 0 );
            }
            if (ImGui::Button("Play high")){
                Mix_PlayChannel( -1, high, 0 );
            }
            if (ImGui::Button("Play medium")){
                Mix_PlayChannel( -1, med, 0 );
            }
            if (ImGui::Button("Play low")){
                Mix_PlayChannel( -1, low, 0 );
            }

        };
        r.startEventLoop();
    }
private:
    SDLRenderer r;
    Mix_Music *music;
    Mix_Chunk *scratch;
    Mix_Chunk *high;
    Mix_Chunk *med;
    Mix_Chunk *low;
};

int main() {
    std::cout << "https://github.com/mortennobel/SimpleRenderEngineProject" << std::endl;
    new MixerExample();

    return 0;
}