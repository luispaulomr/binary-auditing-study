#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <sstream>
#include <random>

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

void WriteToFile(std::fstream& stream_to_write, const std::string& buffer, size_t offset, size_t size, uint8_t debug_mode)
{
	if (!debug_mode) {
		stream_to_write.write(buffer.data() + offset, size);
	} else {
		std::cout << size << " BYTES WRITTEN" << "\n";
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

std::string ReadFile(const std::string& file_path)
{
        std::ifstream t(file_path);
        std::string str;

	if (!t.is_open()) {
		return std::string();
	}

        t.seekg(0, std::ios::end);
        str.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());

        return str;
}

size_t GetMainSectionIndex(const std::vector<std::string>& v_file_content)
{
	for (auto it = v_file_content.begin(); it != v_file_content.end(); ++it) {

		std::size_t found = it->find("main:");

		if (found != std::string::npos) {
			return it - v_file_content.begin();
		}

	}

	return 0;
}

size_t GetEndOfMainSectionIndex(const std::vector<std::string>& v_file_content)
{
	auto it_main_section = v_file_content.begin();

	for (auto it = v_file_content.begin(); it != v_file_content.end(); ++it) {

		std::size_t found = it->find("main:");

		if (found != std::string::npos) {
			it_main_section = it + 1;
			break;
		}

	}

	for (auto it = it_main_section; it != v_file_content.end(); ++it) {

		std::size_t found = it->find("leave");

		if (found != std::string::npos) {
			return it - v_file_content.begin();
		}

	}

	return 0;
}

size_t GetRandomNumberBetween(size_t begin, size_t end)
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(begin, end); // define the range

	return distr(gen);
}

std::vector<std::string> SplitFileContent(const std::string& s_file_content)
{
	std::stringstream ss(s_file_content);
	std::string to;
	std::vector<std::string> ret;

	while (std::getline(ss, to, '\n')) {
		ret.push_back(to);
	}

	return ret;
}

int main(int argc, char* argv[])
{
        if (argc != 3) {
                std::cout << "Usage: ./obfuscate file.s output.s" << "\n";
                return 0;
        }
	
	std::string s_input_file(argv[1]);
	std::string s_output_file(argv[2]);

        std::string s_file_content = ReadFile(s_input_file);

	if (s_file_content.empty()) {
		std::cout << "Could not read from file " << s_input_file << "\n";
		return 0;
	}

	std::vector<std::string> v_file_content = SplitFileContent(s_file_content);

        size_t index_main_section = GetMainSectionIndex(v_file_content);
	size_t index_end_of_main_section = GetEndOfMainSectionIndex(v_file_content);
	size_t index_to_modify = GetRandomNumberBetween(index_main_section, index_end_of_main_section - 1);

	std::cout << "MAIN SECTION BEGINS IN LINE: " << index_main_section << "\n";
	std::cout << "MAIN SECTION ENDS IN LINE: " << index_end_of_main_section << "\n";
	std::cout << "RANDOM NUMBER: " << index_to_modify << "\n";

	std::string s_to_write("jmp $+6\n.string \"LUL\"\nLUL:\n");

	std::fstream stream_to_write = OpenFileToWrite(s_output_file, 0);

	for (auto i = 0; i <= index_to_modify; ++i) {
		WriteToFile(stream_to_write, v_file_content[i] + "\n", 0, v_file_content[i].size() + 1, 0);
	}

	WriteToFile(stream_to_write, s_to_write, 0, s_to_write.size(), 0);

	for (auto i = index_to_modify + 1; i < v_file_content.size(); ++i) {
		WriteToFile(stream_to_write, v_file_content[i] + "\n", 0, v_file_content[i].size() + 1, 0);
	}

        return 0;
}