// Example on how to skip entries in a list and only parse filtered data
package filter;

struct SkipableDictionary
{
    uint32      offsets[];
offsets[@index]:
    DictEntry   entries[];

};

struct DictEntry
{
    varuint32   key;
    uint8       typeCode; // --> in app: map onto enum to be able to parse value[];
    uint8       value[];
};
