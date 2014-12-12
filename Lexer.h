//===-----------------------------------------------------------===//
// Лексический анализатор 2012г.
// Автор : Кабылин Андрей Николаевич (sys.int64@gmail.com)
//===-----------------------------------------------------------===//

#ifndef H_LEXER
#define H_LEXER 1

#include "Def.h"
#include <string>

using namespace std;

// Токены
enum token { // Last -95
	tok_eof = -1, tok_none = 0,
	//
	tok_id = -2, tok_double = -3, tok_number = -4,
	tok_string = -5, tok_char = -33, tok_type = -42,
	tok_const = -43, tok_vector = -44, tok_proc = -45,
	tok_out = -46, tok_asm = -47, tok_case = -49,
	tok_local = -50, tok_break = -51, tok_continue = -52,
	tok_exit = -53, tok_args = -68, tok_set = -77,
	
	//
	tok_cc = -54, tok_linkage = -55, tok_attr = -56,
	tok_template = -57, tok_oper = -73, tok_binary = -74,
	tok_unary = -75, tok_inherited = -76, tok_sizeof = -83,
	//
	tok_interface = -78, tok_implementation = -79,
	tok_nil = -80, tok_constructor = 72, tok_destructor = -82,
	
	//
	tok_record = -58, tok_packed = -59, tok_class = -60,
	tok_private = -61, tok_public = -62, tok_protected = -63,
	
	//
	tok_uses = -6, tok_var = -7, tok_array = -8, tok_new = -93,
	tok_func = -9, tok_extern = -10, tok_return = -90, tok_delete = -94,
	tok_begin = -11, tok_end = -12, tok_elsif = -89, tok_include = -95,
	tok_assign = -13, tok_and = -14, tok_or = -15,
	
	//
	tok_as = -16, tok_is = -17, tok_in = -18,
	tok_to = -19, tok_do = -20, tok_not = -21,
	tok_of = -22, tok_downto = -48, tok_on = -86,
	
	//
	tok_if    = -23, tok_then  = -24, tok_else   = -25,
	tok_for   = -26, tok_while = -27, tok_repeat = -28,
	tok_until = -29, tok_with  = -81, tok_cast = -87,
	
	//
	tok_le = -30, tok_ge = -31, tok_ne = -32, tok_eq = -88,
	//
	tok_true = -34, tok_false = -35, tok_alias = -91,
	tok_div = -36, tok_mod = -37, tok_xor = -64,
	tok_shl = -65, tok_shr = -66, tok_module = -92,
	//
	tok_align = -67, tok_goto = -69, tok_label = -70,
	tok_range = -71, tok_self = -84, tok_assignobj = -85
};

struct tok {
	int token;
	string idstr;
	long double num;
} typedef tok;

// Hex To Val
static uint64_t HexIntToVal(const char *Buffer) {
	uint64_t Result = 0;
	
	for (; *Buffer; ++Buffer) {
		uint64_t OldRes = Result;
		Result *= 16;
		char C = *Buffer;
		
		if (C >= '0' && C <= '9')
			Result += C-'0';
		else if (C >= 'A' && C <= 'F')
			Result += C-'A'+10;
		else if (C >= 'a' && C <= 'f')
			Result += C-'a'+10;
		
	//if (Result < OldRes)   // Uh, oh, overflow detected!!!
	//  ThrowException("constant bigger than 64 bits detected!");
	}
  
	return Result;
}

//
// Перевод символа в нижний регистр
char LowerCase(const char c) {
	if (c >= 'A' && c <= 'Z') {
		return c+32;
	} else return c;
}
	
// Перевод строки в нижний регистр
string LowerCaseS(const string str) {
	string s;
	
	for (int i = 0; i < str.length(); i++)
		s += LowerCase(str.c_str()[i]);
	
	return s;
}

bool exists (const string &fn) {
    struct stat st;
    int result = stat(fn.c_str(), &st);
    return result == 0;
}

// Lexer - Лексический анализатор
class lexer {
public:
	FILE *InFile; // Входной поток

	string		IdentifierStr;		// Заполняется, если tok_identifier
	string		IdentifierStrReg;	// Заполняется, если tok_identifier с учетом регистра
	string		StringConst;
	long double	NumVal;		// Заполняется, если tok_number
	int		CurToken;	// Текущий токен
	int		StrLen;
	vector<tok>	TokenStack;
	vector<int>	fOffsetStack;
	int		nts;
	int		pos, line;
	string  fileName;
	bool EndLine;

	// 
	int GetChar();
	int GetToken(bool io = false);
	int GetNextToken(bool io = false) {
		//
		if (nts < TokenStack.size()) {
			nts++;
			PopToken();
			
			return TokenStack[nts].token;
		} else {
			int Tok = GetToken(io);
			CurToken = Tok;
			PushToken(); // push to stack
			nts = TokenStack.size();
			return Tok;
		}

	}
	
	void PushToken() {
		tok Tok;
		Tok.token = CurToken;
		Tok.idstr = IdentifierStr;
		Tok.num   = NumVal;
		
		TokenStack.push_back(Tok);
	}
	
	void PopToken() {
		CurToken      = TokenStack[nts-1].token;
		IdentifierStr = TokenStack[nts-1].idstr;
		NumVal        = TokenStack[nts-1].num;
	}
	
	int GetPrevToken() {
		nts--;
		PopToken();
		
		return TokenStack[nts-1].token;
	}
	
	int GetStrLen() {
		int s = StrLen;
		StrLen = 0;
		
		return s;
	}

	FILE *macro (string FileName) {

		if (!exists(FileName)) {
			Error<int>(include_file_s_not_found, FileName.c_str());
			exit(1);
		}

		fileName = FileName;
		return fopen(FileName.c_str(), "r");

		#ifdef _WIN32
			string cmd = "bin\\macro.exe";
		#else
			string cmd = "./bin/macro";
		#endif

		if (system((cmd+" "+FileName+" "+MacroFlags+" -o ."+FileName).c_str()) == -1) {
			exit(1);
		}

		return fopen(("."+FileName).c_str(), "r");
	}
	
	lexer() {
		InFile = macro (FileName);
		FileStack.push_back(InFile);
		fOffsetStack.push_back(0);
		StrLen = 0; nts = TokenStack.size();
		pos = 1; line = 1;
	}
	 
	~lexer() { fclose(InFile); }

	void OpenFile (const char *fn) {
		fclose(InFile);
		InFile = macro (fn);

		//InFile = fopen(("."+fileName).c_str(), "r");
		fOffsetStack.push_back(0);
	}

	void ReOpenFile (const char *fn) {
		fclose(InFile);
		InFile = macro (fn);
		fOffsetStack.pop_back();
		char *buf[1000];
		fread(&buf, sizeof(char), fOffsetStack[fOffsetStack.size()-1], InFile);
	}
};

int lexer::GetChar() {
	static int buf;
	EndLine = false;
	if (feof(InFile)) return EOF;
	
	size_t a = fread(&buf, sizeof(char), 1, InFile);
	fOffsetStack[fOffsetStack.size()-1]++;
	pos++;

	if (buf == '\r') {
		EndLine = true;
		size_t a = fread(&buf, sizeof(char), 1, InFile);
		fOffsetStack[fOffsetStack.size()-1]++;

		pos++;
		
		if (buf == '\n') {
			EndLine = true;
			line++;
			pos = 1;
		}
	} else if (buf == '\n') {
		EndLine = true;
		line++;
		pos = 1;
	}
	
	return buf;
}

// GetToken - Возвращает следующий токен из потока
int lexer::GetToken(bool io) {
	static int LastChar = ' ';
	
	// Skip spaces
	while (isspace(LastChar))
		LastChar = GetChar();

	// Проверка конца файла.
	if (feof(InFile)) {
		return tok_eof; }
	
	// Identifier: [a-zA-Z_][a-zA-Z0-9_]*
	if (isalpha(LastChar) || LastChar == '_') {
		IdentifierStr = LastChar;
		
		while (1) {
			LastChar = GetChar();
			
			if (isdigit(LastChar) || isalpha(LastChar) || LastChar == '_') {
				IdentifierStr += LastChar;
				continue;
			}
			
			break;
		}
		
		IdentifierStrReg = IdentifierStr;
		//IdentifierStr = LowerCaseS(IdentifierStr);
		
		//
		if (IdentifierStr == "uses")	 		return tok_uses;
		if (IdentifierStr == "var") 	 		return tok_var;
		if (IdentifierStr == "local")  			return tok_local;
		if (IdentifierStr == "type") 			return tok_type;
		if (IdentifierStr == "const")	 		return tok_const;
		if (IdentifierStr == "out") 	 		return tok_out;
		if (IdentifierStr == "label") 			return tok_label;
		if (IdentifierStr == "break")			return tok_break;
		if (IdentifierStr == "continue")		return tok_continue;
		if (IdentifierStr == "exit")			return tok_exit;
		if (IdentifierStr == "struct")			return tok_record;
		if (IdentifierStr == "class")			return tok_class;
		if (IdentifierStr == "sizeof")			return tok_sizeof;
		
		//
		if (IdentifierStr == "public")			return tok_public;
		if (IdentifierStr == "private")			return tok_private;
		if (IdentifierStr == "protected")		return tok_protected;
		
		//
		if (IdentifierStr == "as")				return tok_as;
		if (IdentifierStr == "is")				return tok_is;
		if (IdentifierStr == "not")				return tok_not;
		if (IdentifierStr == "in")				return tok_in;
		if (IdentifierStr == "do")				return tok_do;
		if (IdentifierStr == "of")				return tok_of;
		if (IdentifierStr == "to")				return tok_to;
		if (IdentifierStr == "on")				return tok_on;
		if (IdentifierStr == "downto")			return tok_downto;
		
		//
		if (IdentifierStr == "begin")			return tok_begin;
		if (IdentifierStr == "end")				return tok_end;
		
		//
		if (IdentifierStr == "if")				return tok_if;
		if (IdentifierStr == "then")			return tok_then;
		if (IdentifierStr == "else")	 		return tok_else;
		if (IdentifierStr == "elsif")			return tok_elsif;
		//
		if (IdentifierStr == "case")			return tok_case;
		//
		//if (IdentifierStr == "and")				return tok_and;
		//if (IdentifierStr == "or")				return tok_or;
		if (IdentifierStr == "shl")				return tok_shl;
		if (IdentifierStr == "shr")				return tok_shr;
		if (IdentifierStr == "xor")				return tok_xor;
		if (IdentifierStr == "div")				return tok_div;
		if (IdentifierStr == "mod")				return tok_mod;
		//
		if (IdentifierStr == "true")	 		return tok_true;
		if (IdentifierStr == "false")			return tok_false;
		//
		if (IdentifierStr == "while")			return tok_while;
		if (IdentifierStr == "for")	 			return tok_for;
		if (IdentifierStr == "repeat")			return tok_repeat;
		if (IdentifierStr == "until")			return tok_until;
		if (IdentifierStr == "with")			return tok_with;
		//	
		if (IdentifierStr == "procedure")		return tok_proc;
		if (IdentifierStr == "def")				return tok_func;
		if (IdentifierStr == "constructor")		return tok_constructor;
		if (IdentifierStr == "destructor")		return tok_destructor;
		if (IdentifierStr == "oper")			return tok_oper;
		if (IdentifierStr == "binary")			return tok_binary;
		if (IdentifierStr == "unary")			return tok_unary;
		if (IdentifierStr == "extern")			return tok_extern;
		if (IdentifierStr == "inherited")		return tok_inherited;
		if (IdentifierStr == "return")			return tok_return;
		if (IdentifierStr == "include")			return tok_include;
		if (IdentifierStr == "cast")			return tok_cast;
		
		// Call Conv
		if (IdentifierStr == "fastcc")			return tok_cc;
		if (IdentifierStr == "ccc")				return tok_cc;
		if (IdentifierStr == "coldcc")			return tok_cc;
		if (IdentifierStr == "cc")				return tok_cc;
		if (IdentifierStr == "fastcall")		{ IdentifierStr = "x86_fastcallcc"; return tok_cc; }
		if (IdentifierStr == "stdcall")			{ IdentifierStr = "x86_stdcallcc";  return tok_cc; }
		if (IdentifierStr == "cdecl")			{ IdentifierStr = "";               return tok_cc; }
		//
		if (IdentifierStr == "overload")		{ IdentifierStr = "overload";     return tok_attr; }
		if (IdentifierStr == "inline")			{ IdentifierStr = "inlinehint";   return tok_attr; }
		if (IdentifierStr == "alwaysinline")	{ IdentifierStr = "alwaysinline"; return tok_attr; }
		if (IdentifierStr == "noinline")		return tok_attr;
		if (IdentifierStr == "override")		return tok_attr;
		if (IdentifierStr == "virtual")			return tok_attr;
		if (IdentifierStr == "abstract")		return tok_attr;
		if (IdentifierStr == "readnone")		return tok_attr;
		if (IdentifierStr == "readonly")		return tok_attr;
		if (IdentifierStr == "addresssafety")	return tok_attr;
		if (IdentifierStr == "alignstack")		return tok_attr;
		if (IdentifierStr == "swap")			return tok_attr; 
		//
		if (IdentifierStr == "this")			return tok_self;
		if (IdentifierStr == "alias")			return tok_alias;
		if (IdentifierStr == "module")			return tok_module;
		
		// Linkage
		if (IdentifierStr == "external")		return tok_linkage;
		//
		if (IdentifierStr == "template")		return tok_template;
		if (IdentifierStr == "align")			return tok_align;
		if (IdentifierStr == "goto")			return tok_goto;
		//
		if (IdentifierStr == "interface")		return tok_interface;
		if (IdentifierStr == "implementation")	return tok_implementation;
		//
		if (IdentifierStr == "nil")				return tok_nil;
		if (IdentifierStr == "packed")			return tok_packed;
		if (IdentifierStr == "new")				return tok_new;
		if (IdentifierStr == "delete")			return tok_delete;

		// Простой идентификатор
		return tok_id;
	}
	
	// оператор '...'
	if (LastChar == '.') {
		int ThisChar = LastChar;
		LastChar = GetChar();

		if (LastChar == '.') {
			int ThisChar = LastChar;
			LastChar = GetChar();
			
			if (LastChar == '.') {
				LastChar = GetChar();
				return tok_args;
			}
			
			return tok_range;
		}
		
		return ThisChar;
	}
	
	// Символ #num
	bool ch = false;
	
	if (LastChar == '#') {
		ch = true;
		LastChar = GetChar();
	}
	
	int ThisTok;
	
	/*if (LastChar == '0') {
		LastChar = GetChar();

		if (LastChar == 'x') {
			//
			LastChar = GetChar();
			//
			string NumStr = "";
			LastChar = LowerCase(GetChar());
			
			while ((LastChar >= '0' && LastChar <= '9') || (LastChar >= 'a' && LastChar <= 'f')) {
				NumStr += LastChar;
				LastChar = LowerCase(GetChar());
			}
			
			
			NumVal = HexIntToVal(NumStr.c_str());
			return tok_number;
		}
		//
		LastChar = '0';
	}*/

	// Число Double || Integer: [0-9] (.[0-9]+)? (E[+-]? [0-9]+)?
	if (isdigit(LastChar)) {
		int stat = 2;
		
		string NumStr;
		string ScaleStr;
		int ScaleFactor = 0;
		
		//
		while (stat < 8) { // Строим конечный автормат
			switch (stat) {
				case 1 : // Hex val
					{
						//
						LastChar = GetChar();
						//
						string NumStr = "";
						LastChar = LowerCase(GetChar());
						
						while ((LastChar >= '0' && LastChar <= '9') || (LastChar >= 'a' && LastChar <= 'f')) {
							NumStr += LastChar;
							LastChar = LowerCase(GetChar());
						}
						
						
						NumVal = HexIntToVal(NumStr.c_str());
						return tok_number;
					}

				case 2 :
					NumStr += LastChar;
					LastChar = GetChar();

					if (LastChar == 'x') {
						stat = 1;
					} else if (isdigit(LastChar)) {
						stat = 2;
					} else if (LastChar == '.') {
						if (io) stat = 9; else stat = 3;
					} else if (LastChar == 'E' || LastChar == 'e') {
						stat = 5;
					} else stat = 9; // Завершающее состояние
					
					break;
					
				case 3 :
					NumStr += LastChar;
					LastChar = GetChar();
					
					if (isdigit(LastChar)) {
						stat = 4;
					} else stat = 9; // Ошибка
					
					break;
					
				case 4 :
					NumStr += LastChar;
					LastChar = GetChar();
						
					if (isdigit(LastChar)) {
						stat = 4;
					} else if (LastChar == 'E' || LastChar == 'e') {
						stat = 5;
					} else stat = 10; // Завершающее состояние
					
					break;
					
				case 5 :
					LastChar = GetChar();
					
					if (LastChar == '+' || LastChar == '-') {
						stat = 6;
					} else if (isdigit(LastChar)) {
						stat = 7;
					} else ; // Ошибка
					
					break;
					
				case 6 :
					ScaleStr += LastChar;
					LastChar = GetChar();
					
					if (isdigit(LastChar)) {
						stat = 7;
					} else ; // Ошибка
						
					break;
					
				case 7 :
					ScaleStr += LastChar;
					LastChar = GetChar();
					
					if (isdigit(LastChar)) {
						stat = 7;
					} else stat = 8; // Завершающее состояние
					
					break;
					
				default : ;
			}
		} // Конечный Автомат
			
		NumVal = strtod(NumStr.c_str(), 0);
		
		if (ScaleStr != "") ScaleFactor = strtod(ScaleStr.c_str(), 0);
		
		NumVal = NumVal*pow(10, ScaleFactor);
		//printf("%d\n", ScaleFactor);
		
		//Trace(NumStr, ltrace);
		//printf("Lexer out tok_number : %lld\n", NumVal);
		
		if (!ch) return tok_number;
		else ThisTok = tok_char;
	}

	//
	if (LastChar == '\'') {
		LastChar = GetChar();
		char ch = LastChar;
		LastChar = GetChar();

		if (LastChar == '\'') {
			//LastChar = ch;
			
		}

		LastChar = GetChar();
		NumVal = ch;
		return tok_char;
	}
	
	// Символ | Строка
	if (LastChar == '\"') {
		StrLen = 0;
		string StrVal;
		StringConst = "";
		int State = 0;
		
		if (ThisTok == tok_char) {
			
		}
		//
		while (State != 3) {
			switch (State) {
				case 0 :
				case 1 :
					do {
						LastChar = GetChar();
						if (LastChar != '\"') {
							//if (LastChar == '\\") IdentifierStr += "\\"+IntToStr(LastChar);
							/*else*/ StringConst += LastChar;
							
							StrLen++;
						}
					} while (LastChar != '\"' && LastChar != EOF);
		
					if (LastChar == EOF) {
						printf("Unknown \" in string const\n");
					} else LastChar = GetChar();

					if (LastChar == '#')  { State = 2; break; }
					if (LastChar == '\"') { StringConst += '\"'; State = 1; break; }
					/*if (LastChar == '+')  { 
						State = 0;
						LastChar = GetChar();
						
						if (LastChar != '\"') {
							LastChar = '+';
							State = 3;
						}
						
						break;
					}*/

					State = 3;
					break;

				case 2 :
					StrLen++;
					string NumStr = "";
					LastChar = GetChar();

					while (isdigit(LastChar)) {
						NumStr += LastChar;//
						LastChar = GetChar();
					} //

					//LastChar = GetChar();
					int c = strtod(NumStr.c_str(), 0);
					char buff[10];
					
					if (c > 255) { /*error*/ c = 255; }
					
					sprintf(buff, "%.2X\0", c);
					//printf("%s, %d\n", buff);
					string ss = buff;
					StringConst += "\\"+ss;

					if (LastChar == '\"') { State = 1; break; }
					if (LastChar == '#')  { State = 2; break; }
					//if (LastChar == '+')  { State = 0; LastChar = GetChar(); break; }

					State = 3;
					break;
			} // switch
		} // while

		return tok_string;
	}
	
	if (ThisTok == tok_char) return ThisTok;
	
	// ==
	if (LastChar == '=') {
		int ThisChar = LastChar;
		LastChar = GetChar();
		
		if (LastChar == '=') {
			LastChar = GetChar();
			return tok_eq;
		}
		
		return ThisChar;
	}

	// ==
	/*if (LastChar == '=') {
		int ThisChar = LastChar;
		LastChar = GetChar();
		
		if (LastChar == '=') {
			LastChar = GetChar();
			return tok_eq;
		}
		
		return ThisChar;
	}*/

	// !=
	if (LastChar == '!') {
		int ThisChar = LastChar;
		LastChar = GetChar();
		
		if (LastChar == '=') {
			LastChar = GetChar();
			return tok_ne;
		}
		
		return tok_not;
	}

	// !=
	if (LastChar == '~') {
		int ThisChar = LastChar;
		LastChar = GetChar();
		return tok_not;
	}

	// || logic or
	if (LastChar == '|') {
		int ThisChar = LastChar;
		LastChar = GetChar();

		if (LastChar == '|') {
			LastChar = GetChar();
			return tok_or;
		}
		
		return ThisChar;
	}

	// && logic and
	if (LastChar == '&') {
		int ThisChar = LastChar;
		LastChar = GetChar();

		if (LastChar == '&') {
			LastChar = GetChar();
			return tok_or;
		}
		
		return ThisChar;
	}

	// <= | <<
	if (LastChar == '<') {
		int ThisChar = LastChar;
		LastChar = GetChar();
		
		if (LastChar == '=') {
			LastChar = GetChar();
			return tok_le;
		}

		if (LastChar == '<') {
			LastChar = GetChar();
			return tok_shl;
		}
		
		return ThisChar;
	}
	
	// >= | >>
	if (LastChar == '>') {
		int ThisChar = LastChar;
		LastChar = GetChar();
		
		if (LastChar == '=') {
			LastChar = GetChar();
			return tok_ge;
		}

		if (LastChar == '>') {
			LastChar = GetChar();
			return tok_shr;
		}
		
		return ThisChar;
	}
	
	// Комментарий
	if (LastChar == '/') {
		int ThisChar = LastChar;
		LastChar = GetChar();

		// Однострочный комментарий
		if (LastChar == '/') { // Удаляем комментарий
			while (LastChar != EOF && LastChar != '\n' && LastChar != '\r')
				LastChar = GetChar();
			
			if (LastChar != EOF)
				return GetToken();
		}
		
		// Многострочный комментарий
		if (LastChar == '*') { // Удаляем комментарий
			while (LastChar != EOF) {
				LastChar = GetChar();
				
				if (LastChar == '*') {
					LastChar = GetChar();
					if (LastChar == '/') { LastChar = GetChar(); break; }
				}
			}
			
			if (LastChar != EOF)
				return GetToken();
		}
		
		return ThisChar;
	}
	
	// Комментарий
	/*if (LastChar == '{') {
		LastChar = GetChar();
		
		while (LastChar != EOF && LastChar != '}') {
			LastChar = GetChar();
		}
		
		LastChar = GetChar();
		
		if (LastChar != EOF)
			return GetToken();
	}*/

	// В противном случае просто возвращаем символ как значение ASCII
	int ThisChar = LastChar;
	LastChar = GetChar();
	
	return ThisChar;
}

#endif
