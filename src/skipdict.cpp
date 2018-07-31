#include <skipdict\filter\SkipableDictionary.h>
#include <skipdict\values\Value.h>
#include <fstream>

int main(int argc, char** argv)
{
	
	/* Write a skipable dictionary with some arbitrary content */
	skipdict::filter::SkipableDictionary dictionary;

	/* Add a rock band */

	/* Add an entry to the dictionary */
	skipdict::filter::DictEntry entry;
	/* set an arbitrary key */
	entry.setKey(12);

	/* Generate a value code and store its value into the typeCode field of the dictionary */
	skipdict::values::ValueCode valueCode(skipdict::values::ValueCode::ROCK_BAND);
	entry.setTypeCode(valueCode.getValue());

	/* Generate the value itself and write to a buffer which we will store in dictionary value field. */
	skipdict::values::RockBand band;
	band.setName("Metallica");
	band.setMembers(4);
	band.setFans(16000000);

	zserio::BitStreamWriter writer;
	band.write(writer);

	size_t size;
	const uint8_t* buffer = writer.getWriteBuffer(size);

	zserio::UInt8Array value;
	value.reserve(size);
	for (int i = 0; i < size; i++)
	{
		value.push_back(buffer[i]);
	}

	entry.setValue(value);

	zserio::ObjectArray<skipdict::filter::DictEntry> entries;

	entries.push_back(entry);

	dictionary.setEntries(entries);

	/* Write to file */
	zserio::BitStreamWriter	writer2;
	dictionary.write(writer2);

	buffer = writer2.getWriteBuffer(size);
	/* Write the buffer to disk */
	std::ofstream os("dict.zsb", std::ofstream::binary);
	os.write(reinterpret_cast<const char*>(buffer), size);
	os.close();

	return 0;
}