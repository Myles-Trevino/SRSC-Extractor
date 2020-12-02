/*
	Copyright 2020 Myles Trevino
	Licensed under the Apache License, Version 2.0
	http://www.apache.org/licenses/LICENSE-2.0
*/


#include <iostream>
#include <filesystem>
#include <thread>
#include <mutex>
#include <queue>

#include "Utilities.hpp"
#include "Extractor.hpp"


std::vector<std::string> database_paths;
std::vector<std::thread> threads;
std::queue<std::thread::id> finished_thread_ids;
std::mutex completion_mutex;
int completed_count;
int error_count;


void thread_error(const std::string& database_path, const std::string& message)
{
	std::lock_guard<std::mutex> completion_mutex_lock{completion_mutex};
	std::cout<<"Error extracting \""<<database_path<<"\": "<<message<<'\n';
	++error_count;
}


// Thread function.
void thread_function(std::string database_path)
{
	try
	{
		// Extract the database.
		Extractor extractor{database_path};
		extractor.extract();

		std::lock_guard<std::mutex> completion_mutex_lock{completion_mutex};

		// Update the completion status.
		++completed_count;
		std::cout<<"\rCompleted "<<completed_count<<" of "<<database_paths.size()
			<<" ("<<static_cast<int>(static_cast<float>(completed_count)/
			database_paths.size()*100+.5f)<<"%).      ";
	}

	// Handle errors.
	catch(const std::exception& exception){ thread_error(database_path, exception.what()); }
	catch(...){ thread_error(database_path, "Unhandled exception."); }

	// Add the thread ID to the finished queue.
	std::lock_guard<std::mutex> completion_mutex_lock{completion_mutex};
	finished_thread_ids.push(std::this_thread::get_id());
}


// Main.
int main()
{
	try
	{
		std::cout<<"Searching for extractable databases (.SDU, .TDU, or .XDU files)...\n";

		// Check that the resources folder exists.
		if(!std::filesystem::exists("Resources"))
			throw std::runtime_error{"Could not find the resources folder. Please create a "
				"folder named \"Resources\" next to this executable containing the game "
				"resources you would like to extract ."};

		// Load the database paths.
		for(const std::filesystem::directory_entry& iterator :
			std::filesystem::recursive_directory_iterator("Resources"))
		{
			if(!iterator.is_regular_file() || !iterator.path().has_extension()) continue;

			std::string extension{Utilities::to_uppercase(
				iterator.path().extension().u8string())};

			if(extension != ".SDU" && extension != ".TDU" && extension != ".XDU") continue;
			database_paths.emplace_back(iterator.path().u8string());
		}

		if(database_paths.empty()) throw std::runtime_error{"No extractable databases "
			"(.SDU, .TDU, or .XDU) were found in the resources folder."};
		std::cout<<"Found "<<database_paths.size()<<" extractable databases.\n\n---\n\n";

		// Extract.
		std::cout<<"Extracting...\n";
		unsigned thread_count{std::thread::hardware_concurrency()/2};

		unsigned file_paths_offset{};
		for(const std::string& path : database_paths)
		{
			// If all threads are currently being used...
			if(threads.size() >= thread_count)
			{
				// Wait for a thread to finish.
				while(finished_thread_ids.size() <= 0)
					std::this_thread::sleep_for(std::chrono::milliseconds(1));

				// Erase the finished thread.
				std::thread::id id{finished_thread_ids.front()};
				finished_thread_ids.pop();

				for(int index{}; index < threads.size(); ++index)
					if(threads[index].get_id() == id)
					{
						threads[index].detach();
						threads.erase(threads.begin()+index);
					}
			}

			// Extract the database on a separate thread.
			threads.emplace_back(thread_function, path);
		}

		// Join the threads that are still running.
		for(std::thread& thread : threads) thread.join();
		std::cout<<"\nErrors: "<<error_count<<".\n\n---\n\nFinished.\n";
	}

	// Handle errors.
	catch(std::runtime_error& error){ std::cout<<"\n\nFatal Error: "<<error.what()<<'\n'; }
	catch(...){ std::cout<<"\n\nFatal unhandled exeption.\n"; }

	// Press enter to exit.
	std::string line;
	std::cout<<"Press enter to exit...\n";
	std::getline(std::cin, line);
}