#include "Extractor.hpp"

#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <S3TC/s3tc.h>

#include "Utilities.hpp"


// Loads the SRSC database at the given path.
Extractor::Extractor(const std::string& file_path) : file_path(file_path)
{
	std::string full_file_path{"Resources\\"+file_path};
	std::cout<<"Loading database \""<<full_file_path<<"\"...\n";

	// Open the input stream.
	input_stream.open(full_file_path.c_str(), std::ios::in|std::ios::binary);
	if(!input_stream) throw std::runtime_error{"Failed to open the file."};

	// Get the extension.
	size_t extension_index{file_path.find_last_of('.')};
	if(extension_index == std::string::npos)
		throw std::runtime_error{"Failed to get the file extension."};
	extension = Utilities::to_uppercase(file_path.substr(extension_index+1));

	// Get the database type string.
	get_database_type_string();

	// Check the file signature (first 4 bytes).
	uint32_t file_signature{binary_read<uint32_t>()};
	if(file_signature != 0x43535253 /* SRSC */)
		throw std::runtime_error{"Invalid file signature."};

	// Get the SRSC version (2 bytes).
	version = binary_read<uint16_t>();
	if(version != 0x0101) throw std::runtime_error{"Unsupported SRSC version."};

	// Get the entry list offset (4 bytes).
	entry_list_offset = binary_read<uint32_t>();

	// Get the entry count (2 bytes).
	uint16_t entry_count{binary_read<uint16_t>()};
	entries.resize(entry_count);

	// Seek to the entry list.
	input_stream.seekg(entry_list_offset);

	// Get the entries.
	for(uint16_t index{}; index < entries.size(); ++index)
	{
		Entry entry;
		entry.index = index;
		entry.type = static_cast<Entry_Type>(binary_read<uint16_t>());
		entry.id = binary_read<uint16_t>();
		entry.group_id = binary_read<uint16_t>();
		entry.data_offset = binary_read<uint32_t>();
		entry.data_size = binary_read<uint32_t>();
		entries[index] = entry;
	}

	// Print the type.
	std::cout<<"Type: "<<extension<<" - "<<type<<"\nLoaded.\n\n";
}


// Gets the type of the loaded database.
void Extractor::get_database_type_string()
{
	if(extension == "SDU") type = "Sounds";
	else if(extension == "TDU") type = "Textures";
	else if(extension == "XDU") type = "Text";
	else type = "Unsupported";
}


// Converts the given entry type to a string.
std::string Extractor::get_entry_type_string(Entry_Type type)
{
	switch(type)
	{
	case Entry_Type::TEXTURE: return "Texture";
	case Entry_Type::TEXTURE_INFORMATION: return "Texture Information";

	case Entry_Type::SOUND_INFORMATION_1: return "Sound Information 1";
	case Entry_Type::SOUND: return "Sound";
	case Entry_Type::SOUND_INFORMATION_2: return "Sound Information 2";

	case Entry_Type::STRING: return "Text";
	case Entry_Type::STRING_DATABASE_INFORMATION: return "Database Information";

	default: return "Unknown";
	}
}


// Gets the appropriate extension for the given entry type.
std::string Extractor::get_entry_type_extension(Entry_Type type)
{
	switch(type)
	{
	case Entry_Type::TEXTURE: return ".png"; break;
	case Entry_Type::SOUND: return ".raw"; break;
	case Entry_Type::STRING: return ".txt"; break;

	case Entry_Type::TEXTURE_INFORMATION:
	case Entry_Type::SOUND_INFORMATION_1:
	case Entry_Type::SOUND_INFORMATION_2:
	case Entry_Type::STRING_DATABASE_INFORMATION:
		return "";
	break;

	default: return "Unsupported";
	}
}


// Extracts the files from the loaded database.
void Extractor::extract()
{
	std::cout<<"Extracting...\n";

	// Create the output directory.
	std::string output_folder{"Extraction\\"+file_path};
	system(("if not exist \""+output_folder+"\" mkdir \""+output_folder+'"').c_str());

	// For each entry...
	int extracted_count{};
	for(int index{}; index < entries.size(); ++index)
	{
		entry = entries[index];

		// Print the progress.
		std::string entry_type_string{get_entry_type_string(entry.type)};
		std::cout<<"\rEntry "<<index+1<<" of "<<entries.size()<<": "<<
			Utilities::get_size_string(entry.data_size)<<" "<<entry_type_string<<
			"                 ";

		// Get the extension.
		std::string extension{get_entry_type_extension(entry.type)};
		if(extension == "Unsupported") throw std::runtime_error{"Failed to extract an entry."
			" Unsupported type "+std::to_string(static_cast<uint16_t>(entry.type))+'.'};

		// Generate the output path.
		output_path = output_folder+"\\Entry "+std::to_string(index+1)+
			" ("+entry_type_string+")"+extension;

		// Seek to the entry's data.
		input_stream.seekg(entry.data_offset);

		// Extract the entry's data.
		switch(entry.type)
		{
		case Entry_Type::TEXTURE: extract_texture(); break;
		case Entry_Type::STRING: extract_string(); break;
		case Entry_Type::SOUND: extract_sound(); break;
		case Entry_Type::SOUND_INFORMATION_1: extract_sound_information_1(); break;
		default: extract_default();
		}

		++extracted_count;
	}

	std::cout<<"\nExtracted.\n\n";
}


// Creates an output stream.
void Extractor::create_output_stream()
{
	output_stream.open(output_path, std::ios::out|std::ios::binary);
	if(!output_stream) throw std::runtime_error{"Failed to create an ouput stream."};
}


// Get the current entry's data starting at the given offset.
std::vector<uint8_t> Extractor::get_data(int offset)
{
	std::vector<uint8_t> data;
	data.resize(static_cast<size_t>(entry.data_size)-offset);

	input_stream.seekg(static_cast<size_t>(entry.data_offset)+offset);
	input_stream.read(reinterpret_cast<char*>(data.data()), data.size());

	return data;
}


// Extracts the current entry by directly copying the data.
std::vector<uint8_t> Extractor::extract_default()
{
	std::vector<uint8_t> data{get_data()};

	create_output_stream();
	output_stream.write(reinterpret_cast<char*>(data.data()), data.size());
	output_stream.close();

	return data;
}


// Extracts the current entry as a string.
void Extractor::extract_string()
{
	// Get the string length.
	uint16_t length{binary_read<uint16_t>()};

	// Read the data as a UTF-16 string.
	std::wstring utf16_string;
	for(uint32_t index{}; index < entry.data_size; index += sizeof(wchar_t))
	{
		wchar_t character;
		input_stream.read(reinterpret_cast<char*>(&character), sizeof(wchar_t));
		utf16_string += character;
	}

	// Convert to UTF-8.
	std::string utf8_string{Utilities::utf16_to_utf8(utf16_string).c_str()};

	// Output.
	create_output_stream();
	output_stream<<utf8_string;
	output_stream.close();
}


// Extracts the current entry as sound information 1 data.
void Extractor::extract_sound_information_1()
{
	// Output the data directly.
	std::vector<uint8_t> data{extract_default()};

	// Get the channel count (byte 44).
	channel_count = data[44];

	// Get the bit depth (byte 46).
	bit_depth = data[46];

	// Get the sample rate (bytes 49 and 50).
	sample_rate = data[48]|static_cast<uint16_t>(data[49])<<8;
}


// Extracts the current entry as a sound.
void Extractor::extract_sound()
{
	// Extract via direct copy to create the raw PCM file.
	extract_default();

	// Generate the WAV output path.
	std::string wav_output_path{output_path.substr(
		0, output_path.find_last_of('.'))+".wav"};

	// Convert the raw PCM to WAV and delete the raw PCM file.
	system(("ffmpeg -nostats -hide_banner -loglevel panic -y -f s"+
		std::to_string(bit_depth)+"le -ac "+std::to_string(channel_count)+
		" -ar "+std::to_string(sample_rate)+" -i \""+output_path+"\" \""+
		wav_output_path+"\" && del \""+output_path+'"').c_str());
}


// Extracts the current entry as a texture.
void Extractor::extract_texture()
{
	// Get the width, height, and pitch (4 bytes each).
	uint32_t width{binary_read<uint32_t>()};
	uint32_t height{binary_read<uint32_t>()};
	uint32_t row_size{binary_read<uint32_t>()};

	// Get the bit depth, alpha bit depth, and color key (4 bytes each).
	uint32_t bits_per_pixel{binary_read<uint32_t>()};
	uint32_t alpha_bits_per_pixel{binary_read<uint32_t>()};
	uint32_t color_key{binary_read<uint32_t>()};

	// Determine the texture type.
	Texture_Type type;
	if(bits_per_pixel == 4) type = Texture_Type::DXT1;
	else if(bits_per_pixel == 8) type = Texture_Type::DXT5;
	else throw std::runtime_error{"Unsupported texture type."};

	// Check for trailing bytes.
	uint32_t trailing_bytes{static_cast<uint32_t>(row_size-width*(bits_per_pixel/8.f))};
	if(trailing_bytes) throw std::runtime_error{"Texture has trailing bytes."};

	// Check for a color key.
	if(color_key != 0xFFFFFFFF) throw std::runtime_error{"Unsupported color key."};

	// Get the compressed image data (starts at byte 49).
	std::vector<uint8_t> compressed_data{get_data(48)};

	// Decompress the image data.
	std::vector<uint32_t> decompressed_data;
	decompressed_data.resize(static_cast<size_t>(width)*height);

	if(type == Texture_Type::DXT1) BlockDecompressImageDXT1(width, height,
		compressed_data.data(), reinterpret_cast<unsigned long*>(decompressed_data.data()));
	else if(type == Texture_Type::DXT5) BlockDecompressImageDXT5(width, height,
		compressed_data.data(), reinterpret_cast<unsigned long*>(decompressed_data.data()));

	// Swap the texture data's endianness.
	for(uint32_t& pixel : decompressed_data)
	{
		uint8_t a{static_cast<uint8_t>((pixel&0xFF000000)>>24)};
		uint8_t b{static_cast<uint8_t>((pixel&0xFF0000)>>16)};
		uint8_t g{static_cast<uint8_t>((pixel&0xFF00)>>8)};
		uint8_t r{static_cast<uint8_t>(pixel&0xFF)};
		pixel = r<<24|g<<16|b<<8|a;
	}

	// Write the decompressed data as a PNG.
	stbi_flip_vertically_on_write(true);
	if(!stbi_write_png(output_path.c_str(), width,
		height, 4, decompressed_data.data(), width*4))
		throw std::runtime_error{"Failed to write the PNG."};
}