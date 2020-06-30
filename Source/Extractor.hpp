#pragma once

#include <string>
#include <vector>
#include <fstream>


enum class Texture_Type{DXT1, DXT5};


enum class Entry_Type
{
	TEXTURE = 0x0040,
	TEXTURE_INFORMATION = 0x0041,

	SOUND_INFORMATION_1 = 0x0302,
	SOUND = 0x0303,
	SOUND_INFORMATION_2 = 0x0305,

	STRING = 0x0400,
	STRING_DATABASE_INFORMATION = 0x0402
};


struct Entry
{
	uint16_t index;
	Entry_Type type;
	uint16_t id;
	uint16_t group_id;
	uint32_t data_offset;
	uint32_t data_size;
};


class Extractor
{
public:
	Extractor(const std::string& file_path);
	void extract();

private:
	template<typename T> T binary_read()
	{
		T data;
		input_stream.read(reinterpret_cast<char*>(&data), sizeof(T));
		return data;
	}

	void get_database_type_string();
	std::string get_entry_type_string(Entry_Type type);
	std::string get_entry_type_extension(Entry_Type type);

	void create_output_stream();
	std::vector<uint8_t> get_data(int offset = 0);

	std::vector<uint8_t> extract_default();
	void extract_string();
	void extract_sound_information_1();
	void extract_sound();
	void extract_texture();

	std::ifstream input_stream;
	std::ofstream output_stream;

	Entry entry;
	std::string output_path;
	uint8_t channel_count;
	uint8_t bit_depth;
	uint16_t sample_rate;

	std::string file_path;
	std::string extension;
	std::string type;
	uint16_t version;
	uint32_t entry_list_offset;
	std::vector<Entry> entries;
};