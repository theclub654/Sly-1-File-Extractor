#include "main.h"

int main(int argc, char** argv)
{
	const char* file = argv[1];

	if (file == nullptr)
	{
		std::cout << "Drag Sly 1 ISO file to program to extract files NTSC Supported Only\n";
		getchar();
		return -1;
	}

	std::ifstream ISO(file, std::ios::binary);
	std::vector <char> fileBuffer;

	std::string region{};
	region.resize(4);

	ISO.seekg(0x828BD, std::ios::beg);
	ISO.read(&region[0], 4);

	if (region != "SCUS")
	{
		std::cout << "Invalid File\n";
		getchar();
		return -1;
	}

	ISO.seekg(0x1D2B14, std::ios::beg);

	for (int i = 0; i < 45; i++)
	{
		ISO.seekg(0x8, SEEK_CUR);
		std::cout << "Extracting " << levelNames[i] << "\n";

		uint32_t temp0; // sector offset
		uint32_t temp1; // file size
		uint32_t temp2; // search value
		uint32_t temp3; // for lsn
		uint32_t temp4; // search cipher
		uint32_t temp5; // for size
		uint32_t temp6; // level ID
		uint32_t temp7; // Level Name

		ISO.read(reinterpret_cast<char*> (&temp0), sizeof(uint32_t));
		ISO.read(reinterpret_cast<char*> (&temp1), sizeof(uint32_t));
		ISO.read(reinterpret_cast<char*> (&temp2), sizeof(uint32_t));
		ISO.read(reinterpret_cast<char*> (&temp3), sizeof(uint32_t));
		ISO.read(reinterpret_cast<char*> (&temp4), sizeof(uint32_t));
		ISO.read(reinterpret_cast<char*> (&temp5), sizeof(uint32_t));
		ISO.read(reinterpret_cast<char*> (&temp6), sizeof(uint32_t));
		ISO.read(reinterpret_cast<char*> (&temp7), sizeof(uint32_t));

		long size = temp1 ^ temp7;
		long sectorOffset = temp0 ^ temp5;
		sectorOffset = sectorOffset * 0x800;
		fileBuffer.resize(size);
		ISO.seekg(4, SEEK_CUR);
		long nextFileTable = ISO.tellg();
		ISO.seekg(sectorOffset, SEEK_SET);
		ISO.read(&fileBuffer[0], size);

		std::vector <unsigned char> outputData;
		Decompress(fileBuffer, size ,outputData);

		std::ofstream output(levelNames[i], std::ios::binary | std::ios::out);
		output.write((char*)&outputData[0], outputData.size());
		output.close();
		ISO.seekg(nextFileTable, SEEK_SET);

		fileBuffer.clear();
		fileBuffer.shrink_to_fit();
		outputData.clear();
		outputData.shrink_to_fit();
	}
}

void Decompress(std::vector <char>& filebuffer, uint64_t size ,std::vector <unsigned char> &outputData)
{
	std::vector <char> output_data;
	output_data.resize(0x4000);
	//static char* output_data = new char[0x4000];
	uint32_t actual_output_data_size = 10 * size;
	std::vector <char> actual_output_data;
	actual_output_data.resize(actual_output_data_size);
	//static char* actual_output_data = new char[actual_output_data_size];

	uint64_t input_size = size;
	uint64_t input_base = 0;
	uint64_t output_base = 0;
	uint64_t output_read = 0;

	uint64_t input_pos = 0;
	uint64_t output_pos = 0;

	unsigned char bits = 0;
	unsigned short src = 0;
	short ssize = 0;
	short offset = 0;
	int i = 0;
	unsigned long k = 0;

	while (input_pos < input_size) {
		bits = (unsigned char)filebuffer[input_pos++];
		if (input_pos >= input_size)
			break;

		for (i = 0; i < 8; i++)
		{
			src = (unsigned char)filebuffer[input_pos++];
			if (input_pos >= input_size)
				break;

			if (bits & 1) {
				output_data[output_pos++] = src;
				if (output_pos >= 0x2000)
				{
					output_pos &= 0x1fff;
					memcpy(&actual_output_data[0] + (k++ * 0x2000), &output_data[0], 0x2000);
				}
			}
			else {
				src |= ((unsigned short)(filebuffer[input_pos++]) << 8);
				ssize = (src >> 13) + 2;
				offset = src & 0x1FFF;
				while (ssize >= 0) {
					--ssize;
					output_data[output_pos++] = output_data[offset];
					if (output_pos >= 0x2000)
					{
						output_pos &= 0x1fff;
						memcpy(&actual_output_data[0] + (k++ * 0x2000), &output_data[0], 0x2000);
					}
					offset = (offset + 1) & 0x1FFF;
				}
			}
			bits >>= 1;
		}
	}

	if (output_pos >= 0)
	{
		memcpy(&actual_output_data[0] + (k++ * 0x2000), &output_data[0], output_pos);
	}

	size_t gggg = k * 0x2000 + output_pos;

	outputData.resize(gggg);

	memcpy(&outputData[0], &actual_output_data[0], gggg);

	actual_output_data.clear();
	actual_output_data.shrink_to_fit();
	output_data.clear();
	output_data.shrink_to_fit();

	/*delete output_data;
	delete actual_output_data;*/
}