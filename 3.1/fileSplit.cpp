//3.1 Exercise - File Splitting Program

//Sometimes you have a file that you would like to put on a floppy disk but it is too big.
//Perhaps you want to copy the file from one computer to another and floppy disks are your
//only means of transfer.It would be useful to split the long file into several short files that each
//fit on a floppy.After transfer to the other computer the short files can be concatenated into a
//copy of the original.The command line for this program is :

//. / fileSplit bigFile baseName chunkSize

//bigFile is the name of the big, existing file.Each small file will be named baseName with a
//number appended to the end.For example, if baseName is "chop" then the small files are
//"chop0", chop1", "chop2", and so on for as many as are needed.

//chunkSize is the size in bytes of each small file, except for the last one.
//For testing, use any file for bigFile, no matter what size, and any size for chunkSize.Use the
//file concatenation program to put the file together again.See if there are any changes with a
//file comparison program. (Such as Unix's dif or Microsofts Win's comp).
//For preliminary testing, use text files.However, write the program so that it works with any
//file(use byte - oriented IO).For advanced testing, split an executable file, then reassemble then
//run it.Or splitand reassemble an image file, and view the results.

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <utility>
#include <memory>

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

void WriteToFiles(
	std::vector<std::fstream>& streams_to_write,
	std::vector<size_t>& number_of_bytes_written,
	size_t& index, 
	size_t chunk_size, 
	std::vector<uint8_t> buffer,
	size_t size, 
	size_t& offset,
	uint8_t debug_mode)
{
	if (number_of_bytes_written[index] + size <= chunk_size) {
		// what if buffer_size < chunk_size ?
		// what if we read the last bytes and now we have number_of_bytes_read < chunk_size ?

		WriteToFile(streams_to_write[index], buffer, offset, size, debug_mode);
		number_of_bytes_written[index] += size;
		offset = 0;

	} else {

		size_t total_number_of_bytes_in_buffer = size;

		while (1) {

			size_t total_number_of_bytes_to_write = chunk_size - number_of_bytes_written[index];

			WriteToFile(streams_to_write[index], buffer, offset, total_number_of_bytes_to_write, debug_mode);
			number_of_bytes_written[index] += total_number_of_bytes_to_write;
			total_number_of_bytes_in_buffer -= total_number_of_bytes_to_write;
			offset += total_number_of_bytes_to_write;

			CloseFile(streams_to_write[index], debug_mode);

			++index;

			if (total_number_of_bytes_in_buffer < chunk_size) {

				WriteToFiles(
					streams_to_write,
					number_of_bytes_written,
					index,
					chunk_size,
					buffer,
					total_number_of_bytes_in_buffer,
					offset,
					debug_mode);

				break;
			}
		}

	}
}

void ReadAndSplitFile(
	std::ifstream& stream_to_read,
	std::vector<std::fstream>& streams_to_write,
	size_t chunk_size,
	size_t buffer_size,
	uint8_t debug_mode)
{
	std::vector<uint8_t> buffer(buffer_size);
	std::vector<size_t> number_of_bytes_written(streams_to_write.size());
	size_t file_index = 0;
	size_t write_offset = 0;

	for (auto it = number_of_bytes_written.begin(); it != number_of_bytes_written.end(); ++it) {
		*it = 0;
	}

	while (true) {

		size_t number_of_bytes_read = static_cast<size_t>(ReadFromFile(stream_to_read, buffer));

		if (!number_of_bytes_read) {
			CloseFile(streams_to_write[file_index], debug_mode);
			break;
		}

		WriteToFiles(
			streams_to_write, 
			number_of_bytes_written, 
			file_index, 
			chunk_size, 
			buffer, 
			number_of_bytes_read, 
			write_offset, 
			debug_mode);

	}

}

int main(int argc, char* argv[])
{
	if (argc != 4 && argc != 5 && argc != 6) {
		std::cout << "Usage: ./fileSplit bigFile baseName chunkSize [bufferSize] [debugMode]" << "\n";
		return 0;
	}

	const std::string s_filename(argv[1]);
	const std::string s_basename(argv[2]);
	const std::size_t kB = 1024;
	size_t chunk_size;
	size_t buffer_size = 4 * kB;
	uint8_t debug_mode = 0;

	try {
		chunk_size = std::stoi(argv[3]);
	} catch (std::invalid_argument&) {
		// if no conversion could be performed
		std::cout << "Could not get chunk size: " << argv[3] << "\n";
		return 0;
	} catch (std::out_of_range&) {
		// if the converted value would fall out of the range of the result type 
		// or if the underlying function (std::strtol or std::strtoull) sets errno 
		// to ERANGE.
		std::cout << "Could not get chunk size: " << argv[3] << "\n";
		return 0;
	} catch (...) {
		// everything else
		std::cout << "Could not get chunk size: " << argv[3] << "\n";
		return 0;
	}

	if (argc == 5) {
		try {
			buffer_size = std::stoi(argv[4]);
		} catch (std::invalid_argument&) {
			// if no conversion could be performed
			std::cout << "Could not get buffer size: " << argv[4] << "\n";
			return 0;
		} catch (std::out_of_range&) {
			// if the converted value would fall out of the range of the result type 
			// or if the underlying function (std::strtol or std::strtoull) sets errno 
			// to ERANGE.
			std::cout << "Could not get buffer size: " << argv[4] << "\n";
			return 0;
		} catch (...) {
			// everything else
			std::cout << "Could not get buffer size: " << argv[4] << "\n";
			return 0;
		}
	}

	if (argc == 6) {
		try {
			debug_mode = std::stoi(argv[5]);
		} catch (std::invalid_argument&) {
			// if no conversion could be performed
			std::cout << "Could not get debug mode: " << argv[5] << "\n";
			return 0;
		} catch (std::out_of_range&) {
			// if the converted value would fall out of the range of the result type 
			// or if the underlying function (std::strtol or std::strtoull) sets errno 
			// to ERANGE.
			std::cout << "Could not get debug mode: " << argv[5] << "\n";
			return 0;
		} catch (...) {
			// everything else
			std::cout << "Could not get debug mode: " << argv[5] << "\n";
			return 0;
		}
	}

	std::ifstream stream_file(s_filename, std::ifstream::in | std::ios::binary);

	if (!stream_file) {
		std::cout << "Could not open file: " << s_filename << "\n";
		return 0;
	}

	size_t number_of_files = static_cast<size_t>(std::ceil(std::filesystem::file_size(s_filename.data()) / static_cast<float>(chunk_size)));
	std::vector<std::fstream> streams_to_write(number_of_files);

	for (size_t i = 0; i < number_of_files; ++i) {

		streams_to_write[i].open(GetFileName(s_basename, i), std::ifstream::out | std::ios::binary);

	}

	ReadAndSplitFile(stream_file, streams_to_write, chunk_size, buffer_size, debug_mode);

	return 0;
}