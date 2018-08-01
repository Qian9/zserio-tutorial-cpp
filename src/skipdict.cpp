#include <skipdict/dictionary/SkippableDictionary.h>
#include <skipdict/dictionary/DictOffsets.h>
#include <skipdict/values/ValueCode.h>
#include <skipdict/values/RockBand.h>
#include <skipdict/values/Company.h>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	
	/* Write a skipable dictionary with some arbitrary content */
	skipdict::dictionary::SkippableDictionary dictionary;

	/* 1. Add a rock band */

	/* Add an entry to the dictionary */
	skipdict::dictionary::DictEntry entry;

	/* Generate a value code and store its value into the typeCode field of the dictionary */
	skipdict::values::ValueCode valueCode(skipdict::values::ValueCode::ROCK_BAND);
	entry.setTypeCode(valueCode.getValue());

	/* Generate the value itself and write to a buffer which we will store in dictionary value field. */
	skipdict::values::RockBand band;
	band.setName("Metallica");
	band.setMembers(4);
	band.setFans(16000000);

	/* Serialize to buffer */
	zserio::BitStreamWriter writer;
	band.write(writer);
	size_t size;
	const uint8_t* buffer = writer.getWriteBuffer(size);

	/* Copy serialized object into untyped uint8 array of dictionary entry */
	zserio::UInt8Array value;
	value.reserve(size);
	for (int i = 0; i < size; i++)
	{
		value.push_back(buffer[i]);
	}
	entry.setValue(value);

	/* Add entry to dictionary */

	zserio::ObjectArray<skipdict::dictionary::DictEntry> entries;
	entries.push_back(entry);

	/*==============================================================*/
	/* 2. Add a company */

	skipdict::dictionary::DictEntry entry2;

	skipdict::values::Company company;
	company.setName("ACME Inc.");
	company.setWebsite("zser.io");
	company.setNumEmployees(1000);

	zserio::BitStreamWriter company_writer;
	company.write(company_writer);
	buffer = company_writer.getWriteBuffer(size);

	/* Copy serialized object into untyped uint8 array of dictionary entry */
	
	zserio::UInt8Array value2;
	value2.reserve(size);
	for (int i = 0; i < size; i++)
	{
		value2.push_back(buffer[i]);
	}
	entry2.setValue(value2);
	
	/* Generate a value code and store its value into the typeCode field of the dictionary */
	skipdict::values::ValueCode valueCode2(skipdict::values::ValueCode::COMPANY);
	entry2.setTypeCode(valueCode2.getValue());

	/* Add entry to dictionary */
	entries.push_back(entry2);

	/* ======================================== */

	/* Set entries */
	dictionary.setEntries(entries);
	
	/* FIX: we have to manually set the offsets array before writing until issue is fixed in zserio */
	dictionary.getOffsets().resize(entries.size());
	
	/* Serialize dictionary and write to file */
	zserio::BitStreamWriter writer2;
	dictionary.write(writer2);

	buffer = writer2.getWriteBuffer(size);
	std::ofstream os("dict.zsb", std::ofstream::binary);
	os.write(reinterpret_cast<const char*>(buffer), size);
	os.close();


	/* Deserialize */

	/* Read the complete buffer from file */
	std::ifstream is("dict.zsb", std::ifstream::binary);
	is.seekg(0, is.end);
	size_t length = is.tellg();
	is.seekg(0, is.beg);
	char* in_buffer = new char[length];
	is.read(in_buffer, length);

	/* Declare the zserio BitStreamReader and assign the buffer */
	zserio::BitStreamReader reader((const uint8_t*)in_buffer, length);

	/* Get the offsets first, without deserializing the complete dictionary */

	skipdict::dictionary::DictOffsets offsets;

	offsets.read(reader);

	/* After we have read the offsets, iterate through them and check if we want to parse the content 
	 * Let's filter only for Companies, i.e. we skip the rock band in this example. */
	
	zserio::UInt32Array offset_list = offsets.getOffsets();
	for (int i = 0; i < offset_list.size(); i++)
	{
		/* Set the BitStreamReader to the correct offset */
		size_t bit_position = offset_list[i] * 8;
		reader.setBitPosition(bit_position);

		/* Read a Dict Entry from reader */
		skipdict::dictionary::DictEntry entry;
		entry.read(reader);

		if (entry.getTypeCode() == skipdict::values::ValueCode::COMPANY)
		{
			/* Get the untyped uint8 value array */
			zserio::UInt8Array value_array;
			value_array = entry.getValue();

			/* Since we are dealing with a uint8 array we can simply derive the length 
			 * needed for the BitStreamReader */
			size_t lenght = value_array.size();
			
			/* Pass the array to  the company parser */
			zserio::BitStreamReader value_reader(&value_array[0], length);
			skipdict::values::Company company;

			company.read(value_reader);

			/* Read out the deserialized company object */
			std::cout << "Company name: " << company.getName() << "\n";
			std::cout << "Company website: " << company.getWebsite() << "\n";
			std::cout << "Company employees: " << company.getNumEmployees() << "\n";
		}
		// move on to the next offset
	}


	return 0;
}