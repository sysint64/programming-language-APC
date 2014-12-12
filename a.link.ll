; ModuleID = 'a.out.ll'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "i686-pc-mingw32"

%string = type { i8, i32, i8* }
%struct._SECURITY_ATTRIBUTES = type { i32, i8*, i32 }

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

declare i8* @new(i64)

declare void @delete(i8*)

; Function Attrs: nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #0

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #0

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1) #0

; Function Attrs: nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) #0

declare x86_stdcallcc i32 @LoadLibraryW(i16*)

declare x86_stdcallcc i32 @LoadLibraryA(i8*)

declare x86_stdcallcc i1 @FreeLibrary(i32)

declare x86_stdcallcc i8* @GetProcAddress(i32, i8*)

; Function Attrs: nounwind readnone
declare float @llvm.sqrt.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.sqrt.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.sin.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.sin.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.cos.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.cos.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.powi.f32(float, i32) #1

; Function Attrs: nounwind readnone
declare double @llvm.powi.f64(double, i32) #1

; Function Attrs: nounwind readnone
declare float @llvm.pow.f32(float, float) #1

; Function Attrs: nounwind readnone
declare double @llvm.pow.f64(double, double) #1

; Function Attrs: nounwind readnone
declare float @llvm.exp.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.exp.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.log.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.log.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.log.f32.1(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.log.f64.1(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.log2.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.log2.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.fma.f32(float, float, float) #1

; Function Attrs: nounwind readnone
declare double @llvm.fma.f64(double, double, double) #1

; Function Attrs: nounwind readnone
declare float @llvm.fabs.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.fabs.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.copysign.f32(float, float) #1

; Function Attrs: nounwind readnone
declare double @llvm.copysign.f64(double, double) #1

; Function Attrs: nounwind readnone
declare float @llvm.floor.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.floor.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.ceil.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.ceil.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.trunc.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.trunc.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.rint.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.rint.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.nearbyint.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.nearbyint.f64(double) #1

; Function Attrs: nounwind readnone
declare float @llvm.round.f32(float) #1

; Function Attrs: nounwind readnone
declare double @llvm.round.f64(double) #1

; Function Attrs: inlinehint
define i32 @strLen(i8* %str.arg) #2 {
code:
  %str = alloca i8*
  %ch = alloca i8
  %len = alloca i32
  store i8* undef, i8** %str
  store i8* %str.arg, i8** %str
  store i8 undef, i8* %ch
  %str.load = load i8** %str
  %cast = getelementptr inbounds i8* %str.load, i32 0
  %cast.load = load i8* %cast
  store i8 %cast.load, i8* %ch
  store i32 undef, i32* %len
  store i32 0, i32* %len
  br label %while

while:                                            ; preds = %while.do, %code
  %ch.load = load i8* %ch
  %cmp = icmp ne i8 %ch.load, 0
  br i1 %cmp, label %while.do, label %while.end

while.do:                                         ; preds = %while
  %len.load = load i32* %len
  %add = add i32 %len.load, 1
  store i32 %add, i32* %len
  br label %while

while.end:                                        ; preds = %while
  %len.load.1 = load i32* %len
  ret i32 %len.load.1
                                                  ; No predecessors!
  ret i32 undef
}

; Function Attrs: inlinehint
define void @setLength(%string* %s.arg, i32 %size.arg) #2 {
code:
  %s = alloca %string*
  %size = alloca i32
  %tmp = alloca %string
  %i = alloca i32
  store %string* undef, %string** %s
  store %string* %s.arg, %string** %s
  store i32 undef, i32* %size
  store i32 %size.arg, i32* %size
  %s.load = load %string** %s
  %cast.1 = getelementptr inbounds %string* %s.load, i32 0, i32 1
  %cast.1.load = load i32* %cast.1
  %cmp.1 = icmp eq i32 %cast.1.load, 0
  br i1 %cmp.1, label %if, label %if.else

if:                                               ; preds = %code
  %size.load = load i32* %size
  %add.1 = add i32 %size.load, 1
  %s.load.1 = load %string** %s
  %cast.2 = getelementptr inbounds %string* %s.load.1, i32 0, i32 1
  store i32 %add.1, i32* %cast.2
  %size.load.1 = load i32* %size
  %add.2 = add i32 %size.load.1, 1
  %cast.3 = sext i32 %add.2 to i64
  %s.load.2 = load %string** %s
  %cast.4 = getelementptr inbounds %string* %s.load.2, i32 0, i32 2
  %cast.4.load = load i8** %cast.4
  %new.1 = call noalias i8* @new(i64 %cast.3)
  %cast.5 = bitcast i8* %new.1 to i8*
  %s.load.3 = load %string** %s
  %cast.6 = getelementptr inbounds %string* %s.load.3, i32 0, i32 2
  %s.load.4 = load %string** %s
  %cast.7 = getelementptr inbounds %string* %s.load.4, i32 0, i32 2
  store i8* %cast.5, i8** %cast.7
  br label %if.end

if.else:                                          ; preds = %code
  store %string undef, %string* %tmp
  %size.load.2 = load i32* %size
  %s.load.5 = load %string** %s
  %cast.8 = getelementptr inbounds %string* %s.load.5, i32 0, i32 1
  %cast.8.load = load i32* %cast.8
  %add.3 = add i32 %size.load.2, %cast.8.load
  %add.4 = add i32 %add.3, 1
  %cast.9 = sext i32 %add.4 to i64
  %cast.10 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  %cast.10.load = load i8** %cast.10
  %new.2 = call noalias i8* @new(i64 %cast.9)
  %cast.11 = bitcast i8* %new.2 to i8*
  %cast.12 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  %cast.13 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  store i8* %cast.11, i8** %cast.13
  store i32 undef, i32* %i
  store i32 0, i32* %i
  br label %for

for:                                              ; preds = %for.inc, %if.else
  %i.load = load i32* %i
  %s.load.6 = load %string** %s
  %cast.14 = getelementptr inbounds %string* %s.load.6, i32 0, i32 1
  %cast.14.load = load i32* %cast.14
  %cmp.2 = icmp sle i32 %i.load, %cast.14.load
  br i1 %cmp.2, label %for.do, label %for.end

for.do:                                           ; preds = %for
  %i.load.1 = load i32* %i
  %s.load.7 = load %string** %s
  %cast.15 = getelementptr inbounds %string* %s.load.7, i32 0, i32 2
  %i.load.2 = load i32* %i
  %cast.15.load = load i8** %cast.15
  %cast.16 = getelementptr inbounds i8* %cast.15.load, i32 %i.load.2
  %cast.16.load = load i8* %cast.16
  %cast.17 = getelementptr inbounds %string* %tmp, i32 0, i32 2
  %i.load.3 = load i32* %i
  %cast.17.load = load i8** %cast.17
  %cast.18 = getelementptr inbounds i8* %cast.17.load, i32 %i.load.3
  store i8 %cast.16.load, i8* %cast.18
  br label %for.inc

for.inc:                                          ; preds = %for.do
  %i.load.4 = load i32* %i
  %add.5 = add i32 %i.load.4, 1
  store i32 %add.5, i32* %i
  br label %for

for.end:                                          ; preds = %for
  %s.load.8 = load %string** %s
  %cast.19 = bitcast %string* %s.load.8 to i8*
  call void @delete(i8* %cast.19)
  br label %if.end

if.end:                                           ; preds = %for.end, %if
  ret void
}

; Function Attrs: inlinehint
define void @binary.add(%string* sret %result.arg, %string* %a.arg, %string* %b.arg) #2 {
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
  %cast.20 = getelementptr inbounds %string* %a.load, i32 0, i32 1
  %cast.20.load = load i32* %cast.20
  %b.load = load %string** %b
  %cast.21 = getelementptr inbounds %string* %b.load, i32 0, i32 1
  %cast.21.load = load i32* %cast.21
  %add.6 = add i32 %cast.20.load, %cast.21.load
  call void @setLength(%string* %res, i32 %add.6)
  %cast.22 = getelementptr inbounds %string* %res, i32 0, i32 2
  %cast.22.load = load i8** %cast.22
  %cast.23 = getelementptr inbounds i8* %cast.22.load, i32 0
  %a.load.1 = load %string** %a
  %cast.24 = getelementptr inbounds %string* %a.load.1, i32 0, i32 2
  %cast.24.load = load i8** %cast.24
  %cast.25 = getelementptr inbounds i8* %cast.24.load, i32 0
  %a.load.2 = load %string** %a
  %cast.26 = getelementptr inbounds %string* %a.load.2, i32 0, i32 1
  %cast.26.load = load i32* %cast.26
  %mul = mul i32 %cast.26.load, 1
  %cast.27 = sext i32 %mul to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.23, i8* %cast.25, i64 %cast.27, i32 4, i1 false)
  %cast.28 = getelementptr inbounds %string* %res, i32 0, i32 2
  %a.load.3 = load %string** %a
  %cast.29 = getelementptr inbounds %string* %a.load.3, i32 0, i32 1
  %cast.29.load = load i32* %cast.29
  %cast.28.load = load i8** %cast.28
  %cast.30 = getelementptr inbounds i8* %cast.28.load, i32 %cast.29.load
  %b.load.1 = load %string** %b
  %cast.31 = getelementptr inbounds %string* %b.load.1, i32 0, i32 2
  %cast.31.load = load i8** %cast.31
  %cast.32 = getelementptr inbounds i8* %cast.31.load, i32 0
  %b.load.2 = load %string** %b
  %cast.33 = getelementptr inbounds %string* %b.load.2, i32 0, i32 1
  %cast.33.load = load i32* %cast.33
  %mul.1 = mul i32 %cast.33.load, 1
  %cast.34 = sext i32 %mul.1 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.30, i8* %cast.32, i64 %cast.34, i32 4, i1 false)
  %cast.35 = getelementptr inbounds %string* %res, i32 0, i32 1
  %cast.35.load = load i32* %cast.35
  %cast.36 = getelementptr inbounds %string* %res, i32 0, i32 2
  %cast.37 = getelementptr inbounds %string* %res, i32 0, i32 1
  %cast.37.load = load i32* %cast.37
  %cast.36.load = load i8** %cast.36
  %cast.38 = getelementptr inbounds i8* %cast.36.load, i32 %cast.37.load
  store i8 0, i8* %cast.38
  %res.load = load %string* %res
  store %string %res.load, %string* %result.arg
  ret void
                                                  ; No predecessors!
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.1(%string* sret %result.arg, %string* %a.arg, i8 %b.arg) #2 {
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
  %cast.39 = getelementptr inbounds %string* %a.1.load, i32 0, i32 1
  %cast.39.load = load i32* %cast.39
  %add.7 = add i32 %cast.39.load, 1
  call void @setLength(%string* %res.1, i32 %add.7)
  %cast.40 = getelementptr inbounds %string* %res.1, i32 0, i32 2
  %cast.40.load = load i8** %cast.40
  %cast.41 = getelementptr inbounds i8* %cast.40.load, i32 0
  %a.1.load.1 = load %string** %a.1
  %cast.42 = getelementptr inbounds %string* %a.1.load.1, i32 0, i32 2
  %cast.42.load = load i8** %cast.42
  %cast.43 = getelementptr inbounds i8* %cast.42.load, i32 0
  %a.1.load.2 = load %string** %a.1
  %cast.44 = getelementptr inbounds %string* %a.1.load.2, i32 0, i32 1
  %cast.44.load = load i32* %cast.44
  %add.8 = add i32 %cast.44.load, 1
  %mul.2 = mul i32 %add.8, 1
  %cast.45 = sext i32 %mul.2 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.41, i8* %cast.43, i64 %cast.45, i32 4, i1 false)
  %cast.46 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.46.load = load i32* %cast.46
  %sub = sub i32 %cast.46.load, 1
  %b.1.load = load i8* %b.1
  %cast.47 = getelementptr inbounds %string* %res.1, i32 0, i32 2
  %cast.48 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.48.load = load i32* %cast.48
  %sub.1 = sub i32 %cast.48.load, 1
  %cast.47.load = load i8** %cast.47
  %cast.49 = getelementptr inbounds i8* %cast.47.load, i32 %sub.1
  store i8 %b.1.load, i8* %cast.49
  %cast.50 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.50.load = load i32* %cast.50
  %cast.51 = getelementptr inbounds %string* %res.1, i32 0, i32 2
  %cast.52 = getelementptr inbounds %string* %res.1, i32 0, i32 1
  %cast.52.load = load i32* %cast.52
  %cast.51.load = load i8** %cast.51
  %cast.53 = getelementptr inbounds i8* %cast.51.load, i32 %cast.52.load
  store i8 0, i8* %cast.53
  %res.1.load = load %string* %res.1
  store %string %res.1.load, %string* %result.arg
  ret void
                                                  ; No predecessors!
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.2(%string* sret %result.arg, i8 %a.arg, %string* %b.arg) #2 {
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
  %cast.54 = sext i8 %a.2.load to i32
  %add.9 = add i32 %cast.54, 1
  call void @setLength(%string* %res.2, i32 %add.9)
  %cast.55 = getelementptr inbounds %string* %res.2, i32 0, i32 2
  %cast.55.load = load i8** %cast.55
  %cast.56 = getelementptr inbounds i8* %cast.55.load, i32 1
  %b.2.load = load %string** %b.2
  %cast.57 = getelementptr inbounds %string* %b.2.load, i32 0, i32 2
  %cast.57.load = load i8** %cast.57
  %cast.58 = getelementptr inbounds i8* %cast.57.load, i32 0
  %b.2.load.1 = load %string** %b.2
  %cast.59 = getelementptr inbounds %string* %b.2.load.1, i32 0, i32 1
  %cast.59.load = load i32* %cast.59
  %add.10 = add i32 %cast.59.load, 1
  %mul.3 = mul i32 %add.10, 1
  %cast.60 = sext i32 %mul.3 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.56, i8* %cast.58, i64 %cast.60, i32 4, i1 false)
  %a.2.load.1 = load i8* %a.2
  %cast.61 = getelementptr inbounds %string* %res.2, i32 0, i32 2
  %cast.61.load = load i8** %cast.61
  %cast.62 = getelementptr inbounds i8* %cast.61.load, i32 0
  store i8 %a.2.load.1, i8* %cast.62
  %cast.63 = getelementptr inbounds %string* %res.2, i32 0, i32 1
  %cast.63.load = load i32* %cast.63
  %cast.64 = getelementptr inbounds %string* %res.2, i32 0, i32 2
  %cast.65 = getelementptr inbounds %string* %res.2, i32 0, i32 1
  %cast.65.load = load i32* %cast.65
  %cast.64.load = load i8** %cast.64
  %cast.66 = getelementptr inbounds i8* %cast.64.load, i32 %cast.65.load
  store i8 0, i8* %cast.66
  %res.2.load = load %string* %res.2
  store %string %res.2.load, %string* %result.arg
  ret void
                                                  ; No predecessors!
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.3(%string* sret %result.arg, %string* %a.arg, i8* %b.arg) #2 {
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
  %cast.67 = getelementptr inbounds %string* %a.3.load, i32 0, i32 1
  %cast.67.load = load i32* %cast.67
  %b.3.load = load i8** %b.3
  %call = call i32 @strLen(i8* %b.3.load)
  %add.11 = add i32 %cast.67.load, %call
  call void @setLength(%string* %res.3, i32 %add.11)
  %cast.68 = getelementptr inbounds %string* %res.3, i32 0, i32 2
  %cast.68.load = load i8** %cast.68
  %cast.69 = getelementptr inbounds i8* %cast.68.load, i32 0
  %a.3.load.1 = load %string** %a.3
  %cast.70 = getelementptr inbounds %string* %a.3.load.1, i32 0, i32 2
  %cast.70.load = load i8** %cast.70
  %cast.71 = getelementptr inbounds i8* %cast.70.load, i32 0
  %a.3.load.2 = load %string** %a.3
  %cast.72 = getelementptr inbounds %string* %a.3.load.2, i32 0, i32 1
  %cast.72.load = load i32* %cast.72
  %mul.4 = mul i32 %cast.72.load, 1
  %cast.73 = sext i32 %mul.4 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.69, i8* %cast.71, i64 %cast.73, i32 4, i1 false)
  %cast.74 = getelementptr inbounds %string* %res.3, i32 0, i32 2
  %a.3.load.3 = load %string** %a.3
  %cast.75 = getelementptr inbounds %string* %a.3.load.3, i32 0, i32 1
  %cast.75.load = load i32* %cast.75
  %cast.74.load = load i8** %cast.74
  %cast.76 = getelementptr inbounds i8* %cast.74.load, i32 %cast.75.load
  %b.3.load.1 = load i8** %b.3
  %b.3.load.2 = load i8** %b.3
  %call.1 = call i32 @strLen(i8* %b.3.load.2)
  %mul.5 = mul i32 %call.1, 1
  %cast.77 = sext i32 %mul.5 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.76, i8* %b.3.load.1, i64 %cast.77, i32 4, i1 false)
  %cast.78 = getelementptr inbounds %string* %res.3, i32 0, i32 1
  %cast.78.load = load i32* %cast.78
  %cast.79 = getelementptr inbounds %string* %res.3, i32 0, i32 2
  %cast.80 = getelementptr inbounds %string* %res.3, i32 0, i32 1
  %cast.80.load = load i32* %cast.80
  %cast.79.load = load i8** %cast.79
  %cast.81 = getelementptr inbounds i8* %cast.79.load, i32 %cast.80.load
  store i8 0, i8* %cast.81
  %res.3.load = load %string* %res.3
  store %string %res.3.load, %string* %result.arg
  ret void
                                                  ; No predecessors!
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.4(%string* sret %result.arg, i8* %a.arg, %string* %b.arg) #2 {
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
  %cast.82 = getelementptr inbounds %string* %b.4.load, i32 0, i32 1
  %cast.82.load = load i32* %cast.82
  %a.4.load = load i8** %a.4
  %call.2 = call i32 @strLen(i8* %a.4.load)
  %add.12 = add i32 %cast.82.load, %call.2
  call void @setLength(%string* %res.4, i32 %add.12)
  %cast.83 = getelementptr inbounds %string* %res.4, i32 0, i32 2
  %cast.83.load = load i8** %cast.83
  %cast.84 = getelementptr inbounds i8* %cast.83.load, i32 0
  %a.4.load.1 = load i8** %a.4
  %a.4.load.2 = load i8** %a.4
  %call.3 = call i32 @strLen(i8* %a.4.load.2)
  %mul.6 = mul i32 %call.3, 1
  %cast.85 = sext i32 %mul.6 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.84, i8* %a.4.load.1, i64 %cast.85, i32 4, i1 false)
  %cast.86 = getelementptr inbounds %string* %res.4, i32 0, i32 2
  %a.4.load.3 = load i8** %a.4
  %call.4 = call i32 @strLen(i8* %a.4.load.3)
  %cast.86.load = load i8** %cast.86
  %cast.87 = getelementptr inbounds i8* %cast.86.load, i32 %call.4
  %b.4.load.1 = load %string** %b.4
  %cast.88 = getelementptr inbounds %string* %b.4.load.1, i32 0, i32 2
  %cast.88.load = load i8** %cast.88
  %cast.89 = getelementptr inbounds i8* %cast.88.load, i32 0
  %b.4.load.2 = load %string** %b.4
  %cast.90 = getelementptr inbounds %string* %b.4.load.2, i32 0, i32 1
  %cast.90.load = load i32* %cast.90
  %mul.7 = mul i32 %cast.90.load, 1
  %cast.91 = sext i32 %mul.7 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.87, i8* %cast.89, i64 %cast.91, i32 4, i1 false)
  %cast.92 = getelementptr inbounds %string* %res.4, i32 0, i32 1
  %cast.92.load = load i32* %cast.92
  %cast.93 = getelementptr inbounds %string* %res.4, i32 0, i32 2
  %cast.94 = getelementptr inbounds %string* %res.4, i32 0, i32 1
  %cast.94.load = load i32* %cast.94
  %cast.93.load = load i8** %cast.93
  %cast.95 = getelementptr inbounds i8* %cast.93.load, i32 %cast.94.load
  store i8 0, i8* %cast.95
  %res.4.load = load %string* %res.4
  store %string %res.4.load, %string* %result.arg
  ret void
                                                  ; No predecessors!
  ret void
}

; Function Attrs: inlinehint
define void @assign.idx(%string* %id.arg, i8 %ch.arg, i32 %idx.arg) #2 {
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
  %cast.96 = getelementptr inbounds %string* %id.load, i32 0, i32 2
  %idx.load.1 = load i32* %idx
  %cast.96.load = load i8** %cast.96
  %cast.97 = getelementptr inbounds i8* %cast.96.load, i32 %idx.load.1
  store i8 %ch.1.load, i8* %cast.97
  ret void
}

; Function Attrs: inlinehint
define void @assign(%string* %id.arg, i8 %val.arg) #2 {
code:
  %id.1 = alloca %string*
  %val = alloca i8
  store %string* undef, %string** %id.1
  store %string* %id.arg, %string** %id.1
  store i8 undef, i8* %val
  store i8 %val.arg, i8* %val
  %id.1.load = load %string** %id.1
  call void @setLength(%string* %id.1.load, i32 1)
  %val.load = load i8* %val
  %id.1.load.1 = load %string** %id.1
  %cast.98 = getelementptr inbounds %string* %id.1.load.1, i32 0, i32 2
  %cast.98.load = load i8** %cast.98
  %cast.99 = getelementptr inbounds i8* %cast.98.load, i32 0
  store i8 %val.load, i8* %cast.99
  %id.1.load.2 = load %string** %id.1
  %cast.100 = getelementptr inbounds %string* %id.1.load.2, i32 0, i32 2
  %cast.100.load = load i8** %cast.100
  %cast.101 = getelementptr inbounds i8* %cast.100.load, i32 1
  store i8 0, i8* %cast.101
  ret void
}

; Function Attrs: inlinehint
define void @assign.1(%string* %id.arg, i8* %val.arg) #2 {
code:
  %id.2 = alloca %string*
  %val.1 = alloca i8*
  store %string* undef, %string** %id.2
  store %string* %id.arg, %string** %id.2
  store i8* undef, i8** %val.1
  store i8* %val.arg, i8** %val.1
  %id.2.load = load %string** %id.2
  %val.1.load = load i8** %val.1
  %call.5 = call i32 @strLen(i8* %val.1.load)
  call void @setLength(%string* %id.2.load, i32 %call.5)
  %id.2.load.1 = load %string** %id.2
  %cast.102 = getelementptr inbounds %string* %id.2.load.1, i32 0, i32 1
  %cast.102.load = load i32* %cast.102
  %id.2.load.2 = load %string** %id.2
  %cast.103 = getelementptr inbounds %string* %id.2.load.2, i32 0, i32 2
  %id.2.load.3 = load %string** %id.2
  %cast.104 = getelementptr inbounds %string* %id.2.load.3, i32 0, i32 1
  %cast.104.load = load i32* %cast.104
  %cast.103.load = load i8** %cast.103
  %cast.105 = getelementptr inbounds i8* %cast.103.load, i32 %cast.104.load
  store i8 0, i8* %cast.105
  %id.2.load.4 = load %string** %id.2
  %cast.106 = getelementptr inbounds %string* %id.2.load.4, i32 0, i32 2
  %cast.106.load = load i8** %cast.106
  %cast.107 = getelementptr inbounds i8* %cast.106.load, i32 0
  %val.1.load.1 = load i8** %val.1
  %val.1.load.2 = load i8** %val.1
  %call.6 = call i32 @strLen(i8* %val.1.load.2)
  %mul.8 = mul i32 %call.6, 1
  %cast.108 = sext i32 %mul.8 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.107, i8* %val.1.load.1, i64 %cast.108, i32 4, i1 false)
  ret void
}

; Function Attrs: inlinehint
define i1 @binary.eq(%string* %a.arg, %string* %b.arg) #2 {
code:
  %a.5 = alloca %string*
  %b.5 = alloca %string*
  %i.1 = alloca i32
  store %string* undef, %string** %a.5
  store %string* %a.arg, %string** %a.5
  store %string* undef, %string** %b.5
  store %string* %b.arg, %string** %b.5
  %a.5.load = load %string** %a.5
  %cast.109 = getelementptr inbounds %string* %a.5.load, i32 0, i32 1
  %cast.109.load = load i32* %cast.109
  %b.5.load = load %string** %b.5
  %cast.110 = getelementptr inbounds %string* %b.5.load, i32 0, i32 1
  %cast.110.load = load i32* %cast.110
  %cmp.3 = icmp ne i32 %cast.109.load, %cast.110.load
  br i1 %cmp.3, label %if.1, label %if.end.1

if.1:                                             ; preds = %code
  ret i1 false
                                                  ; No predecessors!
  br label %if.end.1

if.end.1:                                         ; preds = %0, %code
  store i32 undef, i32* %i.1
  store i32 0, i32* %i.1
  br label %for.1

for.1:                                            ; preds = %for.inc.1, %if.end.1
  %i.1.load = load i32* %i.1
  %a.5.load.1 = load %string** %a.5
  %cast.111 = getelementptr inbounds %string* %a.5.load.1, i32 0, i32 1
  %cast.111.load = load i32* %cast.111
  %cmp.4 = icmp sle i32 %i.1.load, %cast.111.load
  br i1 %cmp.4, label %for.do.1, label %for.end.1

for.do.1:                                         ; preds = %for.1
  %a.5.load.2 = load %string** %a.5
  %cast.112 = getelementptr inbounds %string* %a.5.load.2, i32 0, i32 2
  %i.1.load.1 = load i32* %i.1
  %cast.112.load = load i8** %cast.112
  %cast.113 = getelementptr inbounds i8* %cast.112.load, i32 %i.1.load.1
  %cast.113.load = load i8* %cast.113
  %b.5.load.1 = load %string** %b.5
  %cast.114 = getelementptr inbounds %string* %b.5.load.1, i32 0, i32 2
  %i.1.load.2 = load i32* %i.1
  %cast.114.load = load i8** %cast.114
  %cast.115 = getelementptr inbounds i8* %cast.114.load, i32 %i.1.load.2
  %cast.115.load = load i8* %cast.115
  %cmp.5 = icmp ne i8 %cast.113.load, %cast.115.load
  br i1 %cmp.5, label %if.2, label %if.end.2

if.2:                                             ; preds = %for.do.1
  ret i1 false
                                                  ; No predecessors!
  br label %if.end.2

if.end.2:                                         ; preds = %1, %for.do.1
  br label %for.inc.1

for.inc.1:                                        ; preds = %if.end.2
  %i.1.load.3 = load i32* %i.1
  %add.13 = add i32 %i.1.load.3, 1
  store i32 %add.13, i32* %i.1
  br label %for.1

for.end.1:                                        ; preds = %for.1
  ret i1 true
                                                  ; No predecessors!
  ret i1 undef
}

; Function Attrs: inlinehint
define i1 @binary.eq.1(%string* %a.arg, i8 %b.arg) #2 {
code:
  %a.6 = alloca %string*
  %b.6 = alloca i8
  store %string* undef, %string** %a.6
  store %string* %a.arg, %string** %a.6
  store i8 undef, i8* %b.6
  store i8 %b.arg, i8* %b.6
  %a.6.load = load %string** %a.6
  %cast.116 = getelementptr inbounds %string* %a.6.load, i32 0, i32 1
  %cast.116.load = load i32* %cast.116
  %cmp.6 = icmp eq i32 %cast.116.load, 1
  ret i1 %cmp.6
                                                  ; No predecessors!
  %a.6.load.1 = load %string** %a.6
  %cast.117 = getelementptr inbounds %string* %a.6.load.1, i32 0, i32 2
  %cast.117.load = load i8** %cast.117
  %cast.118 = getelementptr inbounds i8* %cast.117.load, i32 0
  %cast.118.load = load i8* %cast.118
  %b.6.load = load i8* %b.6
  %cmp.7 = icmp eq i8 %cast.118.load, %b.6.load
  ret i1 undef
}

define void @main() {
code:
  %i.2 = alloca i32
  store i32 undef, i32* %i.2
  store i32 0, i32* %i.2
  ret void
}

; Function Attrs: nounwind
define i8* @mmap(i8* %addr, i32 %len, i32 %prot, i32 %flags, i32 %fildes, i32 %off) #3 {
entry:
  %retval = alloca i8*, align 4
  %addr.addr = alloca i8*, align 4
  %len.addr = alloca i32, align 4
  %prot.addr = alloca i32, align 4
  %flags.addr = alloca i32, align 4
  %fildes.addr = alloca i32, align 4
  %off.addr = alloca i32, align 4
  %fm = alloca i8*, align 4
  %h = alloca i8*, align 4
  %map = alloca i8*, align 4
  %dwFileOffsetLow = alloca i32, align 4
  %dwFileOffsetHigh = alloca i32, align 4
  %protect = alloca i32, align 4
  %desiredAccess = alloca i32, align 4
  store i8* %addr, i8** %addr.addr, align 4
  store i32 %len, i32* %len.addr, align 4
  store i32 %prot, i32* %prot.addr, align 4
  store i32 %flags, i32* %flags.addr, align 4
  store i32 %fildes, i32* %fildes.addr, align 4
  store i32 %off, i32* %off.addr, align 4
  store i8* inttoptr (i32 -1 to i8*), i8** %map, align 4
  %0 = load i32* %off.addr, align 4
  store i32 %0, i32* %dwFileOffsetLow, align 4
  store i32 0, i32* %dwFileOffsetHigh, align 4
  %1 = load i32* %prot.addr, align 4
  %call = call i32 @__map_mmap_prot_page(i32 %1)
  store i32 %call, i32* %protect, align 4
  %2 = load i32* %prot.addr, align 4
  %call1 = call i32 @__map_mmap_prot_file(i32 %2)
  store i32 %call1, i32* %desiredAccess, align 4
  %call2 = call i32* @_errno() #0
  store i32 0, i32* %call2, align 4
  %3 = load i32* %len.addr, align 4
  %cmp = icmp eq i32 %3, 0
  br i1 %cmp, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %entry
  %4 = load i32* %flags.addr, align 4
  %and = and i32 %4, 16
  %cmp3 = icmp ne i32 %and, 0
  br i1 %cmp3, label %if.then, label %lor.lhs.false4

lor.lhs.false4:                                   ; preds = %lor.lhs.false
  %5 = load i32* %prot.addr, align 4
  %cmp5 = icmp eq i32 %5, 4
  br i1 %cmp5, label %if.then, label %if.end

if.then:                                          ; preds = %lor.lhs.false4, %lor.lhs.false, %entry
  %call6 = call i32* @_errno() #0
  store i32 22, i32* %call6, align 4
  store i8* inttoptr (i32 -1 to i8*), i8** %retval
  br label %return

if.end:                                           ; preds = %lor.lhs.false4
  %6 = load i32* %flags.addr, align 4
  %and7 = and i32 %6, 32
  %cmp8 = icmp eq i32 %and7, 0
  br i1 %cmp8, label %cond.true, label %cond.false

cond.true:                                        ; preds = %if.end
  %7 = load i32* %fildes.addr, align 4
  %call9 = call i32 @_get_osfhandle(i32 %7) #0
  %8 = inttoptr i32 %call9 to i8*
  br label %cond.end

cond.false:                                       ; preds = %if.end
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i8* [ %8, %cond.true ], [ inttoptr (i32 -1 to i8*), %cond.false ]
  store i8* %cond, i8** %h, align 4
  %9 = load i32* %flags.addr, align 4
  %and10 = and i32 %9, 32
  %cmp11 = icmp eq i32 %and10, 0
  br i1 %cmp11, label %land.lhs.true, label %if.end15

land.lhs.true:                                    ; preds = %cond.end
  %10 = load i8** %h, align 4
  %cmp12 = icmp eq i8* %10, inttoptr (i32 -1 to i8*)
  br i1 %cmp12, label %if.then13, label %if.end15

if.then13:                                        ; preds = %land.lhs.true
  %call14 = call i32* @_errno() #0
  store i32 9, i32* %call14, align 4
  store i8* inttoptr (i32 -1 to i8*), i8** %retval
  br label %return

if.end15:                                         ; preds = %land.lhs.true, %cond.end
  %11 = load i8** %h, align 4
  %12 = load i32* %protect, align 4
  %13 = load i32* %len.addr, align 4
  %call16 = call x86_stdcallcc i8* @"\01_CreateFileMappingA@24"(i8* %11, %struct._SECURITY_ATTRIBUTES* null, i32 %12, i32 0, i32 %13, i8* null)
  store i8* %call16, i8** %fm, align 4
  %14 = load i8** %fm, align 4
  %cmp17 = icmp eq i8* %14, null
  br i1 %cmp17, label %if.then18, label %if.end22

if.then18:                                        ; preds = %if.end15
  %call19 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %call20 = call i32 @__map_mman_error(i32 %call19, i32 1)
  %call21 = call i32* @_errno() #0
  store i32 %call20, i32* %call21, align 4
  store i8* inttoptr (i32 -1 to i8*), i8** %retval
  br label %return

if.end22:                                         ; preds = %if.end15
  %15 = load i8** %fm, align 4
  %16 = load i32* %desiredAccess, align 4
  %17 = load i32* %dwFileOffsetLow, align 4
  %18 = load i32* %len.addr, align 4
  %call23 = call x86_stdcallcc i8* @"\01_MapViewOfFile@20"(i8* %15, i32 %16, i32 0, i32 %17, i32 %18)
  store i8* %call23, i8** %map, align 4
  %19 = load i8** %fm, align 4
  %call24 = call x86_stdcallcc i32 @"\01_CloseHandle@4"(i8* %19)
  %20 = load i8** %map, align 4
  %cmp25 = icmp eq i8* %20, null
  br i1 %cmp25, label %if.then26, label %if.end30

if.then26:                                        ; preds = %if.end22
  %call27 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %call28 = call i32 @__map_mman_error(i32 %call27, i32 1)
  %call29 = call i32* @_errno() #0
  store i32 %call28, i32* %call29, align 4
  store i8* inttoptr (i32 -1 to i8*), i8** %retval
  br label %return

if.end30:                                         ; preds = %if.end22
  %21 = load i8** %map, align 4
  store i8* %21, i8** %retval
  br label %return

return:                                           ; preds = %if.end30, %if.then26, %if.then18, %if.then13, %if.then
  %22 = load i8** %retval
  ret i8* %22
}

; Function Attrs: nounwind
declare i32* @_errno() #3

; Function Attrs: nounwind
declare i32 @_get_osfhandle(i32) #3

declare x86_stdcallcc i8* @"\01_CreateFileMappingA@24"(i8*, %struct._SECURITY_ATTRIBUTES*, i32, i32, i32, i8*) #4

declare x86_stdcallcc i32 @"\01_GetLastError@0"() #4

declare x86_stdcallcc i8* @"\01_MapViewOfFile@20"(i8*, i32, i32, i32, i32) #4

declare x86_stdcallcc i32 @"\01_CloseHandle@4"(i8*) #4

; Function Attrs: nounwind
define i32 @munmap(i8* %addr, i32 %len) #3 {
entry:
  %retval = alloca i32, align 4
  %addr.addr = alloca i8*, align 4
  %len.addr = alloca i32, align 4
  store i8* %addr, i8** %addr.addr, align 4
  store i32 %len, i32* %len.addr, align 4
  %0 = load i8** %addr.addr, align 4
  %call = call x86_stdcallcc i32 @"\01_UnmapViewOfFile@4"(i8* %0)
  %tobool = icmp ne i32 %call, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %call1 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %call2 = call i32 @__map_mman_error(i32 %call1, i32 1)
  %call3 = call i32* @_errno() #0
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %1 = load i32* %retval
  ret i32 %1
}

declare x86_stdcallcc i32 @"\01_UnmapViewOfFile@4"(i8*) #4

; Function Attrs: nounwind
define i32 @mprotect(i8* %addr, i32 %len, i32 %prot) #3 {
entry:
  %retval = alloca i32, align 4
  %addr.addr = alloca i8*, align 4
  %len.addr = alloca i32, align 4
  %prot.addr = alloca i32, align 4
  %newProtect = alloca i32, align 4
  %oldProtect = alloca i32, align 4
  store i8* %addr, i8** %addr.addr, align 4
  store i32 %len, i32* %len.addr, align 4
  store i32 %prot, i32* %prot.addr, align 4
  %0 = load i32* %prot.addr, align 4
  %call = call i32 @__map_mmap_prot_page(i32 %0)
  store i32 %call, i32* %newProtect, align 4
  store i32 0, i32* %oldProtect, align 4
  %1 = load i8** %addr.addr, align 4
  %2 = load i32* %len.addr, align 4
  %3 = load i32* %newProtect, align 4
  %call1 = call x86_stdcallcc i32 @"\01_VirtualProtect@16"(i8* %1, i32 %2, i32 %3, i32* %oldProtect)
  %tobool = icmp ne i32 %call1, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %call2 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %call3 = call i32 @__map_mman_error(i32 %call2, i32 1)
  %call4 = call i32* @_errno() #0
  store i32 %call3, i32* %call4, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %4 = load i32* %retval
  ret i32 %4
}

declare x86_stdcallcc i32 @"\01_VirtualProtect@16"(i8*, i32, i32, i32*) #4

; Function Attrs: nounwind
define i32 @msync(i8* %addr, i32 %len, i32 %flags) #3 {
entry:
  %retval = alloca i32, align 4
  %addr.addr = alloca i8*, align 4
  %len.addr = alloca i32, align 4
  %flags.addr = alloca i32, align 4
  store i8* %addr, i8** %addr.addr, align 4
  store i32 %len, i32* %len.addr, align 4
  store i32 %flags, i32* %flags.addr, align 4
  %0 = load i8** %addr.addr, align 4
  %1 = load i32* %len.addr, align 4
  %call = call x86_stdcallcc i32 @"\01_FlushViewOfFile@8"(i8* %0, i32 %1)
  %tobool = icmp ne i32 %call, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %call1 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %call2 = call i32 @__map_mman_error(i32 %call1, i32 1)
  %call3 = call i32* @_errno() #0
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load i32* %retval
  ret i32 %2
}

declare x86_stdcallcc i32 @"\01_FlushViewOfFile@8"(i8*, i32) #4

; Function Attrs: nounwind
define i32 @mlock(i8* %addr, i32 %len) #3 {
entry:
  %retval = alloca i32, align 4
  %addr.addr = alloca i8*, align 4
  %len.addr = alloca i32, align 4
  store i8* %addr, i8** %addr.addr, align 4
  store i32 %len, i32* %len.addr, align 4
  %0 = load i8** %addr.addr, align 4
  %1 = load i32* %len.addr, align 4
  %call = call x86_stdcallcc i32 @"\01_VirtualLock@8"(i8* %0, i32 %1)
  %tobool = icmp ne i32 %call, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %call1 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %call2 = call i32 @__map_mman_error(i32 %call1, i32 1)
  %call3 = call i32* @_errno() #0
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load i32* %retval
  ret i32 %2
}

declare x86_stdcallcc i32 @"\01_VirtualLock@8"(i8*, i32) #4

; Function Attrs: nounwind
define i32 @munlock(i8* %addr, i32 %len) #3 {
entry:
  %retval = alloca i32, align 4
  %addr.addr = alloca i8*, align 4
  %len.addr = alloca i32, align 4
  store i8* %addr, i8** %addr.addr, align 4
  store i32 %len, i32* %len.addr, align 4
  %0 = load i8** %addr.addr, align 4
  %1 = load i32* %len.addr, align 4
  %call = call x86_stdcallcc i32 @"\01_VirtualUnlock@8"(i8* %0, i32 %1)
  %tobool = icmp ne i32 %call, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %call1 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %call2 = call i32 @__map_mman_error(i32 %call1, i32 1)
  %call3 = call i32* @_errno() #0
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load i32* %retval
  ret i32 %2
}

declare x86_stdcallcc i32 @"\01_VirtualUnlock@8"(i8*, i32) #4

; Function Attrs: nounwind
define internal i32 @__map_mmap_prot_page(i32 %prot) #3 {
entry:
  %retval = alloca i32, align 4
  %prot.addr = alloca i32, align 4
  %protect = alloca i32, align 4
  store i32 %prot, i32* %prot.addr, align 4
  store i32 0, i32* %protect, align 4
  %0 = load i32* %prot.addr, align 4
  %cmp = icmp eq i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load i32* %protect, align 4
  store i32 %1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %2 = load i32* %prot.addr, align 4
  %and = and i32 %2, 4
  %cmp1 = icmp ne i32 %and, 0
  br i1 %cmp1, label %if.then2, label %if.else

if.then2:                                         ; preds = %if.end
  %3 = load i32* %prot.addr, align 4
  %and3 = and i32 %3, 2
  %cmp4 = icmp ne i32 %and3, 0
  %cond = select i1 %cmp4, i32 64, i32 32
  store i32 %cond, i32* %protect, align 4
  br label %if.end8

if.else:                                          ; preds = %if.end
  %4 = load i32* %prot.addr, align 4
  %and5 = and i32 %4, 2
  %cmp6 = icmp ne i32 %and5, 0
  %cond7 = select i1 %cmp6, i32 4, i32 2
  store i32 %cond7, i32* %protect, align 4
  br label %if.end8

if.end8:                                          ; preds = %if.else, %if.then2
  %5 = load i32* %protect, align 4
  store i32 %5, i32* %retval
  br label %return

return:                                           ; preds = %if.end8, %if.then
  %6 = load i32* %retval
  ret i32 %6
}

; Function Attrs: nounwind
define internal i32 @__map_mmap_prot_file(i32 %prot) #3 {
entry:
  %retval = alloca i32, align 4
  %prot.addr = alloca i32, align 4
  %desiredAccess = alloca i32, align 4
  store i32 %prot, i32* %prot.addr, align 4
  store i32 0, i32* %desiredAccess, align 4
  %0 = load i32* %prot.addr, align 4
  %cmp = icmp eq i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load i32* %desiredAccess, align 4
  store i32 %1, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %2 = load i32* %prot.addr, align 4
  %and = and i32 %2, 1
  %cmp1 = icmp ne i32 %and, 0
  br i1 %cmp1, label %if.then2, label %if.end3

if.then2:                                         ; preds = %if.end
  %3 = load i32* %desiredAccess, align 4
  %or = or i32 %3, 4
  store i32 %or, i32* %desiredAccess, align 4
  br label %if.end3

if.end3:                                          ; preds = %if.then2, %if.end
  %4 = load i32* %prot.addr, align 4
  %and4 = and i32 %4, 2
  %cmp5 = icmp ne i32 %and4, 0
  br i1 %cmp5, label %if.then6, label %if.end8

if.then6:                                         ; preds = %if.end3
  %5 = load i32* %desiredAccess, align 4
  %or7 = or i32 %5, 2
  store i32 %or7, i32* %desiredAccess, align 4
  br label %if.end8

if.end8:                                          ; preds = %if.then6, %if.end3
  %6 = load i32* %prot.addr, align 4
  %and9 = and i32 %6, 4
  %cmp10 = icmp ne i32 %and9, 0
  br i1 %cmp10, label %if.then11, label %if.end13

if.then11:                                        ; preds = %if.end8
  %7 = load i32* %desiredAccess, align 4
  %or12 = or i32 %7, 32
  store i32 %or12, i32* %desiredAccess, align 4
  br label %if.end13

if.end13:                                         ; preds = %if.then11, %if.end8
  %8 = load i32* %desiredAccess, align 4
  store i32 %8, i32* %retval
  br label %return

return:                                           ; preds = %if.end13, %if.then
  %9 = load i32* %retval
  ret i32 %9
}

; Function Attrs: nounwind
define internal i32 @__map_mman_error(i32 %err, i32 %deferr) #3 {
entry:
  %retval = alloca i32, align 4
  %err.addr = alloca i32, align 4
  %deferr.addr = alloca i32, align 4
  store i32 %err, i32* %err.addr, align 4
  store i32 %deferr, i32* %deferr.addr, align 4
  %0 = load i32* %err.addr, align 4
  %cmp = icmp eq i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %1 = load i32* %err.addr, align 4
  store i32 %1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load i32* %retval
  ret i32 %2
}

attributes #0 = { nounwind }
attributes #1 = { nounwind readnone }
attributes #2 = { inlinehint }
attributes #3 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (198054)"}
