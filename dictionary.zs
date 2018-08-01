// Example on how to skip entries in a list and only parse filtered data
package dictionary;

struct SkippableDictionary
{
    uint32      offsets[];
offsets[@index]:
    DictEntry   entries[];
};

struct DictEntry
{
    uint8       typeCode;
    uint8       value[];
};

struct DictOffsets
{
  uint32    offsets[];
};
