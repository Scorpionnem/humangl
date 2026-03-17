#include "Chunk.hpp"

void	Chunk::generate(/*Generator *gen*/)
{
	_blocks.resize(CHUNK_VOLUME);

	ChunkLocalVec3i	pos;

	for (pos.x = 0; pos.x < CHUNK_SIZE; pos.x++)
		for (pos.y = 0; pos.y < CHUNK_SIZE; pos.y++)
			for (pos.z = 0; pos.z < CHUNK_SIZE; pos.z++)
			{
				if (rand() % 2)
				{
					setBlock(pos, 1);
				}
				else
					setBlock(pos, 0);
			}
	_state = Chunk::State::EDITED;
}

void	Chunk::save(const std::string &path)
{
	std::ofstream	file;

	file.open(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("chunk failed to open " + path);

	Chunk::Header	hdr;

	hdr.size = CHUNK_SIZE;
	std::memcpy(hdr.magic, CHUNK_HEADER_MAGIC, CHUNK_HEADER_MAGIC_SIZE);

	file.write(reinterpret_cast<char*>(&hdr), sizeof(Chunk::Header));
	file.write(reinterpret_cast<char*>(_blocks.data()), _blocks.size() * sizeof(ChunkBlockStateId));
}

void	Chunk::load(const std::string &path)
{
	struct stat	s;
	stat(path.c_str(), &s);
	if (s.st_size != sizeof(Chunk::Header) + CHUNK_VOLUME * sizeof(ChunkBlockStateId))
		throw std::runtime_error("Invalid save file size");

	std::ifstream	file;

	file.open(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("chunk failed to open " + path);

	Chunk::Header	hdr;

	file.read(reinterpret_cast<char*>(&hdr), sizeof(Chunk::Header));

	if (memcmp(hdr.magic, CHUNK_HEADER_MAGIC, CHUNK_HEADER_MAGIC_SIZE))
		throw std::runtime_error("Invalid chunk header magic");
	if (hdr.size != CHUNK_SIZE)
		throw std::runtime_error("Invalid chunk size");

	_blocks.resize(CHUNK_VOLUME);

	file.read(reinterpret_cast<char*>(_blocks.data()), CHUNK_VOLUME * sizeof(ChunkBlockStateId));
}
