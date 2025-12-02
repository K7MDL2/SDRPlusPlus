#pragma once
#include <imgui/imgui.h>
#include <fftw3.h>
#include <dsp/types.h>
#include <dsp/stream.h>
#include <signal_path/vfo_manager.h>
#include <string>
#include <utils/event.h>
#include <mutex>
#include <gui/tuner.h>

#define WINDOW_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground

class MainWindow {
public:
    void init();
    void draw();
    void setViewBandwidthSlider(float bandwidth);
    bool sdrIsRunning();
    void setFirstMenuRender();

    static float* acquireFFTBuffer(void* ctx);
    static void releaseFFTBuffer(void* ctx);

    // TODO: Replace with it's own class
    void setVFO(double freq);

    void setPlayState(bool _playing);
    bool isPlaying();

    bool lockWaterfallControls = false;
    bool playButtonLocked = false;

    Event<bool> onPlayStateChange;

private:
    static void vfoAddedHandler(VFOManager::VFO* vfo, void* ctx);
	int open_encoder(int device);
    int read_encoder(int device);
    
    // FFT Variables
    int fftSize = 8192 * 8;
    std::mutex fft_mtx;
    fftwf_complex *fft_in, *fft_out;
    fftwf_plan fftwPlan;

    // GUI Variables
    bool firstMenuRender = true;
    bool startedWithMenuClosed = false;
    float fftMin = -70.0;
    float fftMax = 0.0;
    float bw = 8000000;
    bool playing = false;
    bool showCredits = false;
    std::string audioStreamName = "";
    std::string sourceName = "";
    int menuWidth = 300;
    bool grabbingMenu = false;
    int newWidth = 300;
    int fftHeight = 300;
    bool showMenu = true;
    int tuningMode = tuner::TUNER_MODE_NORMAL;
    dsp::stream<dsp::complex_t> dummyStream;
    bool demoWindow = false;
    int selectedWindow = 0;

    bool initComplete = false;
    bool autostart = false;

	#define __RASPI__  // eventually make this a CMake -Doption

	#define E_VFO 		1
	#define E_FFT_MIN 	2
	#define E_FFT_MAX 	3

	#ifdef __RASPI__   // for Pi only - customize if pins are changed	
	// On a RPi5 add this line to the /boot/firmware/config.txt to enable rotary encoder in the file system
	// On a RPi4 add this line to the /boot/config.txt to enable rotary encoder in the file system
	// dtoverlay=rotary-encoder,pin_a=18,pin_b=17,relative_axis=1,steps-per-period=1
	// Do the same for additional encoders with their pin numbers
		const char* ENC_VFO = "/dev/input/by-path/platform-rotary@12-event";  // for PinA=GPIO18
		// rotary@12 is a rotary encoder with a Pin A gpio address of hex 12, or IO18 decimal.
		const char* ENC_FFT_MAX = "/dev/input/by-path/platform-rotary@10-event"; //  for PinA=GPIO16
		const char* ENC_FFT_MIN = "/dev/input/by-path/platform-rotary@14-event"; //  for PinA=GPIO20
	#endif

    EventHandler<VFOManager::VFO*> vfoCreatedHandler;
};