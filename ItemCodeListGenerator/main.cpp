#include <Windows.h>
#include <Windowsx.h>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <fstream>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <Commctrl.h>
#include <Richedit.h>
#pragma comment(lib, "Comctl32.lib")

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "resource.h"

#pragma warning(disable: 4503)

using namespace std;

BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void SHOW_CONSOLE(bool show, bool noclose = false) {
	static bool show_state = false;
	if (show && !show_state) {
		std::cout.clear();
		FILE *stream;
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		freopen_s(&stream, "CONOUT$", "w", stdout);
		if (noclose)
			EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
	}
	if (!show)
		FreeConsole();
	show_state = show;
}
vector<string> split_str(string str, const string& delimiter, int minlen = -1) {
	vector<string> ret;
	UINT pos = 0;
	string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		if ((int)token.size() >= minlen)
			ret.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	if ((int)str.size() > 0)
		ret.push_back(str);
	return ret;
}
string int_to_str(int num, bool is_hex = false) {
	cout.clear();
	stringstream out;
	out << (is_hex ? hex : dec) << num;
	string return_value = (is_hex ? "0x" : "") + out.str();
	return return_value;
}
int str_to_int(string str) {
	cout.clear();
	stringstream out(str);
	int num;
	out >> (str.find("0x") != string::npos ? hex : dec) >> num;
	return num;
}
//LEAKS + warning (str_to_LPWSTR)
LPWSTR str_to_LPWSTR(string s) {
	LPWSTR m = new wchar_t[s.size() + 1];
	copy(s.begin(), s.end(), m);
	m[s.size()] = '\0';
	return m;
}
string wstr_to_str(wstring ws) {
	string ret(ws.begin(), ws.end());
	return ret;
}
wstring str_to_wstr(string s) {
	wstring ret(s.begin(), s.end());
	return ret;
}
string wastr_to_str(wchar_t *wc, UINT size) {
	char* ch = new char[size];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, size, &DefChar, NULL);
	string text(ch);
	delete[] ch;
	return text;
}
std::string getexedir() {
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	string dir = wstr_to_str(path);
	return dir.substr(0, dir.rfind('\\'));
}

string TransColor(int col) {
	if (col == 0) return "white";
	else if (col == 1) return "red";
	else if (col == 2) return "lightgreen";
	else if (col == 3) return "blue";
	else if (col == 4) return "darkgold";
	else if (col == 5) return "grey";
	else if (col == 6) return "black";
	else if (col == 7) return "gold";
	else if (col == 8) return "orange";
	else if (col == 9) return "yellow";
	else if (col == 10) return "darkgreen";
	else if (col == 11) return "purple";
	else if (col == 12) return "green";
	else if (col == 13) return "white2";
	else if (col == 14) return "black2";
	else if (col == 15) return "darkwhite";
	else if (col == 16) return "hide";
	return "";
}
int TransColor(string str) {
	if (str == "white") return 0;
	else if (str == "red") return 1;
	else if (str == "lightgreen") return 2;
	else if (str == "blue") return 3;
	else if (str == "darkgold") return 4;
	else if (str == "grey") return 5;
	else if (str == "black") return 6;
	else if (str == "gold") return 7;
	else if (str == "orange") return 8;
	else if (str == "yellow") return 9;
	else if (str == "darkgreen") return 10;
	else if (str == "purple") return 11;
	else if (str == "green") return 12;
	else if (str == "white2") return 13;
	else if (str == "black2") return 14;
	else if (str == "darkwhite") return 15;
	else if (str == "hide") return 16;
	return 0;
}
string TransQuality(int qual) {
	if (qual == 0) return "whatever";
	else if (qual == 1) return "low";
	else if (qual == 2) return "normal";
	else if (qual == 3) return "superior";
	else if (qual == 4) return "magic";
	else if (qual == 5) return "set";
	else if (qual == 6) return "rare";
	else if (qual == 7) return "unique";
	else if (qual == 8) return "crafted";
	else if (qual == 9) return "tempered";
	return "";
}
int TransQuality(string str) {
	if (str == "whatever") return 0;
	else if (str == "low") return 1;
	else if (str == "normal") return 2;
	else if (str == "superior") return 3;
	else if (str == "magic") return 4;
	else if (str == "set") return 5;
	else if (str == "rare") return 6;
	else if (str == "unique") return 7;
	else if (str == "crafted") return 8;
	else if (str == "tempered") return 9;
	return 0;
}
DWORD TransCode(const char* ptCode) // taken from afj666
{
	DWORD ItemCode = 0;

	if (strlen(ptCode) == 3) {
		char NewStr[5];
		sprintf_s(NewStr, 5, "%s ", ptCode);
		ItemCode = TransCode(NewStr);
	}
	else {
		ItemCode = (DWORD)(ptCode[3] << 24) + (DWORD)(ptCode[2] << 16) +
			(DWORD)(ptCode[1] << 8) + (DWORD)(ptCode[0]);
	}

	return ItemCode;
}
string TransCode(DWORD dwCode) {
	BYTE* aCode = (BYTE*)&dwCode;
	char ItemCode[5] = {0};

	ItemCode[0] = aCode[0];
	ItemCode[1] = aCode[1];
	ItemCode[2] = aCode[2];
	ItemCode[3] = aCode[3];
	ItemCode[4] = 0;

	return string(ItemCode);
}

struct ItemCode {
	std::string basetype;//weapon, armor...
	std::string subtype;//axe, shield...
	std::string type;//battle axe, buckler...
	std::string code;//btx, buc...
	int tier;//normal, exceptional, or elite, otherwise zero
};
struct ItemConfig {
	int Code;
	int	Quality;
	int Color;
	int Type;
};

LPARAM TreeView_GetItemParam(HWND hwnd, HTREEITEM htItem) {
	TVITEMW tvi = {0};
	tvi.hItem = htItem;
	tvi.mask = TVIF_PARAM;
	TreeView_GetItem(hwnd, &tvi);
	return tvi.lParam;
}
string TreeView_GetItemText(HWND hwnd, HTREEITEM htItem) {
	static const size_t len = 240;
	WCHAR buffer[len + 1];

	TVITEMW tvi = {0};
	tvi.hItem = htItem;
	tvi.mask = TVIF_TEXT;
	tvi.cchTextMax = len;
	tvi.pszText = &buffer[0];
	TreeView_GetItem(hwnd, &tvi);
	return wastr_to_str(tvi.pszText, len);
}
HTREEITEM TreeView_FindSibling(HWND hwnd, HTREEITEM start, string match) {
	HTREEITEM current = start;
	do {
		if (TreeView_GetItemText(hwnd, current) == match)
			return current;
	} while ((current = TreeView_GetNextSibling(hwnd, current)) != NULL);
	return NULL;
}
HTREEITEM TreeView_SetCheckStateForAllChildren(HWND tree, HTREEITEM hItem, BOOL checkstate) {
	HTREEITEM current = hItem;
	HTREEITEM sibling = NULL;
	while (current != NULL && sibling == NULL) {
		TreeView_SetCheckState(tree, current, checkstate);
		sibling = TreeView_SetCheckStateForAllChildren(tree, TreeView_GetChild(tree, current), checkstate);
		current = TreeView_GetNextSibling(tree, current);
	}
	return sibling;
}
void TreeView_SetCheckStateForAllParents(HWND tree, HTREEITEM child) {
	while (child != NULL) {
		int check = 0;
		int total = 1;
		HTREEITEM current = TreeView_GetChild(tree, TreeView_GetParent(tree, child));
		check = TreeView_GetCheckState(tree, current) ? check + 1 : check;
		while (current != NULL) {
			current = TreeView_GetNextSibling(tree, current);
			check = TreeView_GetCheckState(tree, current) ? check + 1 : check;
			total++;
		}
		TreeView_SetCheckState(tree, TreeView_GetParent(tree, child), total == check);

		child = TreeView_GetParent(tree, child);
	}
}
HTREEITEM getfilteritems(HWND tree, HTREEITEM hItem, vector<ItemCode*>* list) {
	HTREEITEM current = hItem;
	HTREEITEM child = NULL;
	while (current != NULL && child == NULL) {
		child = getfilteritems(tree, TreeView_GetChild(tree, current), list);
		if ((TreeView_GetChild(tree, current) == NULL) && (TreeView_GetCheckState(tree, current) == 1)) 
			list->push_back((ItemCode*)TreeView_GetItemParam(tree, current));
		current = TreeView_GetNextSibling(tree, current);
	}
	return child;
}

HWND g_hwnd;
string ConfigIni;

vector<ItemCode> g_itemcodes;
vector<ItemConfig> g_ItemArray;

void read_itemcodes() {
	g_itemcodes.clear();
	ifstream file;
	file.open("item codes.txt");
	if (file.is_open()) {
		while (file.good()) {
			string line;
			getline(file, line);
			while (line.find("!") == 0 && file.good()) {
				string basetype = line.substr(1, line.size() - 1);
				getline(file, line);
				while (line.find("@") == 0 && file.good()) {
					string subtype = line.substr(1, line.size() - 1);
					getline(file, line);
					while (line.size() > 0
						&& line[0] != '!'
						&& line[0] != '@'
						&& file.good()) {
						ItemCode code;
						code.basetype = basetype;
						code.subtype = subtype;
						vector<string> cod = split_str(line, ",");
						code.type = cod[0];
						code.code = cod[1];
						code.tier = str_to_int(cod[2]);
						g_itemcodes.push_back(code);
						getline(file, line);
					}
				}
			}
		}
	}
	else
		MessageBox(NULL, L"Unable to open item codes.txt", L"ERROR", MB_OK);

	file.close();
}
void LoadItemConfig() {
	char szConfig[200];

	for (int aNumber = 1;; aNumber++) {
		ostringstream szNumber;
		szNumber << aNumber;

		GetPrivateProfileStringA("Item Config", szNumber.str().c_str(), "", szConfig, 200, ConfigIni.c_str());
		if (szConfig[0] == '\0') { printf("%d item configuration entries have been loaded", aNumber - 1); break; }

		string Config(szConfig);

		string::size_type i = Config.find(',');
		if (i == string::npos) { printf("Error in configuration at line: %d", aNumber); continue; }
		string ItemCode(Config.substr(0, i));

		string::size_type i2 = Config.find(',', i + 1);
		if (i2 == string::npos) { printf("Error in configuration at line: %d", aNumber); continue; }
		string ItemQuality(Config.substr(i + 1, i2 - i - 1));

		string ItemColor(Config.substr(i2 + 1));

		boost::trim(ItemCode); boost::trim(ItemQuality); boost::trim(ItemColor);
		boost::to_lower(ItemCode); boost::to_lower(ItemQuality); boost::to_lower(ItemColor);

		ItemConfig hConfig;
		hConfig.Code = TransCode(ItemCode.c_str());
		hConfig.Quality = TransQuality(ItemQuality.c_str());
		hConfig.Color = TransColor(ItemColor.c_str());
		//Log("%x, %d, %d",hConfig.Code,hConfig.Quality,hConfig.Color);
		g_ItemArray.push_back(hConfig);
	}
}
void SaveItemConfig(int start = 1) {
	WritePrivateProfileStringA("Item Config", NULL, NULL, ConfigIni.c_str());//clear item config
	HWND edit = GetDlgItem(g_hwnd, IDC_RICHEDIT21);
	int len = GetWindowTextLength(edit);
	if (len == 0) return;
	wchar_t* wtext = new wchar_t[GetWindowTextLength(edit)];
	GetWindowText(edit, wtext, len);
	wstring wt = wstring(wtext);
	string text = wstr_to_str(wt);
	vector<string> lines = split_str(text, "\r\n");
	for (UINT i = 0; i < lines.size(); i++) {//write items back into config
		string code = lines[i].substr(lines[i].find('=') + 1, lines[i].find(',') - lines[i].find('=') - 1);
		string qual = lines[i].substr(lines[i].find(',') + 1, lines[i].rfind(',') - lines[i].find(',') - 1);
		string col = lines[i].substr(lines[i].rfind(',') + 1, string::npos);
		boost::trim(code); boost::trim(qual); boost::trim(col);
		boost::to_lower(code); boost::to_lower(qual); boost::to_lower(col);

		string configline = code + "," + qual + "," + col;
		WritePrivateProfileStringA("Item Config", boost::lexical_cast<string>(i + start).c_str(), configline.c_str(), ConfigIni.c_str());
	}
}

void PrintBaseFilterCodes(HWND tree, int start = 1) {
	SetWindowText(GetDlgItem(GetParent(tree), IDC_RICHEDIT21), L"");

	vector<ItemCode*> filter_itemtypes;
	int c = start;
	string output = "";

	output += int_to_str(c++) + "=dy1,whatever,purple\r\n";
	output += int_to_str(c++) + "=dy2,whatever,purple\r\n";
	output += int_to_str(c++) + "=dy3,whatever,purple\r\n";
	output += int_to_str(c++) + "=dy4,whatever,purple\r\n";
	output += int_to_str(c++) + "=dy5,whatever,purple\r\n";
	output += int_to_str(c++) + "=dy6,whatever,purple\r\n";
	output += int_to_str(c++) + "=dy7,whatever,purple\r\n";
	output += int_to_str(c++) + "=dy8,whatever,purple\r\n";
	
	vector<string> qualities = {"low", "normal", "superior", "magic", "set", "rare", "unique"};

	if (GetDlgCtrlID(tree) == IDC_ITEMCODEFILTERTREE) {
		HTREEITEM item = TreeView_GetRoot(tree);

		item = TreeView_GetChild(tree, item);
		getfilteritems(tree, TreeView_GetChild(tree, item), &filter_itemtypes);
		for (auto item : filter_itemtypes) {
			output += int_to_str(c++) + "=" + item->code + "," + qualities[0] + ",hide\r\n";
			//output += "\"" + item->code + "\",";
		}
		filter_itemtypes.clear();

		for (UINT i = 1; i < qualities.size(); i++) {
			item = TreeView_GetNextItem(tree, item, TVGN_NEXT);
			getfilteritems(tree, TreeView_GetChild(tree, item), &filter_itemtypes);
			for (auto item : filter_itemtypes) {
				output += int_to_str(c++) + "=" + item->code + "," + qualities[i] + ",hide\r\n";
			//	output += "\"" + item->code + "\",";
			}
			filter_itemtypes.clear();
		}
	}
	else if (GetDlgCtrlID(tree) == IDC_ITEMCODEFILTERTREE2) {
		
	}

	SetWindowText(GetDlgItem(GetParent(tree), IDC_RICHEDIT21), str_to_wstr(output).c_str());
}
HTREEITEM findcodeintree(HWND tree, HTREEITEM start, ItemCode* icode) {
	start = TreeView_FindSibling(tree, TreeView_GetChild(tree, start), icode->basetype);
	start = TreeView_FindSibling(tree, TreeView_GetChild(tree, start), icode->subtype);
	if (icode->tier)
		start = TreeView_FindSibling(tree, TreeView_GetChild(tree, start), icode->tier == 1 ? "Normal" : icode->tier == 2 ? "Exceptional" : icode->tier == 3 ? "Elite" : "");
	start = TreeView_FindSibling(tree, TreeView_GetChild(tree, start), icode->type);
	//start = TreeView_GetChild(tree, start);
	if (TreeView_GetItemParam(tree, start))
		return start;
	return NULL;
}
void InitTree(HWND tree, vector<string> &configlines) {
	TreeView_SetCheckStateForAllChildren(tree, TreeView_GetRoot(tree), FALSE);

	if (GetDlgCtrlID(tree) == IDC_ITEMCODEFILTERTREE) {
		for (auto line : configlines) {
			string code = line.substr(line.find('=') + 1, line.find(',') - line.find('=') - 1);
			string qual = line.substr(line.find(',') + 1, line.rfind(',') - line.find(',') - 1);
			string col = line.substr(line.rfind(',') + 1, string::npos);
			boost::trim(code); boost::trim(qual); boost::trim(col);
			boost::to_lower(code); boost::to_lower(qual); boost::to_lower(col);
			if (col != "hide") continue;

			HTREEITEM current = TreeView_GetRoot(tree);
			current = TreeView_GetChild(tree, current);
			do {
				string match = TreeView_GetItemText(tree, current);
				transform(match.begin(), match.end(), match.begin(), tolower);
				if (qual == match)
					break;
			} while ((current = TreeView_GetNextSibling(tree, current)) != NULL);
			if (!current) continue;

			ItemCode* item = 0;
			for (UINT i = 0; i < g_itemcodes.size(); i++) {
				if (g_itemcodes[i].code == code) {
					item = &g_itemcodes[i];
					break;
				}
			}
			if (!item) continue;

			ItemConfig ic;
			ic.Code = TransCode(code.c_str());
			ic.Quality = TransQuality(qual);
			ic.Color = 16;
			int pos = -1;
			for (UINT i = 0; i < g_ItemArray.size(); i++) {
				if (g_ItemArray[i].Code == ic.Code && g_ItemArray[i].Quality == ic.Quality) {
					if (g_ItemArray[i].Color == 16) {
						pos = (int)i;
						break;
					}
				}
			}
			if (pos == -1) g_ItemArray.push_back(ic);
			else g_ItemArray.erase(g_ItemArray.begin() + pos);

			HTREEITEM found = findcodeintree(tree, current, item);
			if (!found)
				printf("item not found in tree!\n");
			TreeView_SetCheckState(tree, found, TRUE);
			TreeView_SetCheckStateForAllParents(tree, found);
		}
		PostMessage(GetParent(tree), WM_COMMAND, IDC_BUTTON1, GetDlgCtrlID(tree));//PrintBaseFilterCodes(tree);
	}
	else if (GetDlgCtrlID(tree) == IDC_ITEMCODEFILTERTREE2) {

	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE/* hPrevInstance*/, LPSTR/* args*/, int/* iCmdShow*/) {
#ifdef _DEBUG
	SHOW_CONSOLE(true);
#endif
	ConfigIni = getexedir() + "\\D2Ex.ini";

	LoadItemConfig();
	read_itemcodes();

	LoadLibrary(L"riched20.dll");
	DialogBoxParam(GetModuleHandle(0), MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)DialogProc, 0);
}
BOOL CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_INITDIALOG:{
			g_hwnd = hwnd;

			InitCommonControls();

			vector<string> qualities = {"Low", "Normal", "Superior", "Magic", "Set", "Rare", "Unique"};

			map<string, map<string, map<int, map<string, ItemCode*>>>> item_types;//generates Warning: C4503
			for (UINT i = 0; i < g_itemcodes.size(); i++)
				item_types[g_itemcodes[i].basetype][g_itemcodes[i].subtype][g_itemcodes[i].tier][g_itemcodes[i].type] = &g_itemcodes[i];

			TV_INSERTSTRUCT tvinsert = {0};
			tvinsert.hParent = NULL;
			tvinsert.hInsertAfter = TVI_ROOT;
			tvinsert.item.pszText = L"All Items";
			tvinsert.item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
			tvinsert.item.stateMask = TVIS_STATEIMAGEMASK;
			tvinsert.hInsertAfter = TVI_LAST;
			HTREEITEM TRoot = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
			HTREEITEM TQuality;
			HTREEITEM TBasetype;
			HTREEITEM TSubtype;
			HTREEITEM TTier;
			for (auto quality : qualities) {
				tvinsert.hParent = TRoot;
				tvinsert.item.pszText = str_to_LPWSTR(quality);
				TQuality = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
				delete[] tvinsert.item.pszText;
				for (auto basetype : item_types) {
					tvinsert.hParent = TQuality;
					tvinsert.item.pszText = str_to_LPWSTR(basetype.first);
					TBasetype = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
					delete[] tvinsert.item.pszText;
					for (auto & subtype : basetype.second) {
						tvinsert.hParent = TBasetype;
						tvinsert.item.pszText = str_to_LPWSTR(subtype.first);
						TSubtype = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
						delete[] tvinsert.item.pszText;
						for (auto & tier : subtype.second) {
							tvinsert.hParent = TSubtype;
							tvinsert.item.pszText = tier.first == 1 ? L"Normal" : tier.first == 2 ? L"Exceptional" : tier.first == 3 ? L"Elite" : L"";
							TTier = tier.first > 0 ? (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert) : TSubtype;
							for (auto & type : tier.second) {
								tvinsert.hParent = TTier;
								tvinsert.item.pszText = str_to_LPWSTR(type.first);
								tvinsert.item.lParam = (LPARAM)type.second;
								SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
								delete[] tvinsert.item.pszText;
							}
							tvinsert.item.lParam = 0;
						}
					}
				}
			}
			
			/*tvinsert.hParent = NULL;
			tvinsert.hInsertAfter = TVI_ROOT;
			tvinsert.item.pszText = L"All Items";
			TRoot = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE2, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
			for (auto basetype : item_types) {
				tvinsert.hParent = TRoot;
				tvinsert.item.pszText = str_to_LPWSTR(basetype.first);
				TBasetype = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE2, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
				delete[] tvinsert.item.pszText;
				for (auto & tier : basetype.second) {
					tvinsert.hParent = TBasetype;
					tvinsert.item.pszText = (tier.first == 1 ? L"Normal" : tier.first == 2 ? L"Exceptional" : tier.first == 3 ? L"Elite" : L"");
					TTier = (tier.first > 0 ? (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE2, TVM_INSERTITEM, 0, (LPARAM)&tvinsert) : TBasetype);
					for (auto & subtype : tier.second) {
						tvinsert.hParent = TTier;
						tvinsert.item.pszText = str_to_LPWSTR(subtype.first);
						TSubtype = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE2, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
						delete[] tvinsert.item.pszText;
						for (auto & type : subtype.second) {
							tvinsert.hParent = TSubtype;
							tvinsert.item.pszText = str_to_LPWSTR(type.first);
							tvinsert.item.lParam = (LPARAM)type.second;
							TQuality = (HTREEITEM)SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE2, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
							delete[] tvinsert.item.pszText;
							for (auto quality : qualities) {
								tvinsert.hParent = TQuality;
								tvinsert.item.pszText = str_to_LPWSTR(quality);
								SendDlgItemMessage(hwnd, IDC_ITEMCODEFILTERTREE2, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
								delete[] tvinsert.item.pszText;
							}
						}
						tvinsert.item.lParam = 0;
					}
				}
			}*/

			PostMessage(hwnd, WM_COMMAND, IDC_BUTTON1, NULL);

			break;
		}
		case WM_COMMAND:{
			if (HIWORD(wParam) == EN_CHANGE) {
				if (LOWORD(wParam) == IDC_EDITSTARTVAL) {
					wchar_t text[10];
					GetWindowText(GetDlgItem(hwnd, LOWORD(wParam)), text, 10);
					string txt = wstr_to_str(text);
					int start;
					if (txt.size() == 0)
						start = 1;
					else
						start = str_to_int(txt);
					PrintBaseFilterCodes(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE), start);
					break;
				}
			}
			switch (wParam) {
				case IDC_RADIO1:{
					ShowWindow(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE), SW_SHOW);
					ShowWindow(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE2), SW_HIDE);

					vector<string>* lines;
					vector<string> temp;
					if ((HWND)lParam == GetDlgItem(hwnd, IDC_RADIO1)) {
						HWND edit = GetDlgItem(hwnd, IDC_RICHEDIT21);
						int len = GetWindowTextLength(edit);
						wchar_t* wtext = new wchar_t[GetWindowTextLength(edit)];
						GetWindowText(edit, wtext, len);
						wstring wt = wstring(wtext);
						delete[] wtext;
						string text = wstr_to_str(wt);
						temp = split_str(text, "\r\n");
						lines = &temp;
					}
					else
						lines = (vector<string>*)lParam;

					InitTree(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE), *lines);
					break;
				}
				case IDC_RADIO2:{
					ShowWindow(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE), SW_HIDE);
					ShowWindow(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE2), SW_SHOW);

					break;
				}
				case IDC_BUTTON1:{//hack
					if (lParam == NULL) {
						vector<string> lines;
						for (UINT i = 0; i < g_ItemArray.size(); i++) {
							string configline = TransCode(g_ItemArray[i].Code) + ", " + TransQuality(g_ItemArray[i].Quality) + ", " + TransColor(g_ItemArray[i].Color);
							lines.push_back(configline);
						}
						InitTree(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE), lines);

						auto t = [](HWND hwnd) {
							this_thread::sleep_for(chrono::milliseconds(100));
							SendMessage(hwnd, WM_COMMAND, IDC_RADIO1, (LPARAM)GetDlgItem(hwnd, IDC_RADIO1));
							PrintBaseFilterCodes(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE), 1);
						};
						thread b(t, hwnd);
						b.detach();
					}
					PrintBaseFilterCodes(GetDlgItem(hwnd, IDC_ITEMCODEFILTERTREE), 1);
					break;
				}
				case IDC_SAVEBTN:{
					SaveItemConfig();
					break;
				}
				case 10:{
					RECT* hack = (RECT*)lParam;
					TreeView_SetCheckStateForAllParents((HWND)hack->left, (HTREEITEM)hack->right);
					delete hack;
					break;
				}
			}
			break;
		}
		case WM_NOTIFY: {
			LPNMHDR lpnmh = (LPNMHDR)lParam;
			switch (lpnmh->idFrom) {
				case IDC_ITEMCODEFILTERTREE:
				case IDC_ITEMCODEFILTERTREE2:{
					switch (lpnmh->code) {
						case TVN_KEYDOWN:{
							LPNMTVKEYDOWN ptvkd = (LPNMTVKEYDOWN)lParam;
							if (ptvkd->wVKey == VK_SPACE) {
								HTREEITEM ht = TreeView_GetSelection(ptvkd->hdr.hwndFrom);
								TVITEM tvi;
								tvi.mask = TVIF_HANDLE | TVIF_STATE | TVIF_TEXT;
								tvi.hItem = (HTREEITEM)ht;
								tvi.stateMask = TVIS_STATEIMAGEMASK;
								TreeView_GetItem(ptvkd->hdr.hwndFrom, &tvi);

								TreeView_SetCheckStateForAllChildren(lpnmh->hwndFrom, TreeView_GetChild(lpnmh->hwndFrom, tvi.hItem), !TreeView_GetCheckState(lpnmh->hwndFrom, tvi.hItem));
								RECT* hack = new RECT;
								hack->left = (LONG)lpnmh->hwndFrom;
								hack->right = (LONG)tvi.hItem;
								PostMessage(hwnd, WM_COMMAND, 10, (LPARAM)hack);
								PostMessage(hwnd, WM_COMMAND, IDC_BUTTON1, lpnmh->idFrom);
							}
							return 0L;  // see the documentation for TVN_KEYDOWN
						}
						case NM_CLICK: {
							TVHITTESTINFO ht = {0};
							DWORD dwpos = GetMessagePos();
							ht.pt.x = GET_X_LPARAM(dwpos);
							ht.pt.y = GET_Y_LPARAM(dwpos);
							MapWindowPoints(HWND_DESKTOP, lpnmh->hwndFrom, &ht.pt, 1);
							TreeView_HitTest(lpnmh->hwndFrom, &ht);
							if (ht.flags & TVHT_ONITEMSTATEICON) {
								TVITEM tvi;
								tvi.mask = TVIF_HANDLE | TVIF_STATE | TVIF_TEXT;
								tvi.hItem = (HTREEITEM)ht.hItem;
								tvi.stateMask = TVIS_STATEIMAGEMASK;
								TreeView_SetCheckStateForAllChildren(lpnmh->hwndFrom, TreeView_GetChild(lpnmh->hwndFrom, tvi.hItem), !TreeView_GetCheckState(lpnmh->hwndFrom, tvi.hItem));
								RECT* hack = new RECT;
								hack->left = (LONG)lpnmh->hwndFrom;
								hack->right = (LONG)tvi.hItem;
								PostMessage(hwnd, WM_COMMAND, 10, (LPARAM)hack);
								PostMessage(hwnd, WM_COMMAND, IDC_BUTTON1, lpnmh->idFrom);
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
		case WM_CLOSE:
		case WM_DESTROY:{
			EndDialog(hwnd, 0);
			break;
		}
	}
	return FALSE;
}
