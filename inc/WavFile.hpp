#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <SDL2/SDL.h>

class	WavFile
{
	public:
		struct	Header
		{
			struct	Riff
			{
				char		riff[4];
				uint32_t	file_size;
				char		wave[4];
			}	riff_header;

			char		fmt[4];
			uint32_t	fmt_size;
			uint16_t	audio_format;
			uint16_t	num_channels;
			uint32_t	sample_rate;
			uint32_t	byte_rate;
			uint16_t	block_align;
			uint16_t	bits_per_sample;

			char		data[4];
			uint32_t	data_size;
		};
	private:
		struct Chunk
		{
			char		id[4];
			uint32_t	size;
		};

	public:
		WavFile() {}
		~WavFile()
		{
			if (_loaded)
				SDL_CloseAudioDevice(_dev);
		}

		void	load(const std::string &path)
		{
			std::ifstream	file;

			file.open(path, std::ios::binary);
			if (!file.is_open())
				throw std::runtime_error("Failed to open " + path);

			file.read((char*)&_header.riff_header, sizeof(WavFile::Header::Riff));

			if (std::memcmp(_header.riff_header.riff, "RIFF", 4)
				|| std::memcmp(_header.riff_header.wave, "WAVE", 4))
				throw std::runtime_error("Not a WAV file " + path);

			Chunk chunk;

			while (file.read((char*)&chunk, sizeof(chunk)))
			{
				if (!std::memcmp(chunk.id, "fmt ", 4))
					file.read((char*)&_header.audio_format, chunk.size);
				else if (!std::memcmp(chunk.id, "data", 4))
				{
					_header.data_size = chunk.size;

					samples.resize(chunk.size / (_header.bits_per_sample / 8));
					file.read((char*)samples.data(), chunk.size);
					break;
				}
				else
					file.seekg((chunk.size + 1) & ~1, std::ios::cur);
			}

			if (_header.audio_format != 1)
				throw std::runtime_error("Invalid format (Expecting PCM)");
		}
		void	write(const std::string &path)
		{
			std::ofstream file(path, std::ios::binary);
			if (!file)
				throw std::runtime_error("Failed to open " + path);

			uint32_t data_size = samples.size() * sizeof(int16_t);

			file.write("RIFF", 4);
			uint32_t file_size = 36 + data_size;
			file.write((char*)&file_size, 4);

			file.write("WAVE", 4);
			file.write("fmt ", 4);

			uint32_t fmt_size = 16;
			file.write((char*)&fmt_size, 4);

			uint16_t audio_format = 1;
			file.write((char*)&audio_format, 2);

			file.write((char*)&_header.num_channels, 2);
			file.write((char*)&_header.sample_rate, 4);

			uint32_t byte_rate = _header.sample_rate * _header.num_channels * _header.bits_per_sample / 8;

			file.write((char*)&byte_rate, 4);

			uint16_t block_align = _header.num_channels * _header.bits_per_sample / 8;

			file.write((char*)&block_align, 2);

			file.write((char*)&_header.bits_per_sample, 2);

			file.write("data", 4);
			file.write((char*)&data_size, 4);

			file.write((char*)samples.data(), data_size);
		}

		void	play()
		{
			SDL_AudioSpec spec = {};
			spec.freq = _header.sample_rate;
			spec.format = AUDIO_S16LSB;
			spec.channels = _header.num_channels;
			spec.samples = 4096;

			_dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
			_loaded = true;

			SDL_QueueAudio(_dev, samples.data(), samples.size() * sizeof(int16_t));

			_totalTime = ((double)samples.size() / ((double)_header.sample_rate * (double)_header.num_channels));

			SDL_PauseAudioDevice(_dev, 0);
		}
		void	update(float delta)
		{
			_time += delta;
			if (_time >= _totalTime)
			{
				_time = 0;
				SDL_ClearQueuedAudio(_dev);
				SDL_QueueAudio(_dev, samples.data(), samples.size() * sizeof(int16_t));
				SDL_PauseAudioDevice(_dev, 0);
			}
		}
		static void	play(const std::string &path)
		{
			WavFile	wav;

			wav.load(path);
			wav.play();
		}

	private:
		double					_time = 0;
		double					_totalTime = 0;
		bool					_loaded = false;
		SDL_AudioDeviceID		_dev;
		std::vector<int16_t>	samples;
		Header					_header;
};
