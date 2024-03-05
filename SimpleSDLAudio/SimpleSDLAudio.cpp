#include <SimpleSDLAudio.h>

//----------------
// create instance
//----------------

SimpleSDLAudio::SimpleSDLAudio(int sampleRate, int nChannels, int bufSize_ms) :
	sampleRate(sampleRate), nChannels(nChannels), bufSize_ms(bufSize_ms)
{
}

// Set the idle task
void SimpleSDLAudio::SetIdleTask(void (*idleTask)(void *), void *context)
{
	this->idleTask = idleTask;
	this->idleTaskCtx = context;
}

// Destructor
~SimpleSDLAudio()
{
    // Perform cleanup actions here
    SDL_PauseAudioDevice(audioDevice, 1);  // Pause audio before closing audio device
    SDL_CloseAudioDevice(audioDevice);     // Close the audio device
    // Additional cleanup if needed
    
    // Close the SDL audio device
    SDL_CloseAudio();
    
    // Clean up any allocated resources such as stream buffer

    // Quit SDL audio subsystem
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

}


//----------------
//initialise
//----------------

bool SimpleSDLAudio::InitSDLAudio()
{
    // Initialize SDL audio subsystem
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "SDL failed to initialize audio: %s\n", SDL_GetError());
        return false;
    }

    // Create an SDL audio spec with the desired audio format parameters
    //SDL_AudioSpec sdlAudioSpec;
    
    sdlAudioSpec.freq = sampleRate; //44100
    sdlAudioSpec.format = AUDIO_S16SYS; // 16-bit signed, system byte order
    sdlAudioSpec.channels = numChannels; //2
    sdlAudioSpec.samples = 1024; // Specify the size of a sample buffer - adjust as needed
    sdlAudioSpec.callback = NULL; // Set the callback function for audio processing, if needed
    sdlAudioSpec.userdata = NULL; // Set custom user data if needed
    sdlAudioSpec.callback = audioCallback;

    // Open the audio device
    if (SDL_OpenAudio(&sdlAudioSpec, NULL) < 0)
    {
        fprintf(stderr, "SDL failed to open audio: %s\n", SDL_GetError());
        return false;
    }

    // Start SDL audio playback
    SDL_PauseAudio(0);

    return true;
}


//auto on sdl?
// //----------------
// //create buffers
// //----------------

// bool SimpleSDLAudio::CreateSDLBuffers()
// {
//     // Initialize SDL audio subsystem
//     if (SDL_Init(SDL_INIT_AUDIO) < 0)
//     {
//         fprintf(stderr, "SDL failed to initialize audio: %s\n", SDL_GetError());
//         return false;
//     }

//     // Open the audio device with the desired SDL audio specification
//     if (SDL_OpenAudio(&sdlAudioSpec, NULL) < 0)
//     {
//         fprintf(stderr, "SDL failed to open audio: %s\n", SDL_GetError());
//         return false;
//     }

//     // Allocate memory for stream buffer or any other necessary initialization for playback

//     return true;
// }

//----------------
//clean up
//----------------

// void SimpleSDLAudio::DestroySDLBuffers()
// {
//     // Close the SDL audio device
//     SDL_CloseAudio();
    
//     // Clean up any allocated resources such as stream buffer

//     // Quit SDL audio subsystem
//     SDL_QuitSubSystem(SDL_INIT_AUDIO);
// }

//----------------
//write audio data
//----------------

void SimpleSDLAudio::WriteAudioData(const INT16* data, DWORD nSamples)
{
    // Wait for the buffer to have enough space to write the data
    while (audioBufferPos + nSamples > audioBufferSize)
    {
        if (SDL_GetAudioDeviceStatus(audioDevice) == SDL_AUDIO_PLAYING)
        {
            // If the audio device is playing, wait for some time
            SDL_Delay(1);
            sleepTime += 0.001; // Accumulate sleep time
        }
        else
        {
            // Audio device is not playing, start playback
            SDL_PauseAudioDevice(audioDevice, 0);
        }
    }

    // Copy the audio data to the SDL audio buffer
    SDL_memcpy(audioBuffer + audioBufferPos, data, nSamples * sizeof(INT16));

    // Update the buffer position
    audioBufferPos += nSamples;

    // Check for buffer overflow
    if (audioBufferPos >= audioBufferSize)
    {
        audioBufferPos = 0;
    }
}

//----------------
//set volume
//----------------

void SimpleSDLAudio::SetVolume(int vol)
{
    // limit to -100dB to 0dB
    if (vol > 0)
        vol = 0;
    else if (vol < -100)
        vol = -100;

    // Remember the volume internally
    volume = vol;

    // Convert dB relative to reference level to SDL volume scale
    // SDL_MIX_MAXVOLUME represents the maximum volume (usually 128)
    double linearVolume = SDL_MIX_MAXVOLUME * pow(10, (double)vol / 20.0);  // Convert dB to linear scale

    // Set the volume for the audio device
    SDL_SetVolume(audioDevice, (int)linearVolume);
}



void SimpleSDLAudio::FormatErrorDesc(const char *msg, ...)
{
	// format the message into a temp buffer
	char buf[512];
	va_list va;
	va_start(va, msg);
	sprintf_s(buf, msg, va);
	va_end(msg);

	// store it
	errorDesc = buf;
}
