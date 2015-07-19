#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <map>

enum {
	STAT_CHARACTER_CLASS = -1,
	STAT_STRENGTH = 0,
	STAT_ENERGY = 1,
	STAT_DEXTERITY = 2,
	STAT_VITALITY = 3,
	STAT_CURRENT_LIFE = 6,
	STAT_TOTAL_LIFE = 7,
	STAT_CURRENT_MANA = 8,
	STAT_TOTAL_MANA = 9,
	STAT_CURRENT_STAMINA = 10,
	STAT_TOTAL_STAMINA = 11,
	STAT_PLAYER_LEVEL = 12,
	STAT_EXPERIENCE = 13,
	STAT_GOLD_IN_STASH = 15,
	STAT_ATTACK_RATING = 19,
	STAT_BLOCK_CHANCE = 20,
	STAT_DEFENSE = 31,
	STAT_DAMAGE_REDUCE = 34,
	STAT_MAGIC_DAMAGE_REDUCE = 35,
	STAT_DAMAGE_REDUCE_PERCENT = 36,
	STAT_MAGIC_RESIST = 37,
	STAT_MAX_MAGIC_RESIST = 38,
	STAT_FIRE_RESIST = 39,
	STAT_MAX_FIRE_RESIST = 40,
	STAT_LIGHTNING_RESIST = 41,
	STAT_MAX_LIGHTNING_RESIST = 42,
	STAT_COLD_RESIST = 43,
	STAT_MAX_COLD_RESIST = 44,
	STAT_POISON_RESIST = 45,
	STAT_MAX_POISON_RESIST = 46,
	STAT_LIFE_REPLENISH = 74,
	STAT_MAGIC_FIND = 80,
	STAT_EXPERIENCE_GAIN = 85,
	STAT_INCREASED_ATTACK_SPEED = 93,
	STAT_FASTER_RUN_WALK = 96,
	STAT_FASTER_HIT_RECOVER = 99,
	STAT_FASTER_CAST_RATE = 105,
	STAT_POISON_LENGTH_REDUCE = 110,
	STAT_OPEN_WOUNDS = 135,
	STAT_CRUSHING_BLOW = 136,
	STAT_DEADLY_STRIKE = 141,
	STAT_FIRE_ABSORB_PERCENT = 142,
	STAT_FIRE_ABSORB = 143,
	STAT_LIGHTNING_ABSORB_PERCENT = 144,
	STAT_LIGHTNING_ABSORB = 145,
	STAT_MAGIC_ABOSRB_PERCENT = 146,
	STAT_MAGIC_ABSORB = 147,
	STAT_COLD_ABSORB_PERCENT = 148,
	STAT_COLD_ABSORB = 149,
};

class CharacterStats {
	std::map<int, std::pair<int, std::string>> stats;

	public:
	void addStat(int id, int val, std::string str) {
		stats[id] = std::make_pair(val, str);
	}
	std::string getStatText(int id) {
		return stats[id].second;
	}
	int getStat(int id) {
		return stats[id].first;
	}
	RECT getStatInvPos(int id) {
		RECT ret = {0, 0, 0, 0};
		if (id == 0) ret = {77, 83, 114, 98};
		else if (id == STAT_ENERGY) ret = {77, 293, 114, 308};
		else if (id == STAT_DEXTERITY) ret = {77, 145, 114, 160};
		else if (id == STAT_VITALITY) ret = {77, 231, 114, 246};
		else if (id == STAT_CURRENT_LIFE) ret = {273, 256, 309, 270};
		else if (id == STAT_TOTAL_LIFE) ret = {232, 256, 268, 270};
		else if (id == STAT_CURRENT_MANA) ret = {273, 294, 309, 308};
		else if (id == STAT_TOTAL_MANA) ret = {232, 294, 268, 308};
		else if (id == STAT_CURRENT_STAMINA) ret = {273, 232, 309, 246};
		else if (id == STAT_TOTAL_STAMINA) ret = {232, 232, 268, 246};
		else if (id == STAT_PLAYER_LEVEL) ret = {12, 44, 53, 64};
		else if (id == STAT_EXPERIENCE) ret = {66, 44, 181, 64};
		else if (id == STAT_DEFENSE) ret = {272, 194, 309, 208};
		else if (id == STAT_FIRE_RESIST) ret = {272, 333, 309, 347};
		else if (id == STAT_LIGHTNING_RESIST) ret = {272, 381, 309, 395};
		else if (id == STAT_COLD_RESIST) ret = {272, 357, 309, 371};
		else if (id == STAT_POISON_RESIST) ret = {272, 405, 309, 419};
		return ret;
	}
	std::string getCharacterClassStr() {
		switch (stats[-1].first) {
			case 0: return "Amazon";
			case 1: return "Sorceress";
			case 2: return "Necromancer";
			case 3: return "Paladin";
			case 4: return "Barbarian";
			case 5: return "Druid";
			case 6: return "Assassin";
		}
		return "";
	}
};
struct InvData {
	int x;
	int y;
	int w;
	int h;
	std::string inv;
	std::string invu;
	std::string invs;
};
bool operator==(const InvData& lhs, const InvData& rhs);
struct ItemCode {
	std::string basetype;//weapon, armor...
	std::string subtype;//axe, shield...
	std::string type;//battle axe, buckler...
	std::string code;//btx, buc...
	int tier;//normal, exceptional, or elite, otherwise zero
};
struct ItemData {
	std::string name;
	std::string quality;
	ItemCode icode;
	InvData invdata;
	std::vector<std::string> basestats;
	std::vector<std::string> stats;

	std::string realm;
	std::string account;
	std::string character;
	std::string store;

	int trade_depth = 3;
	bool is_trade = false;
};
bool operator==(const ItemData& lhs, const ItemData& rhs);
enum {
	CMP_ACCOUNT = 1,
	CMP_CHARACTER = 3,
	CMP_STORE = 7,
	CMP_INVPOS = 15,
};
bool itemcmp(const ItemData& lhs, const ItemData& rhs, int cmp_depth = CMP_INVPOS);

class CharacterData {
	public:
	std::string realm;
	std::string account;
	std::string character;
	CharacterStats character_stats;
	std::vector<ItemData> cube_items;
	std::vector<ItemData> body_items;
	std::vector<ItemData> stash_items;
	std::vector<ItemData> merc_items;
	std::vector<ItemData> inv_items;

	std::string wp_norm;
	std::string wp_night;
	std::string wp_hell;

	std::string quest_norm;
	std::string quest_night;
	std::string quest_hell;

	UINT getItemsCount();
	ItemData* getItem(UINT pos);
};

void getItemDataFromCode(std::string code, ItemData* item, bool gettype = false);

struct GameStructInfo {
	BYTE _1[0x1B];					//0x00
	char szGameName[0x18];			//0x1B
	char szGameServerIp[0x56];		//0x33
	char szAccountName[0x30];		//0x89
	char szCharName[0x18];			//0xB9
	char szRealmName[0x18];			//0xD1
	BYTE _2[0x158];					//0xE9
	char szGamePassword[0x18];		//0x241
};

std::vector<ItemData>* get_item_store(std::string realm, std::string account, std::string character, std::string store);
std::vector<ItemData>* get_item_store(const ItemData* item_in_store);
CharacterData* get_character(std::string realm, std::string account, std::string character);
CharacterData* get_character(const ItemData* item);
