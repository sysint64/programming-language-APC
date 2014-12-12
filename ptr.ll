; ModuleID = 'test.cpp'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-f80:128:128-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S32"
target triple = "i686-pc-mingw32"

%struct.TString = type { i32, i8* }

define %struct.TString* @_Z4testP7TStringPc(%struct.TString* %a, i8* %b) {
entry:
  %a.addr = alloca %struct.TString*, align 4
  %b.addr = alloca i8*, align 4
  %Result = alloca %struct.TString*, align 4
  store %struct.TString* %a, %struct.TString** %a.addr, align 4
  store i8* %b, i8** %b.addr, align 4
  %0 = load %struct.TString** %a.addr, align 4
  %Length = getelementptr inbounds %struct.TString* %0, i32 0, i32 0
  %1 = load i32* %Length, align 4
  %2 = load %struct.TString** %Result, align 4
  %Data = getelementptr inbounds %struct.TString* %2, i32 0, i32 1
  %3 = load i8** %Data, align 4
  %arrayidx = getelementptr inbounds i8* %3, i32 %1
  %4 = load i8** %b.addr, align 4
  %call = call i32 @_Z7mmemcpyPcS_i(i8* %arrayidx, i8* %4, i32 10)
  %5 = load %struct.TString** %Result, align 4
  ret %struct.TString* %5
}

declare i32 @_Z7mmemcpyPcS_i(i8*, i8*, i32)

define i32 @main() {
entry:
  %retval = alloca i32, align 4
  %b = alloca i8*, align 4
  %a = alloca %struct.TString*, align 4
  store i32 0, i32* %retval
  %0 = load %struct.TString** %a, align 4
  %1 = load i8** %b, align 4
  %call = call %struct.TString* @_Z4testP7TStringPc(%struct.TString* %0, i8* %1)
  %2 = load i32* %retval
  ret i32 %2
}
