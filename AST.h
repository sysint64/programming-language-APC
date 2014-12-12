//===-----------------------------------------------------------===//
// Абстрактное синтаксическое дерево (AST) 2012г.
// Автор : Кабылин Андрей Николаевич (sys.int64@gmail.com)
//===-----------------------------------------------------------===//

#ifndef H_AST
#define H_AST 1
//
#include "Codegen/Codegen_IR.cpp"
//
#include "AST/Base.cpp"	    // Родители
#include "Type.cpp"	    //
#include "AST/Oper.cpp"     // Операторы
#include "AST/Type.cpp"     // Типы
#include "AST/Expr.cpp"     // Выражения
#include "AST/Convert.cpp"  // Преобразование типов
#include "AST/Var.cpp"      // Переменные
#include "AST/Const.cpp"    // Константы
#include "AST/IfCase.cpp"   // Поток управления if
#include "AST/Loop.cpp"     // Циклы
#include "AST/Func.cpp"     // Подпрограммы
#include "AST/String.cpp"   // Подпрограммы
#include "AST/HType.cpp"    // Определение типов
//

#endif
