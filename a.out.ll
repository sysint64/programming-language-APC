; ModuleID = 'a.out.pas'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"

; Type declares
%Allocator = type {i8, i8*, i64, i64, i64}
%Handle = type i32
%TXID = type i32
%DWORD = type i32
%HDC = type i32
%HGLRC = type i32
%BOOL = type i1
%HMODULE = type i32
%LPCSTR = type i8*
%FARPROC = type i8*
%HGDIOBJ = type i32
%COLORREF = type %DWORD
%Rect = type <{i8, i32, i32, i32, i32}>
%POINTFLOAT = type <{i8, float, float}>
%GLYPHMETRICSFLOAT = type <{i8, float, float, %POINTFLOAT, float, float}>
%LAYERPLANEDESCRIPTOR = type <{i8, i16, i16, %DWORD, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, %COLORREF}>
%PIXELFORMATDESCRIPTOR = type <{i8, i16, i16, %DWORD, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, i8, %DWORD, %DWORD, %DWORD}>
%Display = type i32
%XVisualInfo = type i32
%string = type {i8, i32, i8*}
%wstring = type {i8, i32, i16*}
%LinearAllocator = type {i8, i8*, i8*, i64, i64, i64}
%StackHeader = type {i8, i8*, i8}
%StackAllocator = type {i8, %StackHeader, i8*, i8*, i8*, i64, i64, i64}
%FreeListHeader = type {i8, i64, i8, i32}
%FreeBlock = type {i8, i64, %FreeBlock*}
%FreeListAllocator = type {i8, %FreeListHeader, %FreeBlock*, i8*, i64, i64, i64}
%PoolAllocator = type {i8, i64, i8, i8**, i8*, i64, i64, i64}

; Global vars
@PROT_NONE = private unnamed_addr constant i32 0
@PROT_READ = private unnamed_addr constant i32 1
@PROT_WRITE = private unnamed_addr constant i32 2
@PROT_EXEC = private unnamed_addr constant i32 4
@MAP_FILE = private unnamed_addr constant i32 0
@MAP_SHARED = private unnamed_addr constant i32 1
@MAP_PRIVATE = private unnamed_addr constant i32 2
@MAP_TYPE = private unnamed_addr constant i32 0
@MAP_FIXED = private unnamed_addr constant i32 0
@MAP_ANONYMOUS = private unnamed_addr constant i32 0
@MAP_ANON = private unnamed_addr constant i32 0
@MAP_FAILED = private unnamed_addr constant i32 1
@MS_ASYNC = private unnamed_addr constant i32 1
@MS_SYNC = private unnamed_addr constant i32 2
@MS_INVALIDATE = private unnamed_addr constant i32 4

; Funcs declares
declare i8* @new(i64)
declare void @delete(i8*)
declare void @llvm.lifetime.start(i64, i8*)
declare void @llvm.lifetime.end(i64, i8*)
declare void @llvm.memcpy.p0i8.p0i8.i64(i8*, i8*, i64, i32, i1)
declare void @llvm.memset.p0i8.i64(i8*, i8, i64, i32, i1)
declare x86_stdcallcc %HMODULE @LoadLibraryW(i16*)
declare x86_stdcallcc %HMODULE @LoadLibraryA(i8*)
declare x86_stdcallcc i1 @FreeLibrary(%HMODULE)
declare x86_stdcallcc %FARPROC @GetProcAddress(%HMODULE, %LPCSTR)
declare i8* @mmap(i8*, i64, i32, i32, i32, i64)
declare i32 @munmap(i8*, i64)
declare i32 @mprotect(i8*, i64, i32)
declare i32 @msync(i8*, i64, i32)
declare i32 @mlock(i8*, i64)
declare i32 @munlock(i8*, i64)
declare float @llvm.sqrt.f32(float)
declare double @llvm.sqrt.f64(double)
declare float @llvm.sin.f32(float)
declare double @llvm.sin.f64(double)
declare float @llvm.cos.f32(float)
declare double @llvm.cos.f64(double)
declare float @llvm.powi.f32(float, i32)
declare double @llvm.powi.f64(double, i32)
declare float @llvm.pow.f32(float, float)
declare double @llvm.pow.f64(double, double)
declare float @llvm.exp.f32(float)
declare double @llvm.exp.f64(double)
declare float @llvm.log.f32(float)
declare double @llvm.log.f64(double)
declare float @llvm.log.f32.1(float)
declare double @llvm.log.f64.1(double)
declare float @llvm.log2.f32(float)
declare double @llvm.log2.f64(double)
declare float @llvm.fma.f32(float, float, float)
declare double @llvm.fma.f64(double, double, double)
declare float @llvm.fabs.f32(float)
declare double @llvm.fabs.f64(double)
declare float @llvm.copysign.f32(float, float)
declare double @llvm.copysign.f64(double, double)
declare float @llvm.floor.f32(float)
declare double @llvm.floor.f64(double)
declare float @llvm.ceil.f32(float)
declare double @llvm.ceil.f64(double)
declare float @llvm.trunc.f32(float)
declare double @llvm.trunc.f64(double)
declare float @llvm.rint.f32(float)
declare double @llvm.rint.f64(double)
declare float @llvm.nearbyint.f32(float)
declare double @llvm.nearbyint.f64(double)
declare float @llvm.round.f32(float)
declare double @llvm.round.f64(double)

; Implementation
define void @createAllocator(%Allocator* %alloc.arg, i64 %size.arg, i8* %start.arg) inlinehint {
code:
  %alloc = alloca %Allocator*
  %size.1 = alloca i64
  %start.1 = alloca i8*

  store %Allocator* undef, %Allocator** %alloc
  store %Allocator* %alloc.arg, %Allocator** %alloc
  store i64 undef, i64* %size.1
  store i64 %size.arg, i64* %size.1
  store i8* undef, i8** %start.1
  store i8* %start.arg, i8** %start.1
  %start.1.load = load i8** %start.1
  %alloc.load = load %Allocator** %alloc
  %cast = getelementptr inbounds %Allocator* %alloc.load, i32 0, i32 1
  store i8* %start.1.load, i8** %cast
  %size.1.load = load i64* %size.1
  %alloc.load.1 = load %Allocator** %alloc
  %cast.1 = getelementptr inbounds %Allocator* %alloc.load.1, i32 0, i32 2
  store i64 %size.1.load, i64* %cast.1
  %alloc.load.2 = load %Allocator** %alloc
  %cast.2 = getelementptr inbounds %Allocator* %alloc.load.2, i32 0, i32 3
  store i64 0, i64* %cast.2
  %alloc.load.3 = load %Allocator** %alloc
  %cast.3 = getelementptr inbounds %Allocator* %alloc.load.3, i32 0, i32 4
  store i64 0, i64* %cast.3
  ret void
}

define i32 @strLen(i8* %str.arg) inlinehint {
code:
  %str = alloca i8*
  %ch = alloca i8
  %len = alloca i32

  store i8* undef, i8** %str
  store i8* %str.arg, i8** %str
  store i8 undef, i8* %ch
  %str.load = load i8** %str
  %cast.4 = getelementptr inbounds i8* %str.load, i32 0
  %cast.4.load = load i8* %cast.4
  store i8 %cast.4.load, i8* %ch
  store i32 undef, i32* %len
  store i32 0, i32* %len
  br label %while

while:
  %ch.load = load i8* %ch
  %cmp = icmp ne i8 %ch.load, 0
  br i1 %cmp, label %while.do, label %while.end

while.do:
  %len.load = load i32* %len
  %add = add i32 %len.load, 1
  store i32 %add, i32* %len
  br label %while

while.end:
  %len.load.1 = load i32* %len
  ret i32 %len.load.1
  ret i32 undef
}

define void @setLength(%string* %s.arg, i32 %size.arg) inlinehint {
code:
  %s = alloca %string*
  %size.2 = alloca i32
  %tmp = alloca %string
  %i = alloca i32
  %ss = alloca %string*

  store %string* undef, %string** %s
  store %string* %s.arg, %string** %s
  store i32 undef, i32* %size.2
  store i32 %size.arg, i32* %size.2
  %s.load = load %string** %s
  %cast.5 = getelementptr inbounds %string* %s.load, i32 0, i32 1
  %cast.5.load = load i32* %cast.5
  %cmp.1 = icmp eq i32 %cast.5.load, 0
  br i1 %cmp.1, label %if, label %if.else

if:
  %size.2.load = load i32* %size.2
  %add.1 = add i32 %size.2.load, 1
  %s.load.1 = load %string** %s
  %cast.6 = getelementptr inbounds %string* %s.load.1, i32 0, i32 1
  store i32 %add.1, i32* %cast.6
  %size.2.load.1 = load i32* %size.2
  %add.2 = add i32 %size.2.load.1, 1
  %cast.7 = sext i32 %add.2 to i64
  %s.load.2 = load %string** %s
  %cast.8 = getelementptr inbounds %string* %s.load.2, i32 0, i32 2
  %cast.8.load = load i8** %cast.8
  %new.1 = call noalias i8* @new(i64 %cast.7)
  %cast.9 = bitcast i8* %new.1 to i8*
  %s.load.3 = load %string** %s
  %cast.10 = getelementptr inbounds %string* %s.load.3, i32 0, i32 2
  %s.load.4 = load %string** %s
  %cast.11 = getelementptr inbounds %string* %s.load.4, i32 0, i32 2
  store i8* %cast.9, i8** %cast.11
  br label %if.end

if.else:
  store %string undef, %string* %tmp
  %size.2.load.2 = load i32* %size.2
  %s.load.5 = load %string** %s
  %cast.12 = getelementptr inbounds %string* %s.load.5, i32 0, i32 1
  %cast.12.load = load i32* %cast.12
  %add.3 = add i32 %size.2.load.2, %cast.12.load
  %add.4 = add i32 %add.3, 1
  %cast.13 = sext i32 %add.4 to i64
  %cast.14 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  %cast.14.load = load i8** %cast.14
  %new.2 = call noalias i8* @new(i64 %cast.13)
  %cast.15 = bitcast i8* %new.2 to i8*
  %cast.16 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  %cast.17 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  store i8* %cast.15, i8** %cast.17
  store i32 undef, i32* %i
  store i32 0, i32* %i
  br label %for

for:
  %i.load = load i32* %i
  %s.load.6 = load %string** %s
  %cast.18 = getelementptr inbounds %string* %s.load.6, i32 0, i32 1
  %cast.18.load = load i32* %cast.18
  %cmp.2 = icmp sle i32 %i.load, %cast.18.load
  br i1 %cmp.2, label %for.do, label %for.end

for.do:
  %i.load.1 = load i32* %i
  %s.load.7 = load %string** %s
  %cast.19 = getelementptr inbounds %string* %s.load.7, i32 0, i32 2
  %i.load.2 = load i32* %i
  %cast.19.load = load i8** %cast.19
  %cast.20 = getelementptr inbounds i8* %cast.19.load, i32 %i.load.2
  %cast.20.load = load i8* %cast.20
  %cast.21 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  %i.load.3 = load i32* %i
  %cast.21.load = load i8** %cast.21
  %cast.22 = getelementptr inbounds i8* %cast.21.load, i32 %i.load.3
  store i8 %cast.20.load, i8* %cast.22
  br label %for.inc

for.inc:
  %i.load.4 = load i32* %i
  %add.5 = add i32 %i.load.4, 1
  store i32 %add.5, i32* %i
  br label %for

for.end:
  store %string* undef, %string** %ss
  %cast.23 = bitcast %string** %ss to i8*
  call void @delete(i8* %cast.23)
  br label %if.end

if.end:
  ret void
}

define void @binary.add(%string* sret %result.arg, %string* %a.arg, %string* %b.arg) inlinehint {
code:
  %a = alloca %string*
  %b = alloca %string*
  %res = alloca %string

  store %string* undef, %string** %a
  store %string* %a.arg, %string** %a
  store %string* undef, %string** %b
  store %string* %b.arg, %string** %b
  store %string undef, %string* %res
  %a.load = load %string** %a
  %cast.24 = getelementptr inbounds %string* %a.load, i32 0, i32 1
  %cast.24.load = load i32* %cast.24
  %b.load = load %string** %b
  %cast.25 = getelementptr inbounds %string* %b.load, i32 0, i32 1
  %cast.25.load = load i32* %cast.25
  %add.6 = add i32 %cast.24.load, %cast.25.load
  call void(%string*, i32) * @setLength(%string* %res, i32 %add.6)
  %cast.26 = getelementptr inbounds %string* %res, i32 0, i32 2
  %cast.26.load = load i8** %cast.26
  %cast.27 = getelementptr inbounds i8* %cast.26.load, i32 0
  %a.load.1 = load %string** %a
  %cast.28 = getelementptr inbounds %string* %a.load.1, i32 0, i32 2
  %cast.28.load = load i8** %cast.28
  %cast.29 = getelementptr inbounds i8* %cast.28.load, i32 0
  %a.load.2 = load %string** %a
  %cast.30 = getelementptr inbounds %string* %a.load.2, i32 0, i32 1
  %cast.30.load = load i32* %cast.30
  %mul = mul i32 %cast.30.load, 1
  %cast.31 = sext i32 %mul to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.27, i8* %cast.29, i64 %cast.31, i32 4, i1 false)
  %cast.32 = getelementptr inbounds %string* %res, i32 0, i32 2
  %a.load.3 = load %string** %a
  %cast.33 = getelementptr inbounds %string* %a.load.3, i32 0, i32 1
  %cast.33.load = load i32* %cast.33
  %cast.32.load = load i8** %cast.32
  %cast.34 = getelementptr inbounds i8* %cast.32.load, i32 %cast.33.load
  %b.load.1 = load %string** %b
  %cast.35 = getelementptr inbounds %string* %b.load.1, i32 0, i32 2
  %cast.35.load = load i8** %cast.35
  %cast.36 = getelementptr inbounds i8* %cast.35.load, i32 0
  %b.load.2 = load %string** %b
  %cast.37 = getelementptr inbounds %string* %b.load.2, i32 0, i32 1
  %cast.37.load = load i32* %cast.37
  %mul.1 = mul i32 %cast.37.load, 1
  %cast.38 = sext i32 %mul.1 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.34, i8* %cast.36, i64 %cast.38, i32 4, i1 false)
  %cast.39 = getelementptr inbounds %string* %res, i32 0, i32 1
  %cast.39.load = load i32* %cast.39
  %cast.40 = getelementptr inbounds %string* %res, i32 0, i32 2
  %cast.41 = getelementptr inbounds %string* %res, i32 0, i32 1
  %cast.41.load = load i32* %cast.41
  %cast.40.load = load i8** %cast.40
  %cast.42 = getelementptr inbounds i8* %cast.40.load, i32 %cast.41.load
  store i8 0, i8* %cast.42
  %res.load = load %string* %res
  store %string %res.load, %string* %result.arg
  ret void
  ret void
}

define void @binary.add.1(%string* sret %result.arg, %string* %a.arg, i8 %b.arg) inlinehint {
code:
  %a.1 = alloca %string*
  %b.1 = alloca i8
  %res.1 = alloca %string

  store %string* undef, %string** %a.1
  store %string* %a.arg, %string** %a.1
  store i8 undef, i8* %b.1
  store i8 %b.arg, i8* %b.1
  store %string undef, %string* %res.1
  %a.1.load = load %string** %a.1
  %cast.43 = getelementptr inbounds %string* %a.1.load, i32 0, i32 1
  %cast.43.load = load i32* %cast.43
  %add.7 = add i32 %cast.43.load, 1
  call void(%string*, i32) * @setLength(%string* %res.1, i32 %add.7)
  %cast.44 = getelementptr inbounds %string* %res.1, i32 0, i32 2
  %cast.44.load = load i8** %cast.44
  %cast.45 = getelementptr inbounds i8* %cast.44.load, i32 0
  %a.1.load.1 = load %string** %a.1
  %cast.46 = getelementptr inbounds %string* %a.1.load.1, i32 0, i32 2
  %cast.46.load = load i8** %cast.46
  %cast.47 = getelementptr inbounds i8* %cast.46.load, i32 0
  %a.1.load.2 = load %string** %a.1
  %cast.48 = getelementptr inbounds %string* %a.1.load.2, i32 0, i32 1
  %cast.48.load = load i32* %cast.48
  %add.8 = add i32 %cast.48.load, 1
  %mul.2 = mul i32 %add.8, 1
  %cast.49 = sext i32 %mul.2 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.45, i8* %cast.47, i64 %cast.49, i32 4, i1 false)
  %cast.50 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.50.load = load i32* %cast.50
  %sub = sub i32 %cast.50.load, 1
  %b.1.load = load i8* %b.1
  %cast.51 = getelementptr inbounds %string* %res.1, i32 0, i32 2
  %cast.52 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.52.load = load i32* %cast.52
  %sub.1 = sub i32 %cast.52.load, 1
  %cast.51.load = load i8** %cast.51
  %cast.53 = getelementptr inbounds i8* %cast.51.load, i32 %sub.1
  store i8 %b.1.load, i8* %cast.53
  %cast.54 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.54.load = load i32* %cast.54
  %cast.55 = getelementptr inbounds %string* %res.1, i32 0, i32 2
  %cast.56 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.56.load = load i32* %cast.56
  %cast.55.load = load i8** %cast.55
  %cast.57 = getelementptr inbounds i8* %cast.55.load, i32 %cast.56.load
  store i8 0, i8* %cast.57
  %res.1.load = load %string* %res.1
  store %string %res.1.load, %string* %result.arg
  ret void
  ret void
}

define void @binary.add.2(%string* sret %result.arg, i8 %a.arg, %string* %b.arg) inlinehint {
code:
  %a.2 = alloca i8
  %b.2 = alloca %string*
  %res.2 = alloca %string

  store i8 undef, i8* %a.2
  store i8 %a.arg, i8* %a.2
  store %string* undef, %string** %b.2
  store %string* %b.arg, %string** %b.2
  store %string undef, %string* %res.2
  %a.2.load = load i8* %a.2
  %cast.58 = sext i8 %a.2.load to i32
  %add.9 = add i32 %cast.58, 1
  call void(%string*, i32) * @setLength(%string* %res.2, i32 %add.9)
  %cast.59 = getelementptr inbounds %string* %res.2, i32 0, i32 2
  %cast.59.load = load i8** %cast.59
  %cast.60 = getelementptr inbounds i8* %cast.59.load, i32 1
  %b.2.load = load %string** %b.2
  %cast.61 = getelementptr inbounds %string* %b.2.load, i32 0, i32 2
  %cast.61.load = load i8** %cast.61
  %cast.62 = getelementptr inbounds i8* %cast.61.load, i32 0
  %b.2.load.1 = load %string** %b.2
  %cast.63 = getelementptr inbounds %string* %b.2.load.1, i32 0, i32 1
  %cast.63.load = load i32* %cast.63
  %add.10 = add i32 %cast.63.load, 1
  %mul.3 = mul i32 %add.10, 1
  %cast.64 = sext i32 %mul.3 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.60, i8* %cast.62, i64 %cast.64, i32 4, i1 false)
  %a.2.load.1 = load i8* %a.2
  %cast.65 = getelementptr inbounds %string* %res.2, i32 0, i32 2
  %cast.65.load = load i8** %cast.65
  %cast.66 = getelementptr inbounds i8* %cast.65.load, i32 0
  store i8 %a.2.load.1, i8* %cast.66
  %cast.67 = getelementptr inbounds %string* %res.2, i32 0, i32 1
  %cast.67.load = load i32* %cast.67
  %cast.68 = getelementptr inbounds %string* %res.2, i32 0, i32 2
  %cast.69 = getelementptr inbounds %string* %res.2, i32 0, i32 1
  %cast.69.load = load i32* %cast.69
  %cast.68.load = load i8** %cast.68
  %cast.70 = getelementptr inbounds i8* %cast.68.load, i32 %cast.69.load
  store i8 0, i8* %cast.70
  %res.2.load = load %string* %res.2
  store %string %res.2.load, %string* %result.arg
  ret void
  ret void
}

define void @binary.add.3(%string* sret %result.arg, %string* %a.arg, i8* %b.arg) inlinehint {
code:
  %a.3 = alloca %string*
  %b.3 = alloca i8*
  %res.3 = alloca %string

  store %string* undef, %string** %a.3
  store %string* %a.arg, %string** %a.3
  store i8* undef, i8** %b.3
  store i8* %b.arg, i8** %b.3
  store %string undef, %string* %res.3
  %a.3.load = load %string** %a.3
  %cast.71 = getelementptr inbounds %string* %a.3.load, i32 0, i32 1
  %cast.71.load = load i32* %cast.71
  %b.3.load = load i8** %b.3
  %call = call i32(i8*) * @strLen(i8* %b.3.load)
  %add.11 = add i32 %cast.71.load, %call
  call void(%string*, i32) * @setLength(%string* %res.3, i32 %add.11)
  %cast.72 = getelementptr inbounds %string* %res.3, i32 0, i32 2
  %cast.72.load = load i8** %cast.72
  %cast.73 = getelementptr inbounds i8* %cast.72.load, i32 0
  %a.3.load.1 = load %string** %a.3
  %cast.74 = getelementptr inbounds %string* %a.3.load.1, i32 0, i32 2
  %cast.74.load = load i8** %cast.74
  %cast.75 = getelementptr inbounds i8* %cast.74.load, i32 0
  %a.3.load.2 = load %string** %a.3
  %cast.76 = getelementptr inbounds %string* %a.3.load.2, i32 0, i32 1
  %cast.76.load = load i32* %cast.76
  %mul.4 = mul i32 %cast.76.load, 1
  %cast.77 = sext i32 %mul.4 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.73, i8* %cast.75, i64 %cast.77, i32 4, i1 false)
  %cast.78 = getelementptr inbounds %string* %res.3, i32 0, i32 2
  %a.3.load.3 = load %string** %a.3
  %cast.79 = getelementptr inbounds %string* %a.3.load.3, i32 0, i32 1
  %cast.79.load = load i32* %cast.79
  %cast.78.load = load i8** %cast.78
  %cast.80 = getelementptr inbounds i8* %cast.78.load, i32 %cast.79.load
  %b.3.load.1 = load i8** %b.3
  %b.3.load.2 = load i8** %b.3
  %call.1 = call i32(i8*) * @strLen(i8* %b.3.load.2)
  %mul.5 = mul i32 %call.1, 1
  %cast.81 = sext i32 %mul.5 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.80, i8* %b.3.load.1, i64 %cast.81, i32 4, i1 false)
  %cast.82 = getelementptr inbounds %string* %res.3, i32 0, i32 1
  %cast.82.load = load i32* %cast.82
  %cast.83 = getelementptr inbounds %string* %res.3, i32 0, i32 2
  %cast.84 = getelementptr inbounds %string* %res.3, i32 0, i32 1
  %cast.84.load = load i32* %cast.84
  %cast.83.load = load i8** %cast.83
  %cast.85 = getelementptr inbounds i8* %cast.83.load, i32 %cast.84.load
  store i8 0, i8* %cast.85
  %res.3.load = load %string* %res.3
  store %string %res.3.load, %string* %result.arg
  ret void
  ret void
}

define void @binary.add.4(%string* sret %result.arg, i8* %a.arg, %string* %b.arg) inlinehint {
code:
  %a.4 = alloca i8*
  %b.4 = alloca %string*
  %res.4 = alloca %string

  store i8* undef, i8** %a.4
  store i8* %a.arg, i8** %a.4
  store %string* undef, %string** %b.4
  store %string* %b.arg, %string** %b.4
  store %string undef, %string* %res.4
  %b.4.load = load %string** %b.4
  %cast.86 = getelementptr inbounds %string* %b.4.load, i32 0, i32 1
  %cast.86.load = load i32* %cast.86
  %a.4.load = load i8** %a.4
  %call.2 = call i32(i8*) * @strLen(i8* %a.4.load)
  %add.12 = add i32 %cast.86.load, %call.2
  call void(%string*, i32) * @setLength(%string* %res.4, i32 %add.12)
  %cast.87 = getelementptr inbounds %string* %res.4, i32 0, i32 2
  %cast.87.load = load i8** %cast.87
  %cast.88 = getelementptr inbounds i8* %cast.87.load, i32 0
  %a.4.load.1 = load i8** %a.4
  %a.4.load.2 = load i8** %a.4
  %call.3 = call i32(i8*) * @strLen(i8* %a.4.load.2)
  %mul.6 = mul i32 %call.3, 1
  %cast.89 = sext i32 %mul.6 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.88, i8* %a.4.load.1, i64 %cast.89, i32 4, i1 false)
  %cast.90 = getelementptr inbounds %string* %res.4, i32 0, i32 2
  %a.4.load.3 = load i8** %a.4
  %call.4 = call i32(i8*) * @strLen(i8* %a.4.load.3)
  %cast.90.load = load i8** %cast.90
  %cast.91 = getelementptr inbounds i8* %cast.90.load, i32 %call.4
  %b.4.load.1 = load %string** %b.4
  %cast.92 = getelementptr inbounds %string* %b.4.load.1, i32 0, i32 2
  %cast.92.load = load i8** %cast.92
  %cast.93 = getelementptr inbounds i8* %cast.92.load, i32 0
  %b.4.load.2 = load %string** %b.4
  %cast.94 = getelementptr inbounds %string* %b.4.load.2, i32 0, i32 1
  %cast.94.load = load i32* %cast.94
  %mul.7 = mul i32 %cast.94.load, 1
  %cast.95 = sext i32 %mul.7 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.91, i8* %cast.93, i64 %cast.95, i32 4, i1 false)
  %cast.96 = getelementptr inbounds %string* %res.4, i32 0, i32 1
  %cast.96.load = load i32* %cast.96
  %cast.97 = getelementptr inbounds %string* %res.4, i32 0, i32 2
  %cast.98 = getelementptr inbounds %string* %res.4, i32 0, i32 1
  %cast.98.load = load i32* %cast.98
  %cast.97.load = load i8** %cast.97
  %cast.99 = getelementptr inbounds i8* %cast.97.load, i32 %cast.98.load
  store i8 0, i8* %cast.99
  %res.4.load = load %string* %res.4
  store %string %res.4.load, %string* %result.arg
  ret void
  ret void
}

define void @assign.idx(%string* %id.arg, i8 %ch.arg, i32 %idx.arg) inlinehint {
code:
  %id = alloca %string*
  %ch.1 = alloca i8
  %idx = alloca i32

  store %string* undef, %string** %id
  store %string* %id.arg, %string** %id
  store i8 undef, i8* %ch.1
  store i8 %ch.arg, i8* %ch.1
  store i32 undef, i32* %idx
  store i32 %idx.arg, i32* %idx
  %idx.load = load i32* %idx
  %ch.1.load = load i8* %ch.1
  %id.load = load %string** %id
  %cast.100 = getelementptr inbounds %string* %id.load, i32 0, i32 2
  %idx.load.1 = load i32* %idx
  %cast.100.load = load i8** %cast.100
  %cast.101 = getelementptr inbounds i8* %cast.100.load, i32 %idx.load.1
  store i8 %ch.1.load, i8* %cast.101
  ret void
}

define void @assign(%string* %id.arg, i8 %val.arg) inlinehint {
code:
  %id.1 = alloca %string*
  %val = alloca i8

  store %string* undef, %string** %id.1
  store %string* %id.arg, %string** %id.1
  store i8 undef, i8* %val
  store i8 %val.arg, i8* %val
  %id.1.load = load %string** %id.1
  call void(%string*, i32) * @setLength(%string* %id.1.load, i32 1)
  %val.load = load i8* %val
  %id.1.load.1 = load %string** %id.1
  %cast.102 = getelementptr inbounds %string* %id.1.load.1, i32 0, i32 2
  %cast.102.load = load i8** %cast.102
  %cast.103 = getelementptr inbounds i8* %cast.102.load, i32 0
  store i8 %val.load, i8* %cast.103
  %id.1.load.2 = load %string** %id.1
  %cast.104 = getelementptr inbounds %string* %id.1.load.2, i32 0, i32 2
  %cast.104.load = load i8** %cast.104
  %cast.105 = getelementptr inbounds i8* %cast.104.load, i32 1
  store i8 0, i8* %cast.105
  ret void
}

define void @assign.1(%string* %id.arg, i8* %val.arg) inlinehint {
code:
  %id.2 = alloca %string*
  %val.1 = alloca i8*

  store %string* undef, %string** %id.2
  store %string* %id.arg, %string** %id.2
  store i8* undef, i8** %val.1
  store i8* %val.arg, i8** %val.1
  %id.2.load = load %string** %id.2
  %val.1.load = load i8** %val.1
  %call.5 = call i32(i8*) * @strLen(i8* %val.1.load)
  call void(%string*, i32) * @setLength(%string* %id.2.load, i32 %call.5)
  %id.2.load.1 = load %string** %id.2
  %cast.106 = getelementptr inbounds %string* %id.2.load.1, i32 0, i32 1
  %cast.106.load = load i32* %cast.106
  %id.2.load.2 = load %string** %id.2
  %cast.107 = getelementptr inbounds %string* %id.2.load.2, i32 0, i32 2
  %id.2.load.3 = load %string** %id.2
  %cast.108 = getelementptr inbounds %string* %id.2.load.3, i32 0, i32 1
  %cast.108.load = load i32* %cast.108
  %cast.107.load = load i8** %cast.107
  %cast.109 = getelementptr inbounds i8* %cast.107.load, i32 %cast.108.load
  store i8 0, i8* %cast.109
  %id.2.load.4 = load %string** %id.2
  %cast.110 = getelementptr inbounds %string* %id.2.load.4, i32 0, i32 2
  %cast.110.load = load i8** %cast.110
  %cast.111 = getelementptr inbounds i8* %cast.110.load, i32 0
  %val.1.load.1 = load i8** %val.1
  %val.1.load.2 = load i8** %val.1
  %call.6 = call i32(i8*) * @strLen(i8* %val.1.load.2)
  %mul.8 = mul i32 %call.6, 1
  %cast.112 = sext i32 %mul.8 to i64
  call void(i8*, i8*, i64, i32, i1) * @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.111, i8* %val.1.load.1, i64 %cast.112, i32 4, i1 false)
  ret void
}

define i1 @binary.eq(%string* %a.arg, %string* %b.arg) inlinehint {
code:
  %a.5 = alloca %string*
  %b.5 = alloca %string*
  %i.1 = alloca i32

  store %string* undef, %string** %a.5
  store %string* %a.arg, %string** %a.5
  store %string* undef, %string** %b.5
  store %string* %b.arg, %string** %b.5
  %a.5.load = load %string** %a.5
  %cast.113 = getelementptr inbounds %string* %a.5.load, i32 0, i32 1
  %cast.113.load = load i32* %cast.113
  %b.5.load = load %string** %b.5
  %cast.114 = getelementptr inbounds %string* %b.5.load, i32 0, i32 1
  %cast.114.load = load i32* %cast.114
  %cmp.3 = icmp ne i32 %cast.113.load, %cast.114.load
  br i1 %cmp.3, label %if.1, label %if.end.1

if.1:
  ret i1 false
  br label %if.end.1

if.end.1:
  store i32 undef, i32* %i.1
  store i32 0, i32* %i.1
  br label %for.1

for.1:
  %i.1.load = load i32* %i.1
  %a.5.load.1 = load %string** %a.5
  %cast.115 = getelementptr inbounds %string* %a.5.load.1, i32 0, i32 1
  %cast.115.load = load i32* %cast.115
  %cmp.4 = icmp sle i32 %i.1.load, %cast.115.load
  br i1 %cmp.4, label %for.do.1, label %for.end.1

for.do.1:
  %a.5.load.2 = load %string** %a.5
  %cast.116 = getelementptr inbounds %string* %a.5.load.2, i32 0, i32 2
  %i.1.load.1 = load i32* %i.1
  %cast.116.load = load i8** %cast.116
  %cast.117 = getelementptr inbounds i8* %cast.116.load, i32 %i.1.load.1
  %cast.117.load = load i8* %cast.117
  %b.5.load.1 = load %string** %b.5
  %cast.118 = getelementptr inbounds %string* %b.5.load.1, i32 0, i32 2
  %i.1.load.2 = load i32* %i.1
  %cast.118.load = load i8** %cast.118
  %cast.119 = getelementptr inbounds i8* %cast.118.load, i32 %i.1.load.2
  %cast.119.load = load i8* %cast.119
  %cmp.5 = icmp ne i8 %cast.117.load, %cast.119.load
  br i1 %cmp.5, label %if.2, label %if.end.2

if.2:
  ret i1 false
  br label %if.end.2

if.end.2:
  br label %for.inc.1

for.inc.1:
  %i.1.load.3 = load i32* %i.1
  %add.13 = add i32 %i.1.load.3, 1
  store i32 %add.13, i32* %i.1
  br label %for.1

for.end.1:
  ret i1 true
  ret i1 undef
}

define i1 @binary.eq.1(%string* %a.arg, i8 %b.arg) inlinehint {
code:
  %a.6 = alloca %string*
  %b.6 = alloca i8

  store %string* undef, %string** %a.6
  store %string* %a.arg, %string** %a.6
  store i8 undef, i8* %b.6
  store i8 %b.arg, i8* %b.6
  %a.6.load = load %string** %a.6
  %cast.120 = getelementptr inbounds %string* %a.6.load, i32 0, i32 1
  %cast.120.load = load i32* %cast.120
  %cmp.6 = icmp eq i32 %cast.120.load, 1
  ret i1 %cmp.6
  %a.6.load.1 = load %string** %a.6
  %cast.121 = getelementptr inbounds %string* %a.6.load.1, i32 0, i32 2
  %cast.121.load = load i8** %cast.121
  %cast.122 = getelementptr inbounds i8* %cast.121.load, i32 0
  %cast.122.load = load i8* %cast.122
  %b.6.load = load i8* %b.6
  %cmp.7 = icmp eq i8 %cast.122.load, %b.6.load
  ret i1 undef
}

define i8* @alignForward(i8* %address.arg, i8 %alignment.arg) inlinehint {
code:
  %address = alloca i8*
  %alignment = alloca i8

  store i8* undef, i8** %address
  store i8* %address.arg, i8** %address
  store i8 undef, i8* %alignment
  store i8 %alignment.arg, i8* %alignment
  %address.load = load i8** %address
  %cast.123 = ptrtoint i8* %address.load to i64
  %alignment.load = load i8* %alignment
  %cast.124 = sext i8 %alignment.load to i32
  %sub.2 = sub i32 %cast.124, 1
  %cast.125 = sext i32 %sub.2 to i64
  %alignment.load.1 = load i8* %alignment
  %cast.126 = sext i8 %alignment.load.1 to i32
  %sub.3 = sub i32 %cast.126, 1
  %cmp.8 = icmp ne i32 %sub.3, 0
  %xor = xor i1 %cmp.8, true
  %cast.127 = zext i1 %xor to i64
  %and = and i64 %cast.125, %cast.127
  %add.14 = add i64 %cast.123, %and
  %cast.128 = inttoptr i64 %add.14 to i8*
  ret i8* %cast.128
  ret i8* undef
}

define i8* @alignBackward(i8* %address.arg, i8 %alignment.arg) inlinehint {
code:
  %address.1 = alloca i8*
  %alignment.1 = alloca i8

  store i8* undef, i8** %address.1
  store i8* %address.arg, i8** %address.1
  store i8 undef, i8* %alignment.1
  store i8 %alignment.arg, i8* %alignment.1
  %address.1.load = load i8** %address.1
  %cast.129 = ptrtoint i8* %address.1.load to i64
  %alignment.1.load = load i8* %alignment.1
  %cast.130 = sext i8 %alignment.1.load to i32
  %sub.4 = sub i32 %cast.130, 1
  %cmp.9 = icmp ne i32 %sub.4, 0
  %xor.1 = xor i1 %cmp.9, true
  %cast.131 = zext i1 %xor.1 to i64
  %and.1 = and i64 %cast.129, %cast.131
  %cast.132 = inttoptr i64 %and.1 to i8*
  ret i8* %cast.132
  ret i8* undef
}

define i8 @alignForwardAdjustment(i8* %address.arg, i8 %alignment.arg) inlinehint {
code:
  %address.2 = alloca i8*
  %alignment.2 = alloca i8
  %adjustment = alloca i8

  store i8* undef, i8** %address.2
  store i8* %address.arg, i8** %address.2
  store i8 undef, i8* %alignment.2
  store i8 %alignment.arg, i8* %alignment.2
  store i8 undef, i8* %adjustment
  %alignment.2.load = load i8* %alignment.2
  %cast.133 = sext i8 %alignment.2.load to i64
  %address.2.load = load i8** %address.2
  %cast.134 = ptrtoint i8* %address.2.load to i64
  %alignment.2.load.1 = load i8* %alignment.2
  %cast.135 = sext i8 %alignment.2.load.1 to i32
  %sub.5 = sub i32 %cast.135, 1
  %cast.136 = sext i32 %sub.5 to i64
  %and.2 = and i64 %cast.134, %cast.136
  %sub.6 = sub i64 %cast.133, %and.2
  %cast.137 = trunc i64 %sub.6 to i8
  store i8 %cast.137, i8* %adjustment
  %adjustment.load = load i8* %adjustment
  %alignment.2.load.2 = load i8* %alignment.2
  %cmp.10 = icmp eq i8 %adjustment.load, %alignment.2.load.2
  br i1 %cmp.10, label %if.3, label %if.end.3

if.3:
  ret i8 0
  br label %if.end.3

if.end.3:
  %adjustment.load.1 = load i8* %adjustment
  ret i8 %adjustment.load.1
  ret i8 undef
}

define i8 @alignForwardAdjustmentWithHeader(i8* %address.arg, i8 %alignment.arg, i8 %headerSize.arg) inlinehint {
code:
  %address.3 = alloca i8*
  %alignment.3 = alloca i8
  %headerSize = alloca i8
  %adjustment.1 = alloca i8
  %neededSpace = alloca i8

  store i8* undef, i8** %address.3
  store i8* %address.arg, i8** %address.3
  store i8 undef, i8* %alignment.3
  store i8 %alignment.arg, i8* %alignment.3
  store i8 undef, i8* %headerSize
  store i8 %headerSize.arg, i8* %headerSize
  store i8 undef, i8* %adjustment.1
  %address.3.load = load i8** %address.3
  %alignment.3.load = load i8* %alignment.3
  %call.7 = call i8(i8*, i8) * @alignForwardAdjustment(i8* %address.3.load, i8 %alignment.3.load)
  store i8 %call.7, i8* %adjustment.1
  store i8 undef, i8* %neededSpace
  %headerSize.load = load i8* %headerSize
  store i8 %headerSize.load, i8* %neededSpace
  %adjustment.1.load = load i8* %adjustment.1
  %neededSpace.load = load i8* %neededSpace
  %cmp.11 = icmp ult i8 %adjustment.1.load, %neededSpace.load
  br i1 %cmp.11, label %if.4, label %if.end.4

if.4:
  %neededSpace.load.1 = load i8* %neededSpace
  %adjustment.1.load.1 = load i8* %adjustment.1
  %sub.7 = sub i8 %neededSpace.load.1, %adjustment.1.load.1
  store i8 %sub.7, i8* %neededSpace
  %adjustment.1.load.2 = load i8* %adjustment.1
  %alignment.3.load.1 = load i8* %alignment.3
  %neededSpace.load.2 = load i8* %neededSpace
  %alignment.3.load.2 = load i8* %alignment.3
  %div = sdiv i8 %neededSpace.load.2, %alignment.3.load.2
  %mul.9 = mul i8 %alignment.3.load.1, %div
  %add.15 = add i8 %adjustment.1.load.2, %mul.9
  store i8 %add.15, i8* %adjustment.1
  %neededSpace.load.3 = load i8* %neededSpace
  %alignment.3.load.3 = load i8* %alignment.3
  %rem = srem i8 %neededSpace.load.3, %alignment.3.load.3
  %cast.138 = sext i8 %rem to i32
  %cmp.12 = icmp sgt i32 %cast.138, 0
  br i1 %cmp.12, label %if.5, label %if.end.5

if.5:
  %adjustment.1.load.3 = load i8* %adjustment.1
  %alignment.3.load.4 = load i8* %alignment.3
  %add.16 = add i8 %adjustment.1.load.3, %alignment.3.load.4
  store i8 %add.16, i8* %adjustment.1
  br label %if.end.5

if.end.5:
  br label %if.end.4

if.end.4:
  %adjustment.1.load.4 = load i8* %adjustment.1
  ret i8 %adjustment.1.load.4
  ret i8 undef
}

define i8 @alignBackwardAdjustment(i8* %address.arg, i8 %alignment.arg) inlinehint {
code:
  %address.4 = alloca i8*
  %alignment.4 = alloca i8
  %adjustment.2 = alloca i8

  store i8* undef, i8** %address.4
  store i8* %address.arg, i8** %address.4
  store i8 undef, i8* %alignment.4
  store i8 %alignment.arg, i8* %alignment.4
  store i8 undef, i8* %adjustment.2
  %address.4.load = load i8** %address.4
  %cast.139 = ptrtoint i8* %address.4.load to i64
  %alignment.4.load = load i8* %alignment.4
  %cast.140 = sext i8 %alignment.4.load to i32
  %sub.8 = sub i32 %cast.140, 1
  %cast.141 = sext i32 %sub.8 to i64
  %and.3 = and i64 %cast.139, %cast.141
  %cast.142 = trunc i64 %and.3 to i8
  store i8 %cast.142, i8* %adjustment.2
  %adjustment.2.load = load i8* %adjustment.2
  %alignment.4.load.1 = load i8* %alignment.4
  %cmp.13 = icmp eq i8 %adjustment.2.load, %alignment.4.load.1
  br i1 %cmp.13, label %if.6, label %if.end.6

if.6:
  ret i8 0
  br label %if.end.6

if.end.6:
  %adjustment.2.load.1 = load i8* %adjustment.2
  ret i8 %adjustment.2.load.1
  ret i8 undef
}

define i8* @ptrAdd(i8* %p.arg, i64 %x.arg) inlinehint {
code:
  %p = alloca i8*
  %x.1 = alloca i64

  store i8* undef, i8** %p
  store i8* %p.arg, i8** %p
  store i64 undef, i64* %x.1
  store i64 %x.arg, i64* %x.1
  %p.load = load i8** %p
  %cast.143 = ptrtoint i8* %p.load to i64
  %x.1.load = load i64* %x.1
  %add.17 = add i64 %cast.143, %x.1.load
  %cast.144 = inttoptr i64 %add.17 to i8*
  ret i8* %cast.144
  ret i8* undef
}

define i8* @ptrSub(i8* %p.arg, i64 %x.arg) inlinehint {
code:
  %p.1 = alloca i8*
  %x.2 = alloca i64

  store i8* undef, i8** %p.1
  store i8* %p.arg, i8** %p.1
  store i64 undef, i64* %x.2
  store i64 %x.arg, i64* %x.2
  %p.1.load = load i8** %p.1
  %cast.145 = ptrtoint i8* %p.1.load to i64
  %x.2.load = load i64* %x.2
  %sub.9 = sub i64 %cast.145, %x.2.load
  %cast.146 = inttoptr i64 %sub.9 to i8*
  ret i8* %cast.146
  ret i8* undef
}

define void @createLinearAllocator(%LinearAllocator* %allocator.arg, i64 %size.arg, i8* %start.arg) inlinehint {
code:
  %allocator = alloca %LinearAllocator*
  %size.3 = alloca i64
  %start.2 = alloca i8*

  store %LinearAllocator* undef, %LinearAllocator** %allocator
  store %LinearAllocator* %allocator.arg, %LinearAllocator** %allocator
  store i64 undef, i64* %size.3
  store i64 %size.arg, i64* %size.3
  store i8* undef, i8** %start.2
  store i8* %start.arg, i8** %start.2
  %allocator.load = load %LinearAllocator** %allocator
  %cast.147 = bitcast %LinearAllocator* %allocator.load to %Allocator*
  %size.3.load = load i64* %size.3
  %start.2.load = load i8** %start.2
  call void(%Allocator*, i64, i8*) * @createAllocator(%Allocator* %cast.147, i64 %size.3.load, i8* %start.2.load)
  %start.2.load.1 = load i8** %start.2
  %allocator.load.1 = load %LinearAllocator** %allocator
  %cast.148 = getelementptr inbounds %LinearAllocator* %allocator.load.1, i32 0, i32 1
  store i8* %start.2.load.1, i8** %cast.148
  ret void
}

define void @deleteLinearAllocator(%LinearAllocator* %allocator.arg) inlinehint {
code:
  %allocator.1 = alloca %LinearAllocator*

  store %LinearAllocator* undef, %LinearAllocator** %allocator.1
  store %LinearAllocator* %allocator.arg, %LinearAllocator** %allocator.1
  ret void
}

define i8* @linearAllocate(%LinearAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg) inlinehint {
code:
  %allocator.2 = alloca %LinearAllocator*
  %size.4 = alloca i64
  %alignment.5 = alloca i8
  %adjustment.3 = alloca i8
  %aligned_address = alloca i64

  store %LinearAllocator* undef, %LinearAllocator** %allocator.2
  store %LinearAllocator* %allocator.arg, %LinearAllocator** %allocator.2
  store i64 undef, i64* %size.4
  store i64 %size.arg, i64* %size.4
  store i8 undef, i8* %alignment.5
  store i8 %alignment.arg, i8* %alignment.5
  store i8 undef, i8* %adjustment.3
  %allocator.2.load = load %LinearAllocator** %allocator.2
  %cast.149 = getelementptr inbounds %LinearAllocator* %allocator.2.load, i32 0, i32 1
  %cast.149.load = load i8** %cast.149
  %alignment.5.load = load i8* %alignment.5
  %call.8 = call i8(i8*, i8) * @alignForwardAdjustment(i8* %cast.149.load, i8 %alignment.5.load)
  store i8 %call.8, i8* %adjustment.3
  %allocator.2.load.1 = load %LinearAllocator** %allocator.2
  %cast.150 = getelementptr inbounds %LinearAllocator* %allocator.2.load.1, i32 0, i32 4
  %cast.150.load = load i64* %cast.150
  %adjustment.3.load = load i8* %adjustment.3
  %cast.151 = sext i8 %adjustment.3.load to i64
  %add.18 = add i64 %cast.150.load, %cast.151
  %size.4.load = load i64* %size.4
  %add.19 = add i64 %add.18, %size.4.load
  %allocator.2.load.2 = load %LinearAllocator** %allocator.2
  %cast.152 = getelementptr inbounds %LinearAllocator* %allocator.2.load.2, i32 0, i32 3
  %cast.152.load = load i64* %cast.152
  %cmp.14 = icmp ugt i64 %add.19, %cast.152.load
  br i1 %cmp.14, label %if.7, label %if.end.7

if.7:
  ret i8* null
  br label %if.end.7

if.end.7:
  store i64 undef, i64* %aligned_address
  %allocator.2.load.3 = load %LinearAllocator** %allocator.2
  %cast.153 = getelementptr inbounds %LinearAllocator* %allocator.2.load.3, i32 0, i32 1
  %cast.153.load = load i8** %cast.153
  %cast.154 = ptrtoint i8* %cast.153.load to i64
  %adjustment.3.load.1 = load i8* %adjustment.3
  %cast.155 = sext i8 %adjustment.3.load.1 to i64
  %add.20 = add i64 %cast.154, %cast.155
  store i64 %add.20, i64* %aligned_address
  %aligned_address.load = load i64* %aligned_address
  %size.4.load.1 = load i64* %size.4
  %add.21 = add i64 %aligned_address.load, %size.4.load.1
  %cast.156 = inttoptr i64 %add.21 to i8*
  %allocator.2.load.4 = load %LinearAllocator** %allocator.2
  %cast.157 = getelementptr inbounds %LinearAllocator* %allocator.2.load.4, i32 0, i32 1
  store i8* %cast.156, i8** %cast.157
  %allocator.2.load.5 = load %LinearAllocator** %allocator.2
  %cast.158 = getelementptr inbounds %LinearAllocator* %allocator.2.load.5, i32 0, i32 4
  %cast.158.load = load i64* %cast.158
  %size.4.load.2 = load i64* %size.4
  %adjustment.3.load.2 = load i8* %adjustment.3
  %cast.159 = sext i8 %adjustment.3.load.2 to i64
  %add.22 = add i64 %size.4.load.2, %cast.159
  %add.23 = add i64 %cast.158.load, %add.22
  %allocator.2.load.6 = load %LinearAllocator** %allocator.2
  %cast.160 = getelementptr inbounds %LinearAllocator* %allocator.2.load.6, i32 0, i32 4
  store i64 %add.23, i64* %cast.160
  %allocator.2.load.7 = load %LinearAllocator** %allocator.2
  %cast.161 = getelementptr inbounds %LinearAllocator* %allocator.2.load.7, i32 0, i32 5
  %cast.161.load = load i64* %cast.161
  %add.24 = add i64 %cast.161.load, 1
  %allocator.2.load.8 = load %LinearAllocator** %allocator.2
  %cast.162 = getelementptr inbounds %LinearAllocator* %allocator.2.load.8, i32 0, i32 5
  store i64 %add.24, i64* %cast.162
  %aligned_address.load.1 = load i64* %aligned_address
  %cast.163 = inttoptr i64 %aligned_address.load.1 to i8*
  ret i8* %cast.163
  ret i8* undef
}

define void @clearLinearAllocator(%LinearAllocator* %allocator.arg) inlinehint {
code:
  %allocator.3 = alloca %LinearAllocator*

  store %LinearAllocator* undef, %LinearAllocator** %allocator.3
  store %LinearAllocator* %allocator.arg, %LinearAllocator** %allocator.3
  %allocator.3.load = load %LinearAllocator** %allocator.3
  %cast.164 = getelementptr inbounds %LinearAllocator* %allocator.3.load, i32 0, i32 5
  store i64 0, i64* %cast.164
  %allocator.3.load.1 = load %LinearAllocator** %allocator.3
  %cast.165 = getelementptr inbounds %LinearAllocator* %allocator.3.load.1, i32 0, i32 4
  store i64 0, i64* %cast.165
  %allocator.3.load.2 = load %LinearAllocator** %allocator.3
  %cast.166 = getelementptr inbounds %LinearAllocator* %allocator.3.load.2, i32 0, i32 2
  %cast.166.load = load i8** %cast.166
  %allocator.3.load.3 = load %LinearAllocator** %allocator.3
  %cast.167 = getelementptr inbounds %LinearAllocator* %allocator.3.load.3, i32 0, i32 1
  store i8* %cast.166.load, i8** %cast.167
  ret void
}

define void @createStackAllocator(%StackAllocator* %allocator.arg, i64 %size.arg, i8* %start.arg) inlinehint {
code:
  %allocator.4 = alloca %StackAllocator*
  %size.5 = alloca i64
  %start.3 = alloca i8*

  store %StackAllocator* undef, %StackAllocator** %allocator.4
  store %StackAllocator* %allocator.arg, %StackAllocator** %allocator.4
  store i64 undef, i64* %size.5
  store i64 %size.arg, i64* %size.5
  store i8* undef, i8** %start.3
  store i8* %start.arg, i8** %start.3
  %allocator.4.load = load %StackAllocator** %allocator.4
  %cast.168 = bitcast %StackAllocator* %allocator.4.load to %Allocator*
  %size.5.load = load i64* %size.5
  %start.3.load = load i8** %start.3
  call void(%Allocator*, i64, i8*) * @createAllocator(%Allocator* %cast.168, i64 %size.5.load, i8* %start.3.load)
  %allocator.4.load.1 = load %StackAllocator** %allocator.4
  %cast.169 = getelementptr inbounds %StackAllocator* %allocator.4.load.1, i32 0, i32 2
  store i8* null, i8** %cast.169
  ret void
}

define void @deleteStackAllocator(%StackAllocator* %allocator.arg) inlinehint {
code:
  %allocator.5 = alloca %StackAllocator*

  store %StackAllocator* undef, %StackAllocator** %allocator.5
  store %StackAllocator* %allocator.arg, %StackAllocator** %allocator.5
  ret void
}

define i8* @stackAllocate(%StackAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg) inlinehint {
code:
  %allocator.6 = alloca %StackAllocator*
  %size.6 = alloca i64
  %alignment.6 = alloca i8
  %adjustment.5 = alloca i8
  %aligned_address.1 = alloca i8*
  %header.1 = alloca %StackHeader*

  store %StackAllocator* undef, %StackAllocator** %allocator.6
  store %StackAllocator* %allocator.arg, %StackAllocator** %allocator.6
  store i64 undef, i64* %size.6
  store i64 %size.arg, i64* %size.6
  store i8 undef, i8* %alignment.6
  store i8 %alignment.arg, i8* %alignment.6
  store i8 undef, i8* %adjustment.5
  %allocator.6.load = load %StackAllocator** %allocator.6
  %cast.170 = getelementptr inbounds %StackAllocator* %allocator.6.load, i32 0, i32 3
  %cast.170.load = load i8** %cast.170
  %alignment.6.load = load i8* %alignment.6
  %call.9 = call i8(i8*, i8, i8) * @alignForwardAdjustmentWithHeader(i8* %cast.170.load, i8 %alignment.6.load, i8 9)
  store i8 %call.9, i8* %adjustment.5
  %allocator.6.load.1 = load %StackAllocator** %allocator.6
  %cast.171 = getelementptr inbounds %StackAllocator* %allocator.6.load.1, i32 0, i32 6
  %cast.171.load = load i64* %cast.171
  %adjustment.5.load = load i8* %adjustment.5
  %cast.172 = sext i8 %adjustment.5.load to i64
  %add.25 = add i64 %cast.171.load, %cast.172
  %size.6.load = load i64* %size.6
  %add.26 = add i64 %add.25, %size.6.load
  %allocator.6.load.2 = load %StackAllocator** %allocator.6
  %cast.173 = getelementptr inbounds %StackAllocator* %allocator.6.load.2, i32 0, i32 5
  %cast.173.load = load i64* %cast.173
  %cmp.15 = icmp ugt i64 %add.26, %cast.173.load
  br i1 %cmp.15, label %if.8, label %if.end.8

if.8:
  ret i8* null
  br label %if.end.8

if.end.8:
  store i8* undef, i8** %aligned_address.1
  %allocator.6.load.3 = load %StackAllocator** %allocator.6
  %cast.174 = getelementptr inbounds %StackAllocator* %allocator.6.load.3, i32 0, i32 3
  %cast.174.load = load i8** %cast.174
  %adjustment.5.load.1 = load i8* %adjustment.5
  %cast.175 = sext i8 %adjustment.5.load.1 to i64
  %call.10 = call i8*(i8*, i64) * @ptrAdd(i8* %cast.174.load, i64 %cast.175)
  store i8* %call.10, i8** %aligned_address.1
  store %StackHeader* undef, %StackHeader** %header.1
  %aligned_address.1.load = load i8** %aligned_address.1
  %call.11 = call i8*(i8*, i64) * @ptrSub(i8* %aligned_address.1.load, i64 9)
  %cast.176 = bitcast i8* %call.11 to %StackHeader*
  store %StackHeader* %cast.176, %StackHeader** %header.1
  %adjustment.5.load.2 = load i8* %adjustment.5
  %header.1.load = load %StackHeader** %header.1
  %cast.177 = getelementptr inbounds %StackHeader* %header.1.load, i32 0, i32 2
  store i8 %adjustment.5.load.2, i8* %cast.177
  %allocator.6.load.4 = load %StackAllocator** %allocator.6
  %cast.178 = getelementptr inbounds %StackAllocator* %allocator.6.load.4, i32 0, i32 2
  %cast.178.load = load i8** %cast.178
  %header.1.load.1 = load %StackHeader** %header.1
  %cast.179 = getelementptr inbounds %StackHeader* %header.1.load.1, i32 0, i32 1
  store i8* %cast.178.load, i8** %cast.179
  %aligned_address.1.load.1 = load i8** %aligned_address.1
  %allocator.6.load.5 = load %StackAllocator** %allocator.6
  %cast.180 = getelementptr inbounds %StackAllocator* %allocator.6.load.5, i32 0, i32 2
  store i8* %aligned_address.1.load.1, i8** %cast.180
  %aligned_address.1.load.2 = load i8** %aligned_address.1
  %size.6.load.1 = load i64* %size.6
  %call.12 = call i8*(i8*, i64) * @ptrAdd(i8* %aligned_address.1.load.2, i64 %size.6.load.1)
  %allocator.6.load.6 = load %StackAllocator** %allocator.6
  %cast.181 = getelementptr inbounds %StackAllocator* %allocator.6.load.6, i32 0, i32 3
  store i8* %call.12, i8** %cast.181
  %allocator.6.load.7 = load %StackAllocator** %allocator.6
  %cast.182 = getelementptr inbounds %StackAllocator* %allocator.6.load.7, i32 0, i32 6
  %cast.182.load = load i64* %cast.182
  %size.6.load.2 = load i64* %size.6
  %adjustment.5.load.3 = load i8* %adjustment.5
  %cast.183 = sext i8 %adjustment.5.load.3 to i64
  %add.27 = add i64 %size.6.load.2, %cast.183
  %add.28 = add i64 %cast.182.load, %add.27
  %allocator.6.load.8 = load %StackAllocator** %allocator.6
  %cast.184 = getelementptr inbounds %StackAllocator* %allocator.6.load.8, i32 0, i32 6
  store i64 %add.28, i64* %cast.184
  %allocator.6.load.9 = load %StackAllocator** %allocator.6
  %cast.185 = getelementptr inbounds %StackAllocator* %allocator.6.load.9, i32 0, i32 7
  %cast.185.load = load i64* %cast.185
  %add.29 = add i64 %cast.185.load, 1
  %allocator.6.load.10 = load %StackAllocator** %allocator.6
  %cast.186 = getelementptr inbounds %StackAllocator* %allocator.6.load.10, i32 0, i32 7
  store i64 %add.29, i64* %cast.186
  %aligned_address.1.load.3 = load i8** %aligned_address.1
  ret i8* %aligned_address.1.load.3
  ret i8* undef
}

define void @stackDeallocate(%StackAllocator* %allocator.arg, i8* %p.arg) inlinehint {
code:
  %allocator.7 = alloca %StackAllocator*
  %p.2 = alloca i8*
  %header.2 = alloca %StackHeader*

  store %StackAllocator* undef, %StackAllocator** %allocator.7
  store %StackAllocator* %allocator.arg, %StackAllocator** %allocator.7
  store i8* undef, i8** %p.2
  store i8* %p.arg, i8** %p.2
  store %StackHeader* undef, %StackHeader** %header.2
  %p.2.load = load i8** %p.2
  %call.13 = call i8*(i8*, i64) * @ptrSub(i8* %p.2.load, i64 9)
  %cast.187 = bitcast i8* %call.13 to %StackHeader*
  store %StackHeader* %cast.187, %StackHeader** %header.2
  %allocator.7.load = load %StackAllocator** %allocator.7
  %cast.188 = getelementptr inbounds %StackAllocator* %allocator.7.load, i32 0, i32 6
  %cast.188.load = load i64* %cast.188
  %allocator.7.load.1 = load %StackAllocator** %allocator.7
  %cast.189 = getelementptr inbounds %StackAllocator* %allocator.7.load.1, i32 0, i32 3
  %cast.189.load = load i8** %cast.189
  %cast.190 = ptrtoint i8* %cast.189.load to i64
  %p.2.load.1 = load i8** %p.2
  %cast.191 = ptrtoint i8* %p.2.load.1 to i64
  %sub.10 = sub i64 %cast.190, %cast.191
  %header.2.load = load %StackHeader** %header.2
  %cast.192 = getelementptr inbounds %StackHeader* %header.2.load, i32 0, i32 2
  %cast.192.load = load i8* %cast.192
  %cast.193 = sext i8 %cast.192.load to i64
  %add.30 = add i64 %sub.10, %cast.193
  %sub.11 = sub i64 %cast.188.load, %add.30
  %allocator.7.load.2 = load %StackAllocator** %allocator.7
  %cast.194 = getelementptr inbounds %StackAllocator* %allocator.7.load.2, i32 0, i32 6
  store i64 %sub.11, i64* %cast.194
  %p.2.load.2 = load i8** %p.2
  %header.2.load.1 = load %StackHeader** %header.2
  %cast.195 = getelementptr inbounds %StackHeader* %header.2.load.1, i32 0, i32 2
  %cast.195.load = load i8* %cast.195
  %cast.196 = sext i8 %cast.195.load to i64
  %call.14 = call i8*(i8*, i64) * @ptrSub(i8* %p.2.load.2, i64 %cast.196)
  %allocator.7.load.3 = load %StackAllocator** %allocator.7
  %cast.197 = getelementptr inbounds %StackAllocator* %allocator.7.load.3, i32 0, i32 3
  store i8* %call.14, i8** %cast.197
  %header.2.load.2 = load %StackHeader** %header.2
  %cast.198 = getelementptr inbounds %StackHeader* %header.2.load.2, i32 0, i32 1
  %cast.198.load = load i8** %cast.198
  %allocator.7.load.4 = load %StackAllocator** %allocator.7
  %cast.199 = getelementptr inbounds %StackAllocator* %allocator.7.load.4, i32 0, i32 2
  store i8* %cast.198.load, i8** %cast.199
  %allocator.7.load.5 = load %StackAllocator** %allocator.7
  %cast.200 = getelementptr inbounds %StackAllocator* %allocator.7.load.5, i32 0, i32 7
  %cast.200.load = load i64* %cast.200
  %sub.12 = sub i64 %cast.200.load, 1
  %allocator.7.load.6 = load %StackAllocator** %allocator.7
  %cast.201 = getelementptr inbounds %StackAllocator* %allocator.7.load.6, i32 0, i32 7
  store i64 %sub.12, i64* %cast.201
  ret void
}

define void @createFreeListAllocator(%FreeListAllocator* %allocator.arg, i64 %size.arg, i8* %start.arg) inlinehint {
code:
  %allocator.8 = alloca %FreeListAllocator*
  %size.9 = alloca i64
  %start.4 = alloca i8*

  store %FreeListAllocator* undef, %FreeListAllocator** %allocator.8
  store %FreeListAllocator* %allocator.arg, %FreeListAllocator** %allocator.8
  store i64 undef, i64* %size.9
  store i64 %size.arg, i64* %size.9
  store i8* undef, i8** %start.4
  store i8* %start.arg, i8** %start.4
  %allocator.8.load = load %FreeListAllocator** %allocator.8
  %cast.202 = bitcast %FreeListAllocator* %allocator.8.load to %Allocator*
  %size.9.load = load i64* %size.9
  %start.4.load = load i8** %start.4
  call void(%Allocator*, i64, i8*) * @createAllocator(%Allocator* %cast.202, i64 %size.9.load, i8* %start.4.load)
  %size.9.load.1 = load i64* %size.9
  %allocator.8.load.1 = load %FreeListAllocator** %allocator.8
  %cast.203 = getelementptr inbounds %FreeListAllocator* %allocator.8.load.1, i32 0, i32 2
  %cast.203.load = load %FreeBlock** %cast.203
  %cast.204 = getelementptr inbounds %FreeBlock* %cast.203.load, i32 0, i32 1
  store i64 %size.9.load.1, i64* %cast.204
  %allocator.8.load.2 = load %FreeListAllocator** %allocator.8
  %cast.205 = getelementptr inbounds %FreeListAllocator* %allocator.8.load.2, i32 0, i32 2
  %cast.205.load = load %FreeBlock** %cast.205
  %cast.206 = getelementptr inbounds %FreeBlock* %cast.205.load, i32 0, i32 2
  store %FreeBlock* null, %FreeBlock** %cast.206
  ret void
}

define void @deleteFreeListAllocator(%FreeListAllocator* %allocator.arg) inlinehint {
code:
  %allocator.9 = alloca %FreeListAllocator*

  store %FreeListAllocator* undef, %FreeListAllocator** %allocator.9
  store %FreeListAllocator* %allocator.arg, %FreeListAllocator** %allocator.9
  %allocator.9.load = load %FreeListAllocator** %allocator.9
  %cast.207 = getelementptr inbounds %FreeListAllocator* %allocator.9.load, i32 0, i32 2
  store %FreeBlock* null, %FreeBlock** %cast.207
  ret void
}

define i8 @getSizeFreeList(%FreeListAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg) inlinehint {
code:
  %allocator.10 = alloca %FreeListAllocator*
  %size.10 = alloca i64
  %alignment.7 = alloca i8
  %free_block = alloca %FreeBlock*
  %adjustment.7 = alloca i8
  %total_size = alloca i64

  store %FreeListAllocator* undef, %FreeListAllocator** %allocator.10
  store %FreeListAllocator* %allocator.arg, %FreeListAllocator** %allocator.10
  store i64 undef, i64* %size.10
  store i64 %size.arg, i64* %size.10
  store i8 undef, i8* %alignment.7
  store i8 %alignment.arg, i8* %alignment.7
  store %FreeBlock* undef, %FreeBlock** %free_block
  %allocator.10.load = load %FreeListAllocator** %allocator.10
  %cast.208 = getelementptr inbounds %FreeListAllocator* %allocator.10.load, i32 0, i32 2
  %cast.208.load = load %FreeBlock** %cast.208
  store %FreeBlock* %cast.208.load, %FreeBlock** %free_block
  br label %while.1

while.1:
  %free_block.load = load %FreeBlock** %free_block
  %cmp.16 = icmp ne %FreeBlock* %free_block.load, null
  br i1 %cmp.16, label %while.do.1, label %while.end.1

while.do.1:
  store i8 undef, i8* %adjustment.7
  %cast.209 = bitcast %FreeBlock** %free_block to i8*
  %alignment.7.load = load i8* %alignment.7
  %call.15 = call i8(i8*, i8, i8) * @alignForwardAdjustmentWithHeader(i8* %cast.209, i8 %alignment.7.load, i8 13)
  store i8 %call.15, i8* %adjustment.7
  store i64 undef, i64* %total_size
  %size.10.load = load i64* %size.10
  %adjustment.7.load = load i8* %adjustment.7
  %cast.210 = sext i8 %adjustment.7.load to i64
  %add.31 = add i64 %size.10.load, %cast.210
  store i64 %add.31, i64* %total_size
  %free_block.load.1 = load %FreeBlock** %free_block
  %cast.211 = getelementptr inbounds %FreeBlock* %free_block.load.1, i32 0, i32 1
  %cast.211.load = load i64* %cast.211
  %total_size.load = load i64* %total_size
  %cmp.17 = icmp ult i64 %cast.211.load, %total_size.load
  br i1 %cmp.17, label %if.9, label %if.end.9

if.9:
  %free_block.load.2 = load %FreeBlock** %free_block
  %cast.212 = getelementptr inbounds %FreeBlock* %free_block.load.2, i32 0, i32 2
  %cast.212.load = load %FreeBlock** %cast.212
  store %FreeBlock* %cast.212.load, %FreeBlock** %free_block
  br label %while.1
  br label %if.end.9

if.end.9:
  %free_block.load.3 = load %FreeBlock** %free_block
  %cast.213 = getelementptr inbounds %FreeBlock* %free_block.load.3, i32 0, i32 1
  %cast.213.load = load i64* %cast.213
  %total_size.load.1 = load i64* %total_size
  %sub.13 = sub i64 %cast.213.load, %total_size.load.1
  %cmp.18 = icmp ule i64 %sub.13, 13
  br i1 %cmp.18, label %if.10, label %if.end.10

if.10:
  %free_block.load.4 = load %FreeBlock** %free_block
  %cast.214 = getelementptr inbounds %FreeBlock* %free_block.load.4, i32 0, i32 1
  %cast.214.load = load i64* %cast.214
  store i64 %cast.214.load, i64* %total_size
  br label %if.end.10

if.end.10:
  %total_size.load.2 = load i64* %total_size
  %cast.215 = trunc i64 %total_size.load.2 to i8
  ret i8 %cast.215
  br label %while.1

while.end.1:
  ret i8 undef
}

define i8* @freeListAllocate(%FreeListAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg, i32 %pageId.arg) inlinehint {
code:
  %allocator.11 = alloca %FreeListAllocator*
  %size.11 = alloca i64
  %alignment.8 = alloca i8
  %pageId.1 = alloca i32
  %prev_free_block = alloca %FreeBlock*
  %free_block.1 = alloca %FreeBlock*
  %adjustment.8 = alloca i8
  %total_size.1 = alloca i64
  %next_block = alloca %FreeBlock*
  %aligned_address.2 = alloca i64
  %header.4 = alloca %FreeListHeader*

  store %FreeListAllocator* undef, %FreeListAllocator** %allocator.11
  store %FreeListAllocator* %allocator.arg, %FreeListAllocator** %allocator.11
  store i64 undef, i64* %size.11
  store i64 %size.arg, i64* %size.11
  store i8 undef, i8* %alignment.8
  store i8 %alignment.arg, i8* %alignment.8
  store i32 undef, i32* %pageId.1
  store i32 %pageId.arg, i32* %pageId.1
  store %FreeBlock* undef, %FreeBlock** %prev_free_block
  store %FreeBlock* null, %FreeBlock** %prev_free_block
  store %FreeBlock* undef, %FreeBlock** %free_block.1
  %allocator.11.load = load %FreeListAllocator** %allocator.11
  %cast.216 = getelementptr inbounds %FreeListAllocator* %allocator.11.load, i32 0, i32 2
  %cast.216.load = load %FreeBlock** %cast.216
  store %FreeBlock* %cast.216.load, %FreeBlock** %free_block.1
  br label %while.2

while.2:
  %free_block.1.load = load %FreeBlock** %free_block.1
  %cmp.19 = icmp ne %FreeBlock* %free_block.1.load, null
  br i1 %cmp.19, label %while.do.2, label %while.end.2

while.do.2:
  store i8 undef, i8* %adjustment.8
  %cast.217 = bitcast %FreeBlock** %free_block.1 to i8*
  %alignment.8.load = load i8* %alignment.8
  %call.16 = call i8(i8*, i8, i8) * @alignForwardAdjustmentWithHeader(i8* %cast.217, i8 %alignment.8.load, i8 13)
  store i8 %call.16, i8* %adjustment.8
  store i64 undef, i64* %total_size.1
  %size.11.load = load i64* %size.11
  %adjustment.8.load = load i8* %adjustment.8
  %cast.218 = sext i8 %adjustment.8.load to i64
  %add.32 = add i64 %size.11.load, %cast.218
  store i64 %add.32, i64* %total_size.1
  %free_block.1.load.1 = load %FreeBlock** %free_block.1
  %cast.219 = getelementptr inbounds %FreeBlock* %free_block.1.load.1, i32 0, i32 1
  %cast.219.load = load i64* %cast.219
  %total_size.1.load = load i64* %total_size.1
  %cmp.20 = icmp ult i64 %cast.219.load, %total_size.1.load
  br i1 %cmp.20, label %if.11, label %if.end.11

if.11:
  %free_block.1.load.2 = load %FreeBlock** %free_block.1
  store %FreeBlock* %free_block.1.load.2, %FreeBlock** %prev_free_block
  %free_block.1.load.3 = load %FreeBlock** %free_block.1
  %cast.220 = getelementptr inbounds %FreeBlock* %free_block.1.load.3, i32 0, i32 2
  %cast.220.load = load %FreeBlock** %cast.220
  store %FreeBlock* %cast.220.load, %FreeBlock** %free_block.1
  br label %while.2
  br label %if.end.11

if.end.11:
  %free_block.1.load.4 = load %FreeBlock** %free_block.1
  %cast.221 = getelementptr inbounds %FreeBlock* %free_block.1.load.4, i32 0, i32 1
  %cast.221.load = load i64* %cast.221
  %total_size.1.load.1 = load i64* %total_size.1
  %sub.14 = sub i64 %cast.221.load, %total_size.1.load.1
  %cmp.21 = icmp ule i64 %sub.14, 13
  br i1 %cmp.21, label %if.12, label %if.else.1

if.12:
  %free_block.1.load.5 = load %FreeBlock** %free_block.1
  %cast.222 = getelementptr inbounds %FreeBlock* %free_block.1.load.5, i32 0, i32 1
  %cast.222.load = load i64* %cast.222
  store i64 %cast.222.load, i64* %total_size.1
  %prev_free_block.load = load %FreeBlock** %prev_free_block
  %cmp.22 = icmp ne %FreeBlock* %prev_free_block.load, null
  br i1 %cmp.22, label %if.13, label %if.else.2

if.13:
  %free_block.1.load.6 = load %FreeBlock** %free_block.1
  %cast.223 = getelementptr inbounds %FreeBlock* %free_block.1.load.6, i32 0, i32 2
  %cast.223.load = load %FreeBlock** %cast.223
  %prev_free_block.load.1 = load %FreeBlock** %prev_free_block
  %cast.224 = getelementptr inbounds %FreeBlock* %prev_free_block.load.1, i32 0, i32 2
  store %FreeBlock* %cast.223.load, %FreeBlock** %cast.224
  br label %if.end.13

if.else.2:
  %free_block.1.load.7 = load %FreeBlock** %free_block.1
  %cast.225 = getelementptr inbounds %FreeBlock* %free_block.1.load.7, i32 0, i32 2
  %cast.225.load = load %FreeBlock** %cast.225
  %allocator.11.load.1 = load %FreeListAllocator** %allocator.11
  %cast.226 = getelementptr inbounds %FreeListAllocator* %allocator.11.load.1, i32 0, i32 2
  store %FreeBlock* %cast.225.load, %FreeBlock** %cast.226
  br label %if.end.13

if.end.13:
  br label %if.end.12

if.else.1:
  store %FreeBlock* undef, %FreeBlock** %next_block
  %cast.227 = bitcast %FreeBlock** %free_block.1 to i8*
  %total_size.1.load.2 = load i64* %total_size.1
  %call.17 = call i8*(i8*, i64) * @ptrAdd(i8* %cast.227, i64 %total_size.1.load.2)
  %cast.228 = bitcast i8* %call.17 to %FreeBlock*
  store %FreeBlock* %cast.228, %FreeBlock** %next_block
  %free_block.1.load.8 = load %FreeBlock** %free_block.1
  %cast.229 = getelementptr inbounds %FreeBlock* %free_block.1.load.8, i32 0, i32 1
  %cast.229.load = load i64* %cast.229
  %total_size.1.load.3 = load i64* %total_size.1
  %sub.15 = sub i64 %cast.229.load, %total_size.1.load.3
  %next_block.load = load %FreeBlock** %next_block
  %cast.230 = getelementptr inbounds %FreeBlock* %next_block.load, i32 0, i32 1
  store i64 %sub.15, i64* %cast.230
  %free_block.1.load.9 = load %FreeBlock** %free_block.1
  %cast.231 = getelementptr inbounds %FreeBlock* %free_block.1.load.9, i32 0, i32 2
  %cast.231.load = load %FreeBlock** %cast.231
  %next_block.load.1 = load %FreeBlock** %next_block
  %cast.232 = getelementptr inbounds %FreeBlock* %next_block.load.1, i32 0, i32 2
  store %FreeBlock* %cast.231.load, %FreeBlock** %cast.232
  %prev_free_block.load.2 = load %FreeBlock** %prev_free_block
  %cmp.23 = icmp ne %FreeBlock* %prev_free_block.load.2, null
  br i1 %cmp.23, label %if.14, label %if.else.3

if.14:
  %next_block.load.2 = load %FreeBlock** %next_block
  %prev_free_block.load.3 = load %FreeBlock** %prev_free_block
  %cast.233 = getelementptr inbounds %FreeBlock* %prev_free_block.load.3, i32 0, i32 2
  store %FreeBlock* %next_block.load.2, %FreeBlock** %cast.233
  br label %if.end.14

if.else.3:
  %next_block.load.3 = load %FreeBlock** %next_block
  %allocator.11.load.2 = load %FreeListAllocator** %allocator.11
  %cast.234 = getelementptr inbounds %FreeListAllocator* %allocator.11.load.2, i32 0, i32 2
  store %FreeBlock* %next_block.load.3, %FreeBlock** %cast.234
  br label %if.end.14

if.end.14:
  br label %if.end.12

if.end.12:
  store i64 undef, i64* %aligned_address.2
  %free_block.1.load.10 = load %FreeBlock** %free_block.1
  %cast.235 = ptrtoint %FreeBlock* %free_block.1.load.10 to i64
  %adjustment.8.load.1 = load i8* %adjustment.8
  %cast.236 = sext i8 %adjustment.8.load.1 to i64
  %add.33 = add i64 %cast.235, %cast.236
  store i64 %add.33, i64* %aligned_address.2
  store %FreeListHeader* undef, %FreeListHeader** %header.4
  %aligned_address.2.load = load i64* %aligned_address.2
  %cast.237 = inttoptr i64 %aligned_address.2.load to i8*
  %sub.16 = sub i32 0, 13
  %cast.238 = getelementptr inbounds i8* %cast.237, i32 %sub.16
  %cast.239 = bitcast i8* %cast.238 to %FreeListHeader*
  store %FreeListHeader* %cast.239, %FreeListHeader** %header.4
  %total_size.1.load.4 = load i64* %total_size.1
  %header.4.load = load %FreeListHeader** %header.4
  %cast.240 = getelementptr inbounds %FreeListHeader* %header.4.load, i32 0, i32 1
  store i64 %total_size.1.load.4, i64* %cast.240
  %total_size.1.load.5 = load i64* %total_size.1
  %cast.241 = trunc i64 %total_size.1.load.5 to i8
  %header.4.load.1 = load %FreeListHeader** %header.4
  %cast.242 = getelementptr inbounds %FreeListHeader* %header.4.load.1, i32 0, i32 2
  store i8 %cast.241, i8* %cast.242
  %pageId.1.load = load i32* %pageId.1
  %header.4.load.2 = load %FreeListHeader** %header.4
  %cast.243 = getelementptr inbounds %FreeListHeader* %header.4.load.2, i32 0, i32 3
  store i32 %pageId.1.load, i32* %cast.243
  %allocator.11.load.3 = load %FreeListAllocator** %allocator.11
  %cast.244 = getelementptr inbounds %FreeListAllocator* %allocator.11.load.3, i32 0, i32 5
  %cast.244.load = load i64* %cast.244
  %total_size.1.load.6 = load i64* %total_size.1
  %add.34 = add i64 %cast.244.load, %total_size.1.load.6
  %allocator.11.load.4 = load %FreeListAllocator** %allocator.11
  %cast.245 = getelementptr inbounds %FreeListAllocator* %allocator.11.load.4, i32 0, i32 5
  store i64 %add.34, i64* %cast.245
  %allocator.11.load.5 = load %FreeListAllocator** %allocator.11
  %cast.246 = getelementptr inbounds %FreeListAllocator* %allocator.11.load.5, i32 0, i32 6
  %cast.246.load = load i64* %cast.246
  %add.35 = add i64 %cast.246.load, 1
  %allocator.11.load.6 = load %FreeListAllocator** %allocator.11
  %cast.247 = getelementptr inbounds %FreeListAllocator* %allocator.11.load.6, i32 0, i32 6
  store i64 %add.35, i64* %cast.247
  %aligned_address.2.load.1 = load i64* %aligned_address.2
  %cast.248 = inttoptr i64 %aligned_address.2.load.1 to i8*
  ret i8* %cast.248
  br label %while.2

while.end.2:
  ret i8* undef
}

define void @freeListDeallocate(%FreeListAllocator* %allocator.arg, i8* %p.arg) inlinehint {
code:
  %allocator.12 = alloca %FreeListAllocator*
  %p.3 = alloca i8*
  %header.5 = alloca %FreeListHeader*
  %block_start = alloca i64
  %block_size = alloca i64
  %block_end = alloca i64
  %prev_free_block.1 = alloca %FreeBlock*
  %free_block.2 = alloca %FreeBlock*
  %temp = alloca %FreeBlock*

  store %FreeListAllocator* undef, %FreeListAllocator** %allocator.12
  store %FreeListAllocator* %allocator.arg, %FreeListAllocator** %allocator.12
  store i8* undef, i8** %p.3
  store i8* %p.arg, i8** %p.3
  store %FreeListHeader* undef, %FreeListHeader** %header.5
  %p.3.load = load i8** %p.3
  %call.18 = call i8*(i8*, i64) * @ptrSub(i8* %p.3.load, i64 13)
  %cast.249 = bitcast i8* %call.18 to %FreeListHeader*
  store %FreeListHeader* %cast.249, %FreeListHeader** %header.5
  store i64 undef, i64* %block_start
  %p.3.load.1 = load i8** %p.3
  %cast.250 = ptrtoint i8* %p.3.load.1 to i64
  %header.5.load = load %FreeListHeader** %header.5
  %cast.251 = getelementptr inbounds %FreeListHeader* %header.5.load, i32 0, i32 2
  %cast.251.load = load i8* %cast.251
  %cast.252 = sext i8 %cast.251.load to i64
  %sub.17 = sub i64 %cast.250, %cast.252
  store i64 %sub.17, i64* %block_start
  store i64 undef, i64* %block_size
  %header.5.load.1 = load %FreeListHeader** %header.5
  %cast.253 = getelementptr inbounds %FreeListHeader* %header.5.load.1, i32 0, i32 1
  %cast.253.load = load i64* %cast.253
  store i64 %cast.253.load, i64* %block_size
  store i64 undef, i64* %block_end
  %block_start.load = load i64* %block_start
  %block_size.load = load i64* %block_size
  %add.36 = add i64 %block_start.load, %block_size.load
  store i64 %add.36, i64* %block_end
  store %FreeBlock* undef, %FreeBlock** %prev_free_block.1
  store %FreeBlock* null, %FreeBlock** %prev_free_block.1
  store %FreeBlock* undef, %FreeBlock** %free_block.2
  %allocator.12.load = load %FreeListAllocator** %allocator.12
  %cast.254 = getelementptr inbounds %FreeListAllocator* %allocator.12.load, i32 0, i32 2
  %cast.254.load = load %FreeBlock** %cast.254
  store %FreeBlock* %cast.254.load, %FreeBlock** %free_block.2
  br label %while.3

while.3:
  %free_block.2.load = load %FreeBlock** %free_block.2
  %cmp.24 = icmp ne %FreeBlock* %free_block.2.load, null
  br i1 %cmp.24, label %while.do.3, label %while.end.3

while.do.3:
  %free_block.2.load.1 = load %FreeBlock** %free_block.2
  %cast.255 = ptrtoint %FreeBlock* %free_block.2.load.1 to i64
  %block_end.load = load i64* %block_end
  %cmp.25 = icmp uge i64 %cast.255, %block_end.load
  br i1 %cmp.25, label %if.15, label %if.end.15

if.15:
  br label %while.end.3
  br label %if.end.15

if.end.15:
  %free_block.2.load.2 = load %FreeBlock** %free_block.2
  store %FreeBlock* %free_block.2.load.2, %FreeBlock** %prev_free_block.1
  %free_block.2.load.3 = load %FreeBlock** %free_block.2
  %cast.256 = getelementptr inbounds %FreeBlock* %free_block.2.load.3, i32 0, i32 2
  %cast.256.load = load %FreeBlock** %cast.256
  store %FreeBlock* %cast.256.load, %FreeBlock** %free_block.2
  br label %while.3

while.end.3:
  %prev_free_block.1.load = load %FreeBlock** %prev_free_block.1
  %cmp.26 = icmp eq %FreeBlock* %prev_free_block.1.load, null
  br i1 %cmp.26, label %if.16, label %if.else.4

if.16:
  %block_start.load.1 = load i64* %block_start
  %cast.257 = inttoptr i64 %block_start.load.1 to i8*
  %cast.258 = bitcast i8* %cast.257 to %FreeBlock*
  store %FreeBlock* %cast.258, %FreeBlock** %prev_free_block.1
  %block_size.load.1 = load i64* %block_size
  %prev_free_block.1.load.1 = load %FreeBlock** %prev_free_block.1
  %cast.259 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.1, i32 0, i32 1
  store i64 %block_size.load.1, i64* %cast.259
  %allocator.12.load.1 = load %FreeListAllocator** %allocator.12
  %cast.260 = getelementptr inbounds %FreeListAllocator* %allocator.12.load.1, i32 0, i32 2
  %cast.260.load = load %FreeBlock** %cast.260
  %prev_free_block.1.load.2 = load %FreeBlock** %prev_free_block.1
  %cast.261 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.2, i32 0, i32 2
  store %FreeBlock* %cast.260.load, %FreeBlock** %cast.261
  %prev_free_block.1.load.3 = load %FreeBlock** %prev_free_block.1
  %allocator.12.load.2 = load %FreeListAllocator** %allocator.12
  %cast.262 = getelementptr inbounds %FreeListAllocator* %allocator.12.load.2, i32 0, i32 2
  store %FreeBlock* %prev_free_block.1.load.3, %FreeBlock** %cast.262
  br label %if.end.16

if.else.4:
  %prev_free_block.1.load.4 = load %FreeBlock** %prev_free_block.1
  %cast.263 = ptrtoint %FreeBlock* %prev_free_block.1.load.4 to i64
  %prev_free_block.1.load.5 = load %FreeBlock** %prev_free_block.1
  %cast.264 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.5, i32 0, i32 1
  %cast.264.load = load i64* %cast.264
  %add.37 = add i64 %cast.263, %cast.264.load
  %block_start.load.2 = load i64* %block_start
  %cmp.27 = icmp eq i64 %add.37, %block_start.load.2
  br i1 %cmp.27, label %if.17, label %if.end.17

if.17:
  %prev_free_block.1.load.6 = load %FreeBlock** %prev_free_block.1
  %cast.265 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.6, i32 0, i32 1
  %cast.265.load = load i64* %cast.265
  %block_size.load.2 = load i64* %block_size
  %add.38 = add i64 %cast.265.load, %block_size.load.2
  %prev_free_block.1.load.7 = load %FreeBlock** %prev_free_block.1
  %cast.266 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.7, i32 0, i32 1
  store i64 %add.38, i64* %cast.266
  br label %if.end.17

if.end.17:
  store %FreeBlock* undef, %FreeBlock** %temp
  %block_start.load.3 = load i64* %block_start
  %cast.267 = inttoptr i64 %block_start.load.3 to i8*
  %cast.268 = bitcast i8* %cast.267 to %FreeBlock*
  store %FreeBlock* %cast.268, %FreeBlock** %temp
  %block_size.load.3 = load i64* %block_size
  %temp.load = load %FreeBlock** %temp
  %cast.269 = getelementptr inbounds %FreeBlock* %temp.load, i32 0, i32 1
  store i64 %block_size.load.3, i64* %cast.269
  %prev_free_block.1.load.8 = load %FreeBlock** %prev_free_block.1
  %cast.270 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.8, i32 0, i32 2
  %cast.270.load = load %FreeBlock** %cast.270
  %temp.load.1 = load %FreeBlock** %temp
  %cast.271 = getelementptr inbounds %FreeBlock* %temp.load.1, i32 0, i32 2
  store %FreeBlock* %cast.270.load, %FreeBlock** %cast.271
  %temp.load.2 = load %FreeBlock** %temp
  %prev_free_block.1.load.9 = load %FreeBlock** %prev_free_block.1
  %cast.272 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.9, i32 0, i32 2
  store %FreeBlock* %temp.load.2, %FreeBlock** %cast.272
  %temp.load.3 = load %FreeBlock** %temp
  store %FreeBlock* %temp.load.3, %FreeBlock** %prev_free_block.1
  br label %if.end.16

if.end.16:
  %free_block.2.load.4 = load %FreeBlock** %free_block.2
  %cmp.28 = icmp ne %FreeBlock* %free_block.2.load.4, null
  br i1 %cmp.28, label %if.18, label %if.end.18

if.18:
  %prev_free_block.1.load.10 = load %FreeBlock** %prev_free_block.1
  %cast.273 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.10, i32 0, i32 1
  %cast.273.load = load i64* %cast.273
  %free_block.2.load.5 = load %FreeBlock** %free_block.2
  %cast.274 = getelementptr inbounds %FreeBlock* %free_block.2.load.5, i32 0, i32 1
  %cast.274.load = load i64* %cast.274
  %add.39 = add i64 %cast.273.load, %cast.274.load
  %prev_free_block.1.load.11 = load %FreeBlock** %prev_free_block.1
  %cast.275 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.11, i32 0, i32 1
  store i64 %add.39, i64* %cast.275
  %free_block.2.load.6 = load %FreeBlock** %free_block.2
  %cast.276 = getelementptr inbounds %FreeBlock* %free_block.2.load.6, i32 0, i32 2
  %cast.276.load = load %FreeBlock** %cast.276
  %prev_free_block.1.load.12 = load %FreeBlock** %prev_free_block.1
  %cast.277 = getelementptr inbounds %FreeBlock* %prev_free_block.1.load.12, i32 0, i32 2
  store %FreeBlock* %cast.276.load, %FreeBlock** %cast.277
  br label %if.end.18

if.end.18:
  %allocator.12.load.3 = load %FreeListAllocator** %allocator.12
  %cast.278 = getelementptr inbounds %FreeListAllocator* %allocator.12.load.3, i32 0, i32 6
  %cast.278.load = load i64* %cast.278
  %sub.18 = sub i64 %cast.278.load, 1
  %allocator.12.load.4 = load %FreeListAllocator** %allocator.12
  %cast.279 = getelementptr inbounds %FreeListAllocator* %allocator.12.load.4, i32 0, i32 6
  store i64 %sub.18, i64* %cast.279
  %allocator.12.load.5 = load %FreeListAllocator** %allocator.12
  %cast.280 = getelementptr inbounds %FreeListAllocator* %allocator.12.load.5, i32 0, i32 5
  %cast.280.load = load i64* %cast.280
  %block_size.load.4 = load i64* %block_size
  %sub.19 = sub i64 %cast.280.load, %block_size.load.4
  %allocator.12.load.6 = load %FreeListAllocator** %allocator.12
  %cast.281 = getelementptr inbounds %FreeListAllocator* %allocator.12.load.6, i32 0, i32 5
  store i64 %sub.19, i64* %cast.281
  ret void
}

define void @createPoolAllocator(%PoolAllocator* %allocator.arg, i64 %size.arg, i64 %objectSize.arg, i8 %objectAlignment.arg, i8* %start.arg) inlinehint {
code:
  %allocator.13 = alloca %PoolAllocator*
  %size.12 = alloca i64
  %objectSize.1 = alloca i64
  %objectAlignment.1 = alloca i8
  %start.5 = alloca i8*
  %adjustment.9 = alloca i8
  %numObjects = alloca i64
  %p.4 = alloca i8**
  %i.2 = alloca i64

  store %PoolAllocator* undef, %PoolAllocator** %allocator.13
  store %PoolAllocator* %allocator.arg, %PoolAllocator** %allocator.13
  store i64 undef, i64* %size.12
  store i64 %size.arg, i64* %size.12
  store i64 undef, i64* %objectSize.1
  store i64 %objectSize.arg, i64* %objectSize.1
  store i8 undef, i8* %objectAlignment.1
  store i8 %objectAlignment.arg, i8* %objectAlignment.1
  store i8* undef, i8** %start.5
  store i8* %start.arg, i8** %start.5
  %allocator.13.load = load %PoolAllocator** %allocator.13
  %cast.282 = bitcast %PoolAllocator* %allocator.13.load to %Allocator*
  %size.12.load = load i64* %size.12
  %start.5.load = load i8** %start.5
  call void(%Allocator*, i64, i8*) * @createAllocator(%Allocator* %cast.282, i64 %size.12.load, i8* %start.5.load)
  store i8 undef, i8* %adjustment.9
  %start.5.load.1 = load i8** %start.5
  %objectAlignment.1.load = load i8* %objectAlignment.1
  %call.19 = call i8(i8*, i8) * @alignForwardAdjustment(i8* %start.5.load.1, i8 %objectAlignment.1.load)
  store i8 %call.19, i8* %adjustment.9
  %start.5.load.2 = load i8** %start.5
  %adjustment.9.load = load i8* %adjustment.9
  %cast.283 = sext i8 %adjustment.9.load to i64
  %call.20 = call i8*(i8*, i64) * @ptrAdd(i8* %start.5.load.2, i64 %cast.283)
  %cast.284 = bitcast i8* %call.20 to i8**
  %allocator.13.load.1 = load %PoolAllocator** %allocator.13
  %cast.285 = getelementptr inbounds %PoolAllocator* %allocator.13.load.1, i32 0, i32 3
  store i8** %cast.284, i8*** %cast.285
  store i64 undef, i64* %numObjects
  %size.12.load.1 = load i64* %size.12
  %adjustment.9.load.1 = load i8* %adjustment.9
  %cast.286 = sext i8 %adjustment.9.load.1 to i64
  %sub.20 = sub i64 %size.12.load.1, %cast.286
  %objectSize.1.load = load i64* %objectSize.1
  %div.1 = sdiv i64 %sub.20, %objectSize.1.load
  store i64 %div.1, i64* %numObjects
  store i8** undef, i8*** %p.4
  %allocator.13.load.2 = load %PoolAllocator** %allocator.13
  %cast.287 = getelementptr inbounds %PoolAllocator* %allocator.13.load.2, i32 0, i32 3
  %cast.287.load = load i8*** %cast.287
  store i8** %cast.287.load, i8*** %p.4
  store i64 undef, i64* %i.2
  store i64 0, i64* %i.2
  br label %for.2

for.2:
  %i.2.load = load i64* %i.2
  %numObjects.load = load i64* %numObjects
  %sub.21 = sub i64 %numObjects.load, 1
  %cmp.29 = icmp sle i64 %i.2.load, %sub.21
  br i1 %cmp.29, label %for.do.2, label %for.end.2

for.do.2:
  %cast.288 = bitcast i8*** %p.4 to i8*
  %objectSize.1.load.1 = load i64* %objectSize.1
  %call.21 = call i8*(i8*, i64) * @ptrAdd(i8* %cast.288, i64 %objectSize.1.load.1)
  %p.4.load = load i8*** %p.4
  store i8* %call.21, i8** %p.4.load
  %p.4.load.1 = load i8*** %p.4
  %p.4.load.1.load = load i8** %p.4.load.1
  %cast.289 = bitcast i8* %p.4.load.1.load to i8**
  store i8** %cast.289, i8*** %p.4
  br label %for.inc.2

for.inc.2:
  %i.2.load.1 = load i64* %i.2
  %add.40 = add i64 %i.2.load.1, 1
  store i64 %add.40, i64* %i.2
  br label %for.2

for.end.2:
  %p.4.load.2 = load i8*** %p.4
  store i8* null, i8** %p.4.load.2
  ret void
}

define void @deletePoolAllocator(%PoolAllocator* %allocator.arg) inlinehint {
code:
  %allocator.14 = alloca %PoolAllocator*

  store %PoolAllocator* undef, %PoolAllocator** %allocator.14
  store %PoolAllocator* %allocator.arg, %PoolAllocator** %allocator.14
  %allocator.14.load = load %PoolAllocator** %allocator.14
  %cast.290 = getelementptr inbounds %PoolAllocator* %allocator.14.load, i32 0, i32 3
  store i8** null, i8*** %cast.290
  ret void
}

define i8* @poolAllocate(%PoolAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg) inlinehint {
code:
  %allocator.15 = alloca %PoolAllocator*
  %size.13 = alloca i64
  %alignment.9 = alloca i8
  %p.5 = alloca i8*

  store %PoolAllocator* undef, %PoolAllocator** %allocator.15
  store %PoolAllocator* %allocator.arg, %PoolAllocator** %allocator.15
  store i64 undef, i64* %size.13
  store i64 %size.arg, i64* %size.13
  store i8 undef, i8* %alignment.9
  store i8 %alignment.arg, i8* %alignment.9
  %allocator.15.load = load %PoolAllocator** %allocator.15
  %cast.291 = getelementptr inbounds %PoolAllocator* %allocator.15.load, i32 0, i32 3
  %cast.291.load = load i8*** %cast.291
  %cmp.30 = icmp eq i8** %cast.291.load, null
  br i1 %cmp.30, label %if.19, label %if.end.19

if.19:
  ret i8* null
  br label %if.end.19

if.end.19:
  store i8* undef, i8** %p.5
  %allocator.15.load.1 = load %PoolAllocator** %allocator.15
  %cast.292 = getelementptr inbounds %PoolAllocator* %allocator.15.load.1, i32 0, i32 3
  %cast.293 = bitcast i8*** %cast.292 to i8*
  store i8* %cast.293, i8** %p.5
  %allocator.15.load.2 = load %PoolAllocator** %allocator.15
  %cast.294 = getelementptr inbounds %PoolAllocator* %allocator.15.load.2, i32 0, i32 3
  %cast.294.load = load i8*** %cast.294
  %cast.294.load.load = load i8** %cast.294.load
  %cast.295 = bitcast i8* %cast.294.load.load to i8**
  %allocator.15.load.3 = load %PoolAllocator** %allocator.15
  %cast.296 = getelementptr inbounds %PoolAllocator* %allocator.15.load.3, i32 0, i32 3
  store i8** %cast.295, i8*** %cast.296
  %allocator.15.load.4 = load %PoolAllocator** %allocator.15
  %cast.297 = getelementptr inbounds %PoolAllocator* %allocator.15.load.4, i32 0, i32 6
  %cast.297.load = load i64* %cast.297
  %size.13.load = load i64* %size.13
  %add.41 = add i64 %cast.297.load, %size.13.load
  %allocator.15.load.5 = load %PoolAllocator** %allocator.15
  %cast.298 = getelementptr inbounds %PoolAllocator* %allocator.15.load.5, i32 0, i32 6
  store i64 %add.41, i64* %cast.298
  %allocator.15.load.6 = load %PoolAllocator** %allocator.15
  %cast.299 = getelementptr inbounds %PoolAllocator* %allocator.15.load.6, i32 0, i32 7
  %cast.299.load = load i64* %cast.299
  %add.42 = add i64 %cast.299.load, 1
  %allocator.15.load.7 = load %PoolAllocator** %allocator.15
  %cast.300 = getelementptr inbounds %PoolAllocator* %allocator.15.load.7, i32 0, i32 7
  store i64 %add.42, i64* %cast.300
  %p.5.load = load i8** %p.5
  ret i8* %p.5.load
  ret i8* undef
}

define void @poolDeallocate(%PoolAllocator* %allocator.arg, i8* %p.arg) inlinehint {
code:
  %allocator.16 = alloca %PoolAllocator*
  %p.6 = alloca i8*
  %c = alloca i8*

  store %PoolAllocator* undef, %PoolAllocator** %allocator.16
  store %PoolAllocator* %allocator.arg, %PoolAllocator** %allocator.16
  store i8* undef, i8** %p.6
  store i8* %p.arg, i8** %p.6
  store i8* undef, i8** %c
  %allocator.16.load = load %PoolAllocator** %allocator.16
  %cast.301 = getelementptr inbounds %PoolAllocator* %allocator.16.load, i32 0, i32 3
  %cast.302 = bitcast i8*** %cast.301 to i8*
  store i8* %cast.302, i8** %c
  %c.load = load i8** %c
  %p.6.load = load i8** %p.6
  %cast.303 = bitcast i8* %p.6.load to i8**
  store i8* %c.load, i8** %cast.303
  %p.6.load.1 = load i8** %p.6
  %cast.304 = bitcast i8* %p.6.load.1 to i8**
  %allocator.16.load.1 = load %PoolAllocator** %allocator.16
  %cast.305 = getelementptr inbounds %PoolAllocator* %allocator.16.load.1, i32 0, i32 3
  store i8** %cast.304, i8*** %cast.305
  %allocator.16.load.2 = load %PoolAllocator** %allocator.16
  %cast.306 = getelementptr inbounds %PoolAllocator* %allocator.16.load.2, i32 0, i32 6
  %cast.306.load = load i64* %cast.306
  %allocator.16.load.3 = load %PoolAllocator** %allocator.16
  %cast.307 = getelementptr inbounds %PoolAllocator* %allocator.16.load.3, i32 0, i32 1
  %cast.307.load = load i64* %cast.307
  %sub.22 = sub i64 %cast.306.load, %cast.307.load
  %allocator.16.load.4 = load %PoolAllocator** %allocator.16
  %cast.308 = getelementptr inbounds %PoolAllocator* %allocator.16.load.4, i32 0, i32 6
  store i64 %sub.22, i64* %cast.308
  %allocator.16.load.5 = load %PoolAllocator** %allocator.16
  %cast.309 = getelementptr inbounds %PoolAllocator* %allocator.16.load.5, i32 0, i32 7
  %cast.309.load = load i64* %cast.309
  %sub.23 = sub i64 %cast.309.load, 1
  %allocator.16.load.6 = load %PoolAllocator** %allocator.16
  %cast.310 = getelementptr inbounds %PoolAllocator* %allocator.16.load.6, i32 0, i32 7
  store i64 %sub.23, i64* %cast.310
  ret void
}

; Main driver code
define void @main() {
code:
  %i.3 = alloca i32

  store i32 undef, i32* %i.3
  store i32 0, i32* %i.3
  ret void
}


; Metadata
!0 = metadata !{ metadata !0 }
