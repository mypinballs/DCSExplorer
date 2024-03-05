#pragma once

#include <string>
#include <memory>
#include <SDL.h>

class SimpleSDLAudio
{
public:
    // Initialize the audio player with the given parameters
    SimpleSDLAudio(int sampleRate, int numChannels, int bufferSize_ms);

    // Set the idle callback function
    void SetIdleTask(void (*idleTask)(void*), void *context);

    // Initialize SDL audio - returns true on success, false on failure
    // Sets the error status string to a descriptive message on error
    bool InitSDLAudio();

    // Destruction
    ~SimpleSDLAudio();

    // Get the error status string
    const char *GetErrorDesc() { return errorDesc.c_str(); }

    // Write audio data to the hardware buffer
    void WriteAudioData(const INT16* data, DWORD numSamples);

    // Set the volume level
    void SetVolume(int vol);

    // Get the current volume setting
    int GetVolume() const { return volume; }

    // Get the sleep time
    double GetSleepTime() const { return sleepTime; }

private:
    // Create/Destroy audio buffers
    //bool CreateAudioBuffers();
    //void DestroyAudioBuffers();

    // Sample rate
    int sampleRate = 0;

    // Number of audio channels
    int numChannels = 0;

    // Buffer size in milliseconds
    int bufferSize_ms = 0;

    // Volume level
    int volume = 0;

    // Cumulative sleep time waiting for buffer write space, in seconds
    double sleepTime = 0.0;

    // Idle task callback and context
    void (*idleTask)(void*) = nullptr;
    void *idleTaskCtx = nullptr;

    // Error string, if applicable
    std::string errorDesc;
    void FormatErrorDesc(const char* msg, ...);

    // SDL audio variables
    SDL_AudioSpec audioSpec;
    SDL_AudioDeviceID audioDevice = 0; //main audio device

    // Audio buffer variables
    SDL_AudioCVT audioCvt;
    Uint8* audioBuffer = nullptr;
    uint32 streamBufferPos = 0;
    uint32 streamBufferSize = 0;
    uint32 streamBufferIn = 0;
};