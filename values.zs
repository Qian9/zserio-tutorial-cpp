package values;

enum uint8 ValueCode
{
    COMPANY = 0,
    ROCK_BAND = 1,
    BASKETBALL_TEAM = 2,
    CLOUD_STORAGE = 3
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

struct BasketballTeam
{
    bit:3     onField : onField <= 5;

    string    PlayersOnField[onField];
    string    PlayersOnBench[];
};

enum bit:3 CloudStorage
{
      TWO_GB =    0,
      FOUR_GB =   1,
      SIX_GB =    2,
      EIGHT_GB =  3
};
