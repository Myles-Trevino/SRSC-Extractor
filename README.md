# SRSC Extractor
Extracts the .XDU (text), .SDU (sound), and .TDU (texture) SRSC databases found in Surreal Software's "The Suffering".


## Usage
- Place an [FFmpeg](https://ffmpeg.org/) binary in the executable's directory. The extractor depends on FFmpeg for converting the sound data from raw PCM to WAV.
- Create a folder called "Resources" and place the "COMMON "LEVELS and "World Common" folders from "The Suffering" in it.
- Run the extractor and the files in all of the databases listed below will be extracted.


## Databases
Each database starts with a "Database Information" entry. It appears to be static except for the first byte which changes based on the archive type (05 for XDU, 19 for SDU, etc). All data in the archive is little-endian.


## .XDU - Text Database
Contains "Text" data entries, which are plain UTF-16 encoded strings.


### Locations
- `COMMON/Interface/Interface.xdu`
- `COMMON/NPCs/NPCs.xdu`
- `COMMON/Resources/Resources.xdu`
- `COMMON/System/System.xdu`
- `LEVELS/0 Prelude Docks/0 Prelude Docks.xdu`
- `LEVELS/1 PrisonDeathRow/1 PrisonDeathRow.xdu`
- `LEVELS/2 Cellblocks/2 Cellblocks.xdu`
- `LEVELS/3 YardCellBlock/3 YardCellBlock.xdu`
- `LEVELS/4 Quarry/4 Quarry.xdu`
- `LEVELS/5 Asylum/5 Asylum.xdu`
- `LEVELS/6 Woods/6 Woods.xdu`
- `LEVELS/8 Bluff/8 Bluff.xdu`
- `LEVELS/9 Lighthouse/9 Lighthouse.xdu`
- `LEVELS/10 Docks/10 Docks.xdu`


## .SDU - Sound Database
Contains "Sound" data entries preceded by a "Sound Information 1" entry and optionally preceeded by a "Sound Information 2" entry. "Sound" data is signed little-endian raw PCM audio. The sample rate is found at bytes 49 and 50, the bit depth is found at byte 46, and the channel count is found at byte 44 in the preceeding "Sound Information 1" entry.


### Locations
- `COMMON/FX/FX.sdu`
- `COMMON/Interface/Interface.sdu`
- `COMMON/NPCs/NPCs.sdu`
- `COMMON/Resources/Resources.sdu`
- `COMMON/System/System.sdu`
- `LEVELS/0 Prelude Docks/0 Prelude Docks.sdu`
- `LEVELS/1 PrisonDeathRow/1 PrisonDeathRow.sdu`
- `LEVELS/2 Cellblocks/2 Cellblocks.sdu`
- `LEVELS/3 YardCellBlock/3 YardCellBlock.sdu`
- `LEVELS/4 Quarry/4 Quarry.sdu`
- `LEVELS/5 Asylum/5 Asylum.sdu`
- `LEVELS/6 Woods/6 Woods.sdu`
- `LEVELS/8 Bluff/8 Bluff.sdu`
- `LEVELS/9 Lighthouse/9 Lighthouse.sdu`
- `LEVELS/10 Docks/10 Docks.sdu`
- `World Common/World Common/World Common.sdu`


## .TDU - Texture Database
Contains "Texture" and "Texture Information" data entries. Textures have a header containing various information about the texture data. The header begins with: width, height, row size, bits per pixel, alpha bits per pixel, and the color key. Each of these is 4 bytes. The header ends after 48 bytes and the texture data begins. The texture data is compressed as either DXT1 or DXT5. If the bits per pixel is 4, the compression is DXT1 and if the bits per pixel is 8, it is DXT5.

### Locations
- `COMMON/FX/FX.tdu`
- `COMMON/Interface/Interface.tdu`
- `COMMON/NPCs/NPCs.tdu`
- `COMMON/Resources/Resources.tdu`
- `COMMON/System/System.tdu`
- `LEVELS/0 Prelude Docks/0 Prelude Docks.tdu`
- `LEVELS/0 Prelude Docks/0 Prelude Docks_level.tdu`
- `LEVELS/1 PrisonDeathRow/1 PrisonDeathRow.tdu`
- `LEVELS/1 PrisonDeathRow/1a PrisonDeathRow_level.tdu`
- `LEVELS/1 PrisonDeathRow/1b PrisonDeathRow_level.tdu`
- `LEVELS/1 PrisonDeathRow/1c PrisonDeathRow_level.tdu`
- `LEVELS/2 Cellblocks/2 Cellblocks.tdu`
- `LEVELS/2 Cellblocks/2 Cellblocks_level.tdu`
- `LEVELS/3 YardCellBlock/3 YardCellBlock.tdu`
- `LEVELS/3 YardCellBlock/3a YardCellBlock_level.tdu`
- `LEVELS/3 YardCellBlock/3b YardCellBlock_level.tdu`
- `LEVELS/4 Quarry/4 Quarry.tdu`
- `LEVELS/4 Quarry/4a Quarry_level.tdu`
- `LEVELS/4 Quarry/4b Quarry_level.tdu`
- `LEVELS/5 Asylum/5 Asylum.tdu`
- `LEVELS/5 Asylum/5a Asylum_level.tdu`
- `LEVELS/5 Asylum/5b Asylum_level.tdu`
- `LEVELS/6 Woods/6 Woods.tdu`
- `LEVELS/6 Woods/6a Woods_level.tdu`
- `LEVELS/6 Woods/6b Woods_level.tdu`
- `LEVELS/7 ReturnToPrison/7 ReturnToPrison_level.tdu`
- `LEVELS/8 Bluff/8 Bluff.tdu`
- `LEVELS/8 Bluff/8a Bluff_level.tdu`
- `LEVELS/8 Bluff/8b Bluff_level.tdu`
- `LEVELS/9 Lighthouse/9 Lighthouse.tdu`
- `LEVELS/9 Lighthouse/9 Lighthouse_level.tdu`
- `LEVELS/10 Docks/10 Docks.tdu`
- `LEVELS/10 Docks/10 Docks_level.tdu`
- `World Common/Asylum Common/Asylum Common.tdu`
- `World Common/Landscape Textures/Landscape Textures.tdu`
- `World Common/Outdoor Common/Outdoor Common.tdu`
- `World Common/Prison Common/Prison Common.tdu`
- `World Common/World Common/World Common.tdu`


## Credits
[Zalasus](https://github.com/Zalasus/opendrakan) - Documentation on an older SRSC version used in Surreal Software's "Drakan".

[Benjamin Dobell](https://github.com/Benjamin-Dobell/s3tc-dxt-decompression)  - The S3TC DTX1/DTX5 Texture Decompression library.

[Sean Barret](https://github.com/nothings/stb) - The STB Image Write library.