#ifndef CPP_ERROR
#define CPP_ERROR 1

#include "Error.h"

// Загрузка сообщений об ошибках и предупреждениях
void LoadMessage(const string FN) {
	FILE *mf = fopen(FN.c_str(), "r");
	string line = "";
	string mns = "";
	int mn = 0;
	char buf = '\0';
	bool err = true;
	
	while (!feof(mf)) {
		fread(&buf, sizeof(char), 1, mf);

		if (buf != '\n' && buf != '\r') {			
			if (buf == 'E' || buf == 'W') {
				if (buf == 'E') err = true;
				else err = false;
				
				fread(&buf, sizeof(char), 1, mf);
				
				while (buf != ' ') {
					mns += buf;
					fread(&buf, sizeof(char), 1, mf);
				} // end while
				
				fread(&buf, sizeof(char), 1, mf);
				mn = strtod(mns.c_str(), 0);
				mns = "";
			} // end if
			
			//line += buf;
			if (err) Errors[mn] += buf;
			else Warnings[mn] += buf;
		} else {
			//if (err) Errors[mn] = line;
			//else Warnings[mn] = line;

			//line = "";
		}
		//Msg.push_back(s);
	}
	
	if (err) Errors[mn] = line;
	else Warnings[mn] = line;
	
	fclose(mf);
}

// функции для вывода ошибок
template<class T> T Error (const int errn, const string s1, const string s2) {
	//SetCurLine();
	string format = "%s:%d:%d: "+Errors[0]+": "+Errors[errn];
	//SetCurLine();
	int line = mLexer->line;

	if (mLexer->EndLine)
		line--;		

	if ((expected_expr != errn) || (NoErrors && expected_expr == errn)) {
		printf(format.c_str(), mLexer->fileName.c_str(), line, mLexer->pos, s1.c_str(), s2.c_str());	
		printf("\n");
	
		fprintf(errf, format.c_str(), mLexer->fileName.c_str(), line, mLexer->pos, s1.c_str(), s2.c_str());	
		fprintf(errf, "\n");
	}
	
	NoErrors = false; condif = false; ifend  = false;
	LIRInS   = false; mLexer->GetNextToken(); isClass = false;
	WithParams.clear(); CurClassName = ""; CurClassTy = 0;
	isFuncDef = false; ThisClassIR = 0; CurClassIR = 0;
	CurClassExpr = 0; InOperator = false;
	//EE = (errn == expected_expr);
	
	if (bie) exit(1);// Завершение работы
	return 0;
}

void Warning (const int errn, const string s1, const string s2) {
	string format = "%s:%d:%d: "+Warnings[0]+": "+Warnings[errn];

	int line = mLexer->line;

	if (mLexer->EndLine)
		line--;	
	
	printf(format.c_str(), mLexer->fileName.c_str(), line, mLexer->pos, s1.c_str(), s2.c_str());	
	printf("\n");
	
	fprintf(errf, format.c_str(), mLexer->fileName.c_str(), line, mLexer->pos, s1.c_str(), s2.c_str());	
	fprintf(errf, "\n");
	
	condif = false; ifend  = false;
	LIRInS   = false;
}

#endif