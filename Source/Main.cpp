#include <iostream>

#include "Extractor.hpp"


std::vector<std::string> file_paths
{
	// XDUs.
	"COMMON\\Interface\\Interface.xdu",
	"COMMON\\NPCs\\NPCs.xdu",
	"COMMON\\Resources\\Resources.xdu",
	"COMMON\\System\\System.xdu",
	"LEVELS\\0 Prelude Docks\\0 Prelude Docks.xdu",
	"LEVELS\\1 PrisonDeathRow\\1 PrisonDeathRow.xdu",
	"LEVELS\\2 Cellblocks\\2 Cellblocks.xdu",
	"LEVELS\\3 YardCellBlock\\3 YardCellBlock.xdu",
	"LEVELS\\4 Quarry\\4 Quarry.xdu",
	"LEVELS\\5 Asylum\\5 Asylum.xdu",
	"LEVELS\\6 Woods\\6 Woods.xdu",
	"LEVELS\\8 Bluff\\8 Bluff.xdu",
	"LEVELS\\9 Lighthouse\\9 Lighthouse.xdu",
	"LEVELS\\10 Docks\\10 Docks.xdu",

	// SDUs.
	"COMMON\\FX\\FX.sdu",
	"COMMON\\Interface\\Interface.sdu",
	"COMMON\\NPCs\\NPCs.sdu",
	"COMMON\\Resources\\Resources.sdu",
	"COMMON\\System\\System.sdu",
	"LEVELS\\0 Prelude Docks\\0 Prelude Docks.sdu",
	"LEVELS\\1 PrisonDeathRow\\1 PrisonDeathRow.sdu",
	"LEVELS\\2 Cellblocks\\2 Cellblocks.sdu",
	"LEVELS\\3 YardCellBlock\\3 YardCellBlock.sdu",
	"LEVELS\\4 Quarry\\4 Quarry.sdu",
	"LEVELS\\5 Asylum\\5 Asylum.sdu",
	"LEVELS\\6 Woods\\6 Woods.sdu",
	"LEVELS\\8 Bluff\\8 Bluff.sdu",
	"LEVELS\\9 Lighthouse\\9 Lighthouse.sdu",
	"LEVELS\\10 Docks\\10 Docks.sdu",
	"World Common\\World Common\\World Common.sdu",

	// TDUs.
	"COMMON\\FX\\FX.tdu",
	"COMMON\\Interface\\Interface.tdu",
	"COMMON\\NPCs\\NPCs.tdu",
	"COMMON\\Resources\\Resources.tdu",
	"COMMON\\System\\System.tdu",
	"LEVELS\\0 Prelude Docks\\0 Prelude Docks.tdu",
	"LEVELS\\0 Prelude Docks\\0 Prelude Docks_level.tdu",
	"LEVELS\\1 PrisonDeathRow\\1 PrisonDeathRow.tdu",
	"LEVELS\\1 PrisonDeathRow\\1a PrisonDeathRow_level.tdu",
	"LEVELS\\1 PrisonDeathRow\\1b PrisonDeathRow_level.tdu",
	"LEVELS\\1 PrisonDeathRow\\1c PrisonDeathRow_level.tdu",
	"LEVELS\\2 Cellblocks\\2 Cellblocks.tdu",
	"LEVELS\\2 Cellblocks\\2 Cellblocks_level.tdu",
	"LEVELS\\3 YardCellBlock\\3 YardCellBlock.tdu",
	"LEVELS\\3 YardCellBlock\\3a YardCellBlock_level.tdu",
	"LEVELS\\3 YardCellBlock\\3b YardCellBlock_level.tdu",
	"LEVELS\\4 Quarry\\4 Quarry.tdu",
	"LEVELS\\4 Quarry\\4a Quarry_level.tdu",
	"LEVELS\\4 Quarry\\4b Quarry_level.tdu",
	"LEVELS\\5 Asylum\\5 Asylum.tdu",
	"LEVELS\\5 Asylum\\5a Asylum_level.tdu",
	"LEVELS\\5 Asylum\\5b Asylum_level.tdu",
	"LEVELS\\6 Woods\\6 Woods.tdu",
	"LEVELS\\6 Woods\\6a Woods_level.tdu",
	"LEVELS\\6 Woods\\6b Woods_level.tdu",
	"LEVELS\\7 ReturnToPrison\\7 ReturnToPrison_level.tdu",
	"LEVELS\\8 Bluff\\8 Bluff.tdu",
	"LEVELS\\8 Bluff\\8a Bluff_level.tdu",
	"LEVELS\\8 Bluff\\8b Bluff_level.tdu",
	"LEVELS\\9 Lighthouse\\9 Lighthouse.tdu",
	"LEVELS\\9 Lighthouse\\9 Lighthouse_level.tdu",
	"LEVELS\\10 Docks\\10 Docks.tdu",
	"LEVELS\\10 Docks\\10 Docks_level.tdu",
	"World Common\\Asylum Common\\Asylum Common.tdu",
	"World Common\\Landscape Textures\\Landscape Textures.tdu",
	"World Common\\Outdoor Common\\Outdoor Common.tdu",
	"World Common\\Prison Common\\Prison Common.tdu",
	"World Common\\World Common\\World Common.tdu",
};


int main()
{
	try
	{
		// For each file path...
		for(int index{}; index < file_paths.size(); ++index)
		{
			std::string file_path{file_paths[index]};

			// Extract the file.
			Extractor extractor{file_path};
			extractor.extract();

			// Print the progress.
			std::cout<<index+1<<" of "<<file_paths.size()<<
				" database extractions complete.\n\n---\n\n";
		}

		std::cout<<"Finished.";
	}
	catch(std::runtime_error& error){ std::cout<<"\n\nFatal Error: "<<error.what(); }
}