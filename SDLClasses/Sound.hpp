#ifndef _SDLCLASSES_SOUND
#define _SDLCLASSES_SOUND

#include <iostream>
#include <map>

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_audio.h>

#include <source_location>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/efx.h>
#include <AL/xram.h>

#include "./LuauClasses/Vector3.hpp"

#include "./LuauClasses/task.hpp"

#include <memory>
#include <utility>

static ALCdevice* AudioDevice;
static ALCcontext* ALContext;

namespace SDLClasses::Sound {

	constexpr int AudioFrequency = 44100;

	class ALAudioBuffer {
	public:
		ALAudioBuffer() noexcept {
			
			alGetError(); // clear errors

			alGenBuffers(1, &this->ALBuffer);
			if (const ALenum ALError = alGetError(); ALError != AL_NO_ERROR) {
				std::cerr << ":ERROR: Failed to generate an OpenAL buffer! AL Error Code: " << std::hex << ALError
					<< "\nLine: " << __LINE__ << " in header " << __FILE__ << std::endl;
				
				return;
			}
		};

		inline ~ALAudioBuffer() {
			//std::cout << "deleted ALAUDIODECONSTRUCT ALID: " << this->ALBuffer << '\n';

			alDeleteBuffers(1, &this->ALBuffer);
		};

		ALuint ALBuffer;
	};

	//std::map<const char*, std::shared_ptr<ALAudioBuffer>>* LoadedAudioBuffers;
	std::map<const char*, ALAudioBuffer*>* LoadedAudioBuffers;

	std::shared_ptr<ALAudioBuffer> LoadALAudioBuffer(const char* AudioPath) {
		
		if (!LoadedAudioBuffers->contains(AudioPath)) {

			//ALAudioBuffer* AudioBuffer = new ALAudioBuffer();
			std::shared_ptr<ALAudioBuffer> AudioBuffer = std::make_shared<ALAudioBuffer>();

			Mix_Chunk* LoadedAudio = Mix_LoadWAV(AudioPath);
			alBufferData(AudioBuffer->ALBuffer, AL_FORMAT_STEREO_FLOAT32, LoadedAudio->abuf, LoadedAudio->alen, AudioFrequency);
			Mix_FreeChunk(LoadedAudio);

			LoadedAudioBuffers->insert({ AudioPath, AudioBuffer.get() });

			return AudioBuffer;
		}

		//return std::make_shared<ALAudioBuffer>(LoadedAudioBuffers[AudioPath]);
		
		return std::move(std::shared_ptr<ALAudioBuffer>((*LoadedAudioBuffers)[AudioPath])); //std::make_shared<ALAudioBuffer>((*LoadedAudioBuffers)[AudioPath]));
	}

	class ALSource {
	public:
		ALSource() {

			alGetError(); // clear errors

			alGenSources(1, &this->ALBuffer);
			if (const ALenum error = alGetError(); error != AL_NO_ERROR) {
				//DisplayALError("alGenSources 1 : ", error);
			}
		}

		inline ~ALSource() {
			alDeleteSources(1, &this->ALBuffer);
		}

		ALuint ALBuffer;

		void AttachAudioBuffer(const std::shared_ptr<ALAudioBuffer> AudioBuffer) const {

			alGetError(); // clear errors

			alSourcei(this->ALBuffer, AL_BUFFER, AudioBuffer->ALBuffer);
			if (const ALenum error = alGetError(); error != AL_NO_ERROR) {
				//DisplayALError("alSourcei AL_BUFFER 0 : ", error);
			}
		}

		inline void Play() const {
			alSourcePlay(this->ALBuffer);
		}

		inline void IntSetProperty(ALenum Property, ALint SetTo) const {
			alSourcei(this->ALBuffer, Property, SetTo);
		}
		inline void FloatSetProperty(ALenum Property, ALfloat SetTo) const {
			alSourcef(this->ALBuffer, Property, SetTo);
		}
		inline void FloatArraySetProperty(ALenum Property, const ALfloat Values[]) const {
			alSourcefv(this->ALBuffer, Property, Values);
		}

		inline void Vec3SetProperty(ALenum Property, const LuauClasses::Vector3& SetTo) const {
			alSource3f(this->ALBuffer, Property, SetTo.X, SetTo.Y, SetTo.Z);
		}
	};

	class ALSource3D : public ALSource {
	public:
		inline ALSource3D() {
			this->Position = LuauClasses::Vector3(0.f);
		}

		LuauClasses::Vector3 Position;

		inline void Update() {
			this->Vec3SetProperty(AL_POSITION, this->Position);
		}
	};

	namespace Listener {

		LuauClasses::Vector3* Position = nullptr;// = LuauClasses::Vector3(0.f);
		//LuauClasses::Vector3 Orientation;		//ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
		LuauClasses::Vector3* Velocity = nullptr;// = LuauClasses::Vector3(0.f);

		inline void Update() {

			alListenerfv(AL_POSITION, (ALfloat*)Position);
			if (const ALenum error = alGetError(); error != AL_NO_ERROR) {
				//DisplayALError("alListenerfv POSITION : ", error);
				return;
			}

			alListenerfv(AL_VELOCITY, (ALfloat*)Velocity);
			if (const ALenum error = alGetError(); error != AL_NO_ERROR) {
				//DisplayALError("alListenerfv VELOCITY : ", error);
				return;
			}

			/*
			// Orientation ...
			alListenerfv(AL_ORIENTATION, listenerOri);
			if (const ALenum error = alGetError(); error != AL_NO_ERROR) {
				//DisplayALError("alListenerfv ORIENTATION : ", error);
				return;*/
		}
	}
	

	class ALSoundObject {
	public:

		inline ALSoundObject() {}
		inline ALSoundObject(const char* AudioPath) {

			this->AudioBuffer = LoadALAudioBuffer(AudioPath);
			this->Source.AttachAudioBuffer(this->AudioBuffer);
		}

		std::shared_ptr<ALAudioBuffer> AudioBuffer;
		ALSource Source;

		inline void Play() const {
			this->Source.Play();
		}
	};

	class ALAttached3DSoundObject : public ALSoundObject {
	public:
		ALAttached3DSoundObject(const char* AudioPath, const LuauClasses::Vector3* Position = nullptr) {
			this->AudioBuffer = LoadALAudioBuffer(AudioPath);
			this->Position = Position;

			this->Source.AttachAudioBuffer(this->AudioBuffer);
		}

		const LuauClasses::Vector3* Position;

		inline void Update() {
			this->Source.Vec3SetProperty(AL_POSITION, *this->Position);
		}
	};

	class AL3DSoundObject : public ALSoundObject {
	public:
		AL3DSoundObject(const char* AudioPath, const LuauClasses::Vector3& Position) {
			this->AudioBuffer = LoadALAudioBuffer(AudioPath);
			this->Position = Position;

			this->Source.AttachAudioBuffer(this->AudioBuffer);
		}

		AL3DSoundObject(const LuauClasses::Vector3& Position) {
			this->Position = Position;
			this->Source.AttachAudioBuffer(this->AudioBuffer);
		}

		AL3DSoundObject(const char* AudioPath) {
			this->AudioBuffer = LoadALAudioBuffer(AudioPath);
			this->Position = LuauClasses::Vector3(0.f);

			this->Source.AttachAudioBuffer(this->AudioBuffer);
		}

		AL3DSoundObject() {
			this->Position = LuauClasses::Vector3(0.f);
			this->Source.AttachAudioBuffer(this->AudioBuffer);
		}

		LuauClasses::Vector3 Position;

		inline void Update() {
			this->Source.Vec3SetProperty(AL_POSITION, this->Position);
		}
	};

	inline void Init() {

		// i do this so i can manually free it before deinitializing OpenAL (if i didn't do this a get a crash!!! yay!!!!)
		LoadedAudioBuffers = new std::map<const char*, ALAudioBuffer*>;

		const int InitiliazedFlags = Mix_Init(MIX_INIT_OGG);

		if (!(InitiliazedFlags & MIX_INIT_OGG)) {
			constexpr std::source_location SourceLocation;
			
			std::cerr << ":Mix_Init ERROR: Failed to initialize flag " << "MIX_INIT_OGG"
				<< "\nOccured in " << SourceLocation.file_name() << " at line " << SourceLocation.line()
				<< ".\nSDL_Mixer Error: " << Mix_GetError() << std::endl;
		}

		if (Mix_OpenAudio(AudioFrequency, AUDIO_F32, 2, 128) == -1) {
			constexpr std::source_location SourceLocation;

			std::cerr << ":Mix_OpenAudio ERROR: Failed to open audio device "
				<< "\nOccured in " << SourceLocation.file_name() << " at line " << SourceLocation.line()
				<< ".\nSDL_Mixer Error: " << Mix_GetError() << std::endl;
		}


		AudioDevice = alcOpenDevice(nullptr);
		if (AudioDevice) {
			ALContext = alcCreateContext(AudioDevice, nullptr);
			alcMakeContextCurrent(ALContext);
		}

		// X-RAM (neat little place to store ze music)
		if (alIsExtensionPresent("EAX-RAM") == AL_TRUE) {
			std::cout << "XRAM supported!\n";
			/*
			auto g_eaxSetMode = (decltype(EAXSetBufferMode)*)alGetProcAddress("EAXSetBufferMode");
			auto g_eaxGetMode = (decltype(EAXGetBufferMode)*)alGetProcAddress("EAXGetBufferMode");
			
			ALenum g_eXRAMSize = alGetEnumValue("AL_EAX_RAM_SIZE");
			ALenum g_eXRAMFree = alGetEnumValue("AL_EAX_RAM_FREE");
			ALenum g_eXRAMAuto = alGetEnumValue("AL_STORAGE_AUTOMATIC");
			ALenum g_eXRAMHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
			ALenum g_eXRAMAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");*/
		}

		/*
		auto AudioBuffer = LoadALAudioBuffer("mus\\loading.ogg");
		ALSource Source;

		Source.AttachAudioBuffer(AudioBuffer);
		Source.Play();*/

		//SDL_Delay(1000000u);
	}

	inline void Quit() {
		Mix_Quit();

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(ALContext);
		alcCloseDevice(AudioDevice);

		delete LoadedAudioBuffers;
	}
}

#endif