## Example: Skippable Dictionary

The following example generates a dictionary which can be partially parsed and entries can be skipped if not known to the application.

We will demonstrate the use of an indexed offset array which will hold different types of data.

For the example we have are using the following structs:
- Dictionary that holds arbitrary types and values
- Predefined structs that can be stored in the dictionary

### Running the tutorial

To be able to run the tutorial you need to add it to the build environment adding the CMake option `-DZSERIO_TUTORIAL_SKIPDICT=ON`

If you want to generate the zserio classes on your own (please note that we have already included them in the src/skipdict folder) you have to run the zserio compiler with an additional `-setTopLevelPackage skipdict` switch to put the two packages under a common package `skipdict`.

### Classic zserio modeling

In a strongly typed zserio schema you could design two packages, one for the dictionary and one for all the values. You would import the values und use them in the dictionary.

**Example:** dictionary.zs
```
package dictionary;

import values;

struct SkipableDictionary
{
    uint32      offsets[];
offsets[@index]:
    DictEntry   entries[];
};

struct DictEntry
{
    ValueCode   type;
    Value(type) value;
};
```
**Example**: values.zs
```
package values;

enum uint8 ValueCode
{
    COMPANY = 0,
    ROCK_BAND = 1,
    BASKETBALL_TEAM = 2,
    CLOUD_STORAGE = 3
};

choice Value(ValueCode code) on code
{
    case COMPANY:
          Company         business;
    case ROCK_BAND:
          RockBand        band;
    case BASKETBALL_TEAM:
          BasketballTeam  team;
    case CLOUD_STORAGE:
          CloudStorage    store;
};

struct Company
{
    string    name;
    string    website;
    varuint32 numEmployees;
};

struct RockBand
{
    string    name;
    bit:3     members;
    varuint64 fans;
};

[...]
```

The example above has two downsides
- it is not really skippable
- the import generates a dependency

##### Why are the items in the example not skippable even if it uses the offsets?

Reason for this is the design of a byte stream and how zserio works. We have to completely read a buffer with a dedicated schema to be able to correctly parse the content. So content of the  ` SkipableDictionary` can only be used once it is completely parsed. We indeed have offsets at the beginning, but there is currently no way in zserio to get only a part of a struct. We will work around this in this tutorial.

##### Why is it bad to have a dependency on the import?

Actually this is not really a bad thing and we could have made the tutorial completely with leaving in the dependency here.

But sometimes we may want to use a base structure like the dictionary with arbitrary content. That means we would need to always update the schema of the dictionary once we add another package of structs that we want to store inside. Of course one could argue that we just need to append to the `values` package, but this may generate one monolithic block which we need to maintain.

### zserio tricks to make it work

#### Skippability

Let's first look at what it means to be a skippable dictionary. Basically that means that we jump from byte offset to byte offset (i.e. go from entry to entry), check the type and then decide whether we *want* to parse it. It could also be a use case to be able to detect whether we are really *able* to parse the content.

To be able to jump from offset to offset, we only need the list of offsets. No need to go through the whole dictionary structure.

A smart way of doing that is to provide a "mask" which fits the first part of the dictionary so that we can use a BitStreamReader on this. This is why we define another struct which we only use for reading the offsets from the `SkippableDictionary`:
```
struct DictOffsets
{
  uint32    offsets[];
};
```
This represents the exact byte stream of the beginning of
```
struct SkipableDictionary
{
    uint32      offsets[];
offsets[@index]:
    DictEntry   entries[];
};
```

#### No dependencies

Next step is to deal with the dependencies. So we actually need to get rid of all imports. Once we removed the imports, we actually have to change the `DictEntry` structure from above. The types `ValueCode` and `Value` are not known anymore. We need to replace them with a basic data type for the value code that fits the size of the original enum (`uint8` in that case) and provide a byte buffer where we store the content. An arbitrary byte buffer can be modeled as a `uint8` array in zserio.

>Please note that we loose type safety here. That means the parser will not be able to detect a incorrectly filled entry any longer. If we need this strongly typed guarantee from the parser, then we have to work with the imports. We actively ignore that fact in this tutorial, knowing that we have to deal with this issue on the application level now.

So after replacing the value code and value with basic datatypes, the `DictEntry` structure looks like this:

```
struct DictEntry
{
    uint8       typeCode;
    uint8       value[];
};
```




#### Tasks for the application

- Mapping between type code field and enum values
- Quick code walkthrough
