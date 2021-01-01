#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

std::string GetFileName(const std::string& s_basename, size_t counter)
{
	return s_basename + std::to_string(counter);
}

std::fstream OpenFileToWrite(const std::string& filename, uint8_t debug_mode)
{
	std::fstream stream_to_open;

	if (!debug_mode) {
		stream_to_open.open(filename, std::ios::out | std::ios::binary);
	} else {
		std::cout << "FILE " << filename << " CREATED" << "\n";
	}

	return stream_to_open;
}

std::ifstream OpenFileToRead(const std::string& filename, uint8_t debug_mode)
{
	std::ifstream stream_to_open;

	if (!debug_mode) {
		stream_to_open.open(filename, std::ifstream::in | std::ios::binary);
	} else {
		std::cout << "FILE " << filename << " OPENED" << "\n";
	}

	return stream_to_open;
}

void CloseFile(std::fstream& stream_to_close, uint8_t debug_mode)
{
	if (!debug_mode) {
		stream_to_close.close();
	} else {
		std::cout << "FILE CLOSED" << "\n";
	}
}

void WriteToFile(std::fstream& stream_to_write, std::vector<uint8_t>& buffer, size_t offset, size_t size, uint8_t debug_mode)
{
	if (!debug_mode) {
		stream_to_write.write(reinterpret_cast<char*>(buffer.data() + offset), size);
	} else {
		std::cout << size << " BYTES WRITTEN" << "\n";
	}
}

std::streamsize ReadFromFile(std::ifstream& stream_to_read, std::vector<uint8_t>& buffer)
{
	stream_to_read.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

	return stream_to_read.gcount();
}

std::vector<std::string> GetListOfFiles(const std::string& s_basename)
{
	std::vector<std::string> ret;

	for (auto& p : std::filesystem::directory_iterator("./")) {

		std::string file_name = p.path().filename().string();

		if (file_name.find(s_basename) == 0) {

			ret.push_back(file_name);

		}

	}

	return ret;
}


void FileMerge(const std::string& s_filename, const std::string& s_basename, uint8_t debug_mode)
{
	std::vector<std::string> list_of_files = GetListOfFiles(s_basename);
	std::fstream stream_to_write = OpenFileToWrite(s_filename, debug_mode);
	const std::size_t kB = 1024;
	size_t buffer_size = 4 * kB;
	std::vector<uint8_t> buffer(buffer_size);

	for (auto it = list_of_files.begin(); it != list_of_files.end(); ++it) {

		std::ifstream stream_to_read = OpenFileToRead(*it, debug_mode);

		while (1) {
			
			size_t number_of_bytes_read = ReadFromFile(stream_to_read, buffer);
			WriteToFile(stream_to_write, buffer, 0, number_of_bytes_read, debug_mode);

			if (number_of_bytes_read < buffer.size()) {
				stream_to_read.close();
				break;
			}
		}

	}

}

int main(int argc, char* argv[])
{
	if (argc != 3 && argc != 4) {
		std::cout << "Usage: ./fileMerge fileName baseName [debugMode]" << "\n";
		return 0;
	}

	const std::string s_filename(argv[1]);
	const std::string s_basename(argv[2]);
	const std::size_t kB = 1024;
	size_t buffer_size = 4 * kB;
	uint8_t debug_mode = 0;

	if (argc == 4) {
		try {
			debug_mode = std::stoi(argv[3]);
		} catch (std::invalid_argument&) {
			// if no conversion could be performed
			std::cout << "Could not get debug mode: " << argv[3] << "\n";
			return 0;
		} catch (std::out_of_range&) {
			// if the converted value would fall out of the range of the result type 
			// or if the underlying function (std::strtol or std::strtoull) sets errno 
			// to ERANGE.
			std::cout << "Could not get debug mode: " << argv[3] << "\n";
			return 0;
		} catch (...) {
			// everything else
			std::cout << "Could not get debug mode: " << argv[3] << "\n";
			return 0;
		}
	}

	FileMerge(s_filename, s_basename, debug_mode);
}