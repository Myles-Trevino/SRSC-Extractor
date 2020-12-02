# SRSC Extractor
Extracts the .XDU (text), .SDU (sound), and .TDU (texture) SRSC databases found in Surreal Software's "The Suffering".


## Usage
- Place an [FFmpeg](https://ffmpeg.org/) binary in the executable's directory. The extractor depends on FFmpeg for converting the sound data from raw PCM to WAV.
- Create a folder called "Resources" and place the game's resources in it.
- Run the program and the files in the databases listed below will be extracted.


## Databases
Each SRSC database starts with a header. The header contains the following information in order: file signature (4 bytes), SRSC version (2 bytes), entry list offset (4 bytes), and entry count (2 bytes). After the header, there is a list of entries starting at the entry list offset. Each entry has its own header and data. The information in the entry headers is as follows: type (2 bytes), id (2 bytes), group id (2 bytes), data offset (4 bytes), and data size (4 bytes). Each database starts with a "Database Information" entry which contains information such as the database type. The types of the entries following the "Database Information" entry depends on the type of the database. The currently supported database types are described below.


## .XDU - Text Database
Contains "Text" data entries. The first two bytes is the size of the string. This is immediately followed by UTF-16 string data. The extractor converts these entries to plain UTF-8 encoded strings.


## .SDU - Sound Database
Contains "Sound" data entries preceded by a "Sound Information 1" entry and optionally preceeded by a "Sound Information 2" entry. Sound data is signed little-endian raw PCM audio. The sample rate is found at bytes 49 and 50, the bit depth is found at byte 46, and the channel count is found at byte 44 in the preceeding "Sound Information 1" entry.


## .TDU - Texture Database
Contains "Texture" and "Texture Information" data entries. Textures have a header containing information about the texture data. The header begins with: width, height, row size, bits per pixel, alpha bits per pixel, and the color key. Each of these is 4 bytes. The header ends after 48 bytes and the texture data begins. The texture data is compressed as either DXT1 or DXT5. If the bits per pixel is 4, the compression is DXT1 and if the bits per pixel is 8, it is DXT5.


## Credits
[Zalasus](https://github.com/Zalasus/opendrakan) - Documentation on an older SRSC version used in Surreal Software's "Drakan".

[Benjamin Dobell](https://github.com/Benjamin-Dobell/s3tc-dxt-decompression)  - The S3TC DTX1/DTX5 Texture Decompression library.

[Sean Barret](https://github.com/nothings/stb) - The STB Image Write library.
