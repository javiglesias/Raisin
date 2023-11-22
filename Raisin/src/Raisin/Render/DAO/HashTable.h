#prama once
#include <cstdlib>

struct iHashTableItem
{
iHashTableItem() : key(""), value(0)
	{}
	char key[64];
	unsigned int value;
};
struct iHashTable
{
	iHashTableItem* table;
	unsigned int elements;
	void _Init(int _size)
	{
		table = (iHashTableItem*)malloc(_size);
		elements = 0;
	}
	void _Add(char* _key, unsigned int _value)
	{
		const iHashTableItem itm{ _key, _value };
		table[elements] = itm;
		++elements;
	}
	unsigned int _Find(char* _key)
	{
		
	}
};