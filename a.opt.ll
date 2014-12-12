; ModuleID = 'a.out.ll'
target datalayout = "e-i64:64-f80:128-n8:16:32:64-S128"

%Allocator = type { i8, i8*, i64, i64, i64 }
%string = type { i8, i32, i8* }
%LinearAllocator = type { i8, i8*, i8*, i64, i64, i64 }
%StackAllocator = type { i8, %StackHeader, i8*, i8*, i8*, i64, i64, i64 }
%StackHeader = type { i8, i8*, i8 }
%FreeListAllocator = type { i8, %FreeListHeader, %FreeBlock*, i8*, i64, i64, i64 }
%FreeListHeader = type { i8, i64, i8, i32 }
%FreeBlock = type { i8, i64, %FreeBlock* }
%PoolAllocator = type { i8, i64, i8, i8**, i8*, i64, i64, i64 }

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

; Function Attrs: inlinehint nounwind
define void @createAllocator(%Allocator* %alloc.arg, i64 %size.arg, i8* %start.arg) #1 {
code:
  %cast = getelementptr inbounds %Allocator* %alloc.arg, i64 0, i32 1
  store i8* %start.arg, i8** %cast, align 8
  %cast.1 = getelementptr inbounds %Allocator* %alloc.arg, i64 0, i32 2
  store i64 %size.arg, i64* %cast.1, align 8
  %cast.2 = getelementptr inbounds %Allocator* %alloc.arg, i64 0, i32 3
  %0 = bitcast i64* %cast.2 to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 16, i32 8, i1 false)
  ret void
}

; Function Attrs: inlinehint nounwind readonly
define i32 @strLen(i8* nocapture readonly %str.arg) #2 {
code:
  %cast.4.load = load i8* %str.arg, align 1
  %cmp = icmp eq i8 %cast.4.load, 0
  br i1 %cmp, label %while.end, label %while.preheader

while.preheader:                                  ; preds = %code
  br label %while

while:                                            ; preds = %while.preheader, %while
  br label %while

while.end:                                        ; preds = %code
  ret i32 0
}

; Function Attrs: inlinehint
define void @setLength(%string* %s.arg, i32 %size.arg) #3 {
code:
  %ss = alloca %string*, align 8
  %cast.5 = getelementptr inbounds %string* %s.arg, i64 0, i32 1
  %cast.5.load = load i32* %cast.5, align 4
  %cmp.1 = icmp eq i32 %cast.5.load, 0
  %add.1 = add i32 %size.arg, 1
  br i1 %cmp.1, label %if, label %if.else

if:                                               ; preds = %code
  store i32 %add.1, i32* %cast.5, align 4
  %cast.7 = sext i32 %add.1 to i64
  %new.1 = tail call noalias i8* @new(i64 %cast.7)
  %cast.11 = getelementptr inbounds %string* %s.arg, i64 0, i32 2
  store i8* %new.1, i8** %cast.11, align 8
  br label %if.end

if.else:                                          ; preds = %code
  %add.4 = add i32 %add.1, %cast.5.load
  %cast.13 = sext i32 %add.4 to i64
  %new.2 = tail call noalias i8* @new(i64 %cast.13)
  %cast.18.load1 = load i32* %cast.5, align 4
  %cmp.22 = icmp slt i32 %cast.18.load1, 0
  br i1 %cmp.22, label %for.end, label %for.do.lr.ph

for.do.lr.ph:                                     ; preds = %if.else
  %cast.19 = getelementptr inbounds %string* %s.arg, i64 0, i32 2
  %cast.19.load = load i8** %cast.19, align 8
  br label %for.do

for.do:                                           ; preds = %for.do.lr.ph, %for.do
  %storemerge3 = phi i32 [ 0, %for.do.lr.ph ], [ %add.5, %for.do ]
  %0 = sext i32 %storemerge3 to i64
  %cast.20 = getelementptr inbounds i8* %cast.19.load, i64 %0
  %cast.20.load = load i8* %cast.20, align 1
  %cast.22 = getelementptr inbounds i8* %new.2, i64 %0
  store i8 %cast.20.load, i8* %cast.22, align 1
  %add.5 = add i32 %storemerge3, 1
  %cmp.2 = icmp sgt i32 %add.5, %cast.18.load1
  br i1 %cmp.2, label %for.end.loopexit, label %for.do

for.end.loopexit:                                 ; preds = %for.do
  br label %for.end

for.end:                                          ; preds = %for.end.loopexit, %if.else
  %cast.23 = bitcast %string** %ss to i8*
  call void @delete(i8* %cast.23)
  br label %if.end

if.end:                                           ; preds = %for.end, %if
  ret void
}

; Function Attrs: inlinehint
define void @binary.add(%string* nocapture sret %result.arg, %string* %a.arg, %string* %b.arg) #3 {
setLength.exit:
  %cast.24 = getelementptr inbounds %string* %a.arg, i64 0, i32 1
  %cast.24.load = load i32* %cast.24, align 4
  %cast.25 = getelementptr inbounds %string* %b.arg, i64 0, i32 1
  %cast.25.load = load i32* %cast.25, align 4
  %add.6 = add i32 %cast.24.load, 1
  %add.1.i = add i32 %add.6, %cast.25.load
  %cast.7.i = sext i32 %add.1.i to i64
  %new.1.i = tail call noalias i8* @new(i64 %cast.7.i)
  %cast.28 = getelementptr inbounds %string* %a.arg, i64 0, i32 2
  %cast.28.load = load i8** %cast.28, align 8
  %cast.30.load = load i32* %cast.24, align 4
  %cast.31 = sext i32 %cast.30.load to i64
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %new.1.i, i8* %cast.28.load, i64 %cast.31, i32 4, i1 false)
  %cast.34 = getelementptr inbounds i8* %new.1.i, i64 %cast.31
  %cast.35 = getelementptr inbounds %string* %b.arg, i64 0, i32 2
  %cast.35.load = load i8** %cast.35, align 8
  %cast.37.load = load i32* %cast.25, align 4
  %cast.38 = sext i32 %cast.37.load to i64
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.34, i8* %cast.35.load, i64 %cast.38, i32 4, i1 false)
  %cast.42 = getelementptr inbounds i8* %new.1.i, i64 %cast.7.i
  store i8 0, i8* %cast.42, align 1
  %0 = insertvalue %string undef, i32 %add.1.i, 1
  %1 = insertvalue %string %0, i8* %new.1.i, 2
  store %string %1, %string* %result.arg, align 8
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.1(%string* nocapture sret %result.arg, %string* %a.arg, i8 %b.arg) #3 {
setLength.exit:
  %cast.43 = getelementptr inbounds %string* %a.arg, i64 0, i32 1
  %cast.43.load = load i32* %cast.43, align 4
  %add.7 = add i32 %cast.43.load, 1
  %add.1.i = add i32 %cast.43.load, 2
  %cast.7.i = sext i32 %add.1.i to i64
  %new.1.i = tail call noalias i8* @new(i64 %cast.7.i)
  %cast.46 = getelementptr inbounds %string* %a.arg, i64 0, i32 2
  %cast.46.load = load i8** %cast.46, align 8
  %cast.48.load = load i32* %cast.43, align 4
  %add.8 = add i32 %cast.48.load, 1
  %cast.49 = sext i32 %add.8 to i64
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %new.1.i, i8* %cast.46.load, i64 %cast.49, i32 4, i1 false)
  %0 = sext i32 %add.7 to i64
  %cast.53 = getelementptr inbounds i8* %new.1.i, i64 %0
  store i8 %b.arg, i8* %cast.53, align 1
  %cast.57 = getelementptr inbounds i8* %new.1.i, i64 %cast.7.i
  store i8 0, i8* %cast.57, align 1
  %1 = insertvalue %string undef, i32 %add.1.i, 1
  %2 = insertvalue %string %1, i8* %new.1.i, 2
  store %string %2, %string* %result.arg, align 8
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.2(%string* nocapture sret %result.arg, i8 %a.arg, %string* %b.arg) #3 {
setLength.exit:
  %cast.58 = sext i8 %a.arg to i32
  %add.1.i = add nsw i32 %cast.58, 2
  %cast.7.i = sext i32 %add.1.i to i64
  %new.1.i = tail call noalias i8* @new(i64 %cast.7.i)
  %cast.60 = getelementptr inbounds i8* %new.1.i, i64 1
  %cast.61 = getelementptr inbounds %string* %b.arg, i64 0, i32 2
  %cast.61.load = load i8** %cast.61, align 8
  %cast.63 = getelementptr inbounds %string* %b.arg, i64 0, i32 1
  %cast.63.load = load i32* %cast.63, align 4
  %add.10 = add i32 %cast.63.load, 1
  %cast.64 = sext i32 %add.10 to i64
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %cast.60, i8* %cast.61.load, i64 %cast.64, i32 4, i1 false)
  store i8 %a.arg, i8* %new.1.i, align 1
  %cast.70 = getelementptr inbounds i8* %new.1.i, i64 %cast.7.i
  store i8 0, i8* %cast.70, align 1
  %0 = insertvalue %string undef, i32 %add.1.i, 1
  %1 = insertvalue %string %0, i8* %new.1.i, 2
  store %string %1, %string* %result.arg, align 8
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.3(%string* nocapture sret %result.arg, %string* %a.arg, i8* %b.arg) #3 {
code:
  %cast.71 = getelementptr inbounds %string* %a.arg, i64 0, i32 1
  %cast.4.load.i = load i8* %b.arg, align 1
  %cmp.i = icmp eq i8 %cast.4.load.i, 0
  br i1 %cmp.i, label %setLength.exit, label %while.i.preheader

while.i.preheader:                                ; preds = %code
  br label %while.i

while.i:                                          ; preds = %while.i.preheader, %while.i
  br label %while.i

setLength.exit:                                   ; preds = %code
  %cast.71.load = load i32* %cast.71, align 4
  %add.1.i = add i32 %cast.71.load, 1
  %cast.7.i = sext i32 %add.1.i to i64
  %new.1.i = tail call noalias i8* @new(i64 %cast.7.i)
  %cast.74 = getelementptr inbounds %string* %a.arg, i64 0, i32 2
  %cast.74.load = load i8** %cast.74, align 8
  %cast.76.load = load i32* %cast.71, align 4
  %cast.77 = sext i32 %cast.76.load to i64
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %new.1.i, i8* %cast.74.load, i64 %cast.77, i32 4, i1 false)
  %cast.4.load.i1 = load i8* %b.arg, align 1
  %cmp.i2 = icmp eq i8 %cast.4.load.i1, 0
  br i1 %cmp.i2, label %strLen.exit4, label %while.i3.preheader

while.i3.preheader:                               ; preds = %setLength.exit
  br label %while.i3

while.i3:                                         ; preds = %while.i3.preheader, %while.i3
  br label %while.i3

strLen.exit4:                                     ; preds = %setLength.exit
  %cast.85 = getelementptr inbounds i8* %new.1.i, i64 %cast.7.i
  store i8 0, i8* %cast.85, align 1
  %0 = insertvalue %string undef, i32 %add.1.i, 1
  %1 = insertvalue %string %0, i8* %new.1.i, 2
  store %string %1, %string* %result.arg, align 8
  ret void
}

; Function Attrs: inlinehint
define void @binary.add.4(%string* nocapture sret %result.arg, i8* %a.arg, %string* %b.arg) #3 {
code:
  %cast.86 = getelementptr inbounds %string* %b.arg, i64 0, i32 1
  %cast.4.load.i = load i8* %a.arg, align 1
  %cmp.i = icmp eq i8 %cast.4.load.i, 0
  br i1 %cmp.i, label %setLength.exit, label %while.i.preheader

while.i.preheader:                                ; preds = %code
  br label %while.i

while.i:                                          ; preds = %while.i.preheader, %while.i
  br label %while.i

setLength.exit:                                   ; preds = %code
  %cast.86.load = load i32* %cast.86, align 4
  %add.1.i = add i32 %cast.86.load, 1
  %cast.7.i = sext i32 %add.1.i to i64
  %new.1.i = tail call noalias i8* @new(i64 %cast.7.i)
  %cast.4.load.i5 = load i8* %a.arg, align 1
  %cmp.i6 = icmp eq i8 %cast.4.load.i5, 0
  br i1 %cmp.i6, label %strLen.exit4, label %while.i7.preheader

while.i7.preheader:                               ; preds = %setLength.exit
  br label %while.i7

while.i7:                                         ; preds = %while.i7.preheader, %while.i7
  br label %while.i7

strLen.exit4:                                     ; preds = %setLength.exit
  %cast.92 = getelementptr inbounds %string* %b.arg, i64 0, i32 2
  %cast.92.load = load i8** %cast.92, align 8
  %cast.94.load = load i32* %cast.86, align 4
  %cast.95 = sext i32 %cast.94.load to i64
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* %new.1.i, i8* %cast.92.load, i64 %cast.95, i32 4, i1 false)
  %cast.99 = getelementptr inbounds i8* %new.1.i, i64 %cast.7.i
  store i8 0, i8* %cast.99, align 1
  %0 = insertvalue %string undef, i32 %add.1.i, 1
  %1 = insertvalue %string %0, i8* %new.1.i, 2
  store %string %1, %string* %result.arg, align 8
  ret void
}

; Function Attrs: inlinehint nounwind
define void @assign.idx(%string* nocapture readonly %id.arg, i8 %ch.arg, i32 %idx.arg) #1 {
code:
  %cast.100 = getelementptr inbounds %string* %id.arg, i64 0, i32 2
  %cast.100.load = load i8** %cast.100, align 8
  %0 = sext i32 %idx.arg to i64
  %cast.101 = getelementptr inbounds i8* %cast.100.load, i64 %0
  store i8 %ch.arg, i8* %cast.101, align 1
  ret void
}

; Function Attrs: inlinehint
define void @assign(%string* %id.arg, i8 %val.arg) #3 {
code:
  %ss.i = alloca %string*, align 8
  %0 = bitcast %string** %ss.i to i8*
  call void @llvm.lifetime.start(i64 8, i8* %0)
  %cast.5.i = getelementptr inbounds %string* %id.arg, i64 0, i32 1
  %cast.5.load.i = load i32* %cast.5.i, align 4
  %cmp.1.i = icmp eq i32 %cast.5.load.i, 0
  br i1 %cmp.1.i, label %if.i, label %if.else.i

if.i:                                             ; preds = %code
  store i32 2, i32* %cast.5.i, align 4
  %new.1.i = tail call noalias i8* @new(i64 2)
  %cast.11.i = getelementptr inbounds %string* %id.arg, i64 0, i32 2
  store i8* %new.1.i, i8** %cast.11.i, align 8
  br label %setLength.exit

if.else.i:                                        ; preds = %code
  %add.4.i = add i32 %cast.5.load.i, 2
  %cast.13.i = sext i32 %add.4.i to i64
  %new.2.i = tail call noalias i8* @new(i64 %cast.13.i)
  %cast.18.load1.i = load i32* %cast.5.i, align 4
  %cmp.22.i = icmp slt i32 %cast.18.load1.i, 0
  %cast.102.phi.trans.insert.pre = getelementptr inbounds %string* %id.arg, i64 0, i32 2
  br i1 %cmp.22.i, label %for.end.i, label %for.do.lr.ph.i

for.do.lr.ph.i:                                   ; preds = %if.else.i
  %cast.19.load.i = load i8** %cast.102.phi.trans.insert.pre, align 8
  br label %for.do.i

for.do.i:                                         ; preds = %for.do.i, %for.do.lr.ph.i
  %storemerge3.i = phi i32 [ 0, %for.do.lr.ph.i ], [ %add.5.i, %for.do.i ]
  %1 = sext i32 %storemerge3.i to i64
  %cast.20.i = getelementptr inbounds i8* %cast.19.load.i, i64 %1
  %cast.20.load.i = load i8* %cast.20.i, align 1
  %cast.22.i = getelementptr inbounds i8* %new.2.i, i64 %1
  store i8 %cast.20.load.i, i8* %cast.22.i, align 1
  %add.5.i = add i32 %storemerge3.i, 1
  %cmp.2.i = icmp sgt i32 %add.5.i, %cast.18.load1.i
  br i1 %cmp.2.i, label %for.end.i.loopexit, label %for.do.i

for.end.i.loopexit:                               ; preds = %for.do.i
  br label %for.end.i

for.end.i:                                        ; preds = %for.end.i.loopexit, %if.else.i
  call void @delete(i8* %0)
  %cast.102.load.pre = load i8** %cast.102.phi.trans.insert.pre, align 8
  br label %setLength.exit

setLength.exit:                                   ; preds = %if.i, %for.end.i
  %cast.102.load = phi i8* [ %new.1.i, %if.i ], [ %cast.102.load.pre, %for.end.i ]
  call void @llvm.lifetime.end(i64 8, i8* %0)
  %cast.102 = getelementptr inbounds %string* %id.arg, i64 0, i32 2
  store i8 %val.arg, i8* %cast.102.load, align 1
  %cast.104.load = load i8** %cast.102, align 8
  %cast.105 = getelementptr inbounds i8* %cast.104.load, i64 1
  store i8 0, i8* %cast.105, align 1
  ret void
}

; Function Attrs: inlinehint
define void @assign.1(%string* %id.arg, i8* %val.arg) #3 {
code:
  %ss.i = alloca %string*, align 8
  %cast.4.load.i = load i8* %val.arg, align 1
  %cmp.i = icmp eq i8 %cast.4.load.i, 0
  br i1 %cmp.i, label %strLen.exit, label %while.i.preheader

while.i.preheader:                                ; preds = %code
  br label %while.i

while.i:                                          ; preds = %while.i.preheader, %while.i
  br label %while.i

strLen.exit:                                      ; preds = %code
  %0 = bitcast %string** %ss.i to i8*
  call void @llvm.lifetime.start(i64 8, i8* %0)
  %cast.5.i = getelementptr inbounds %string* %id.arg, i64 0, i32 1
  %cast.5.load.i = load i32* %cast.5.i, align 4
  %cmp.1.i = icmp eq i32 %cast.5.load.i, 0
  br i1 %cmp.1.i, label %if.i, label %if.else.i

if.i:                                             ; preds = %strLen.exit
  store i32 1, i32* %cast.5.i, align 4
  %new.1.i = tail call noalias i8* @new(i64 1)
  %cast.11.i = getelementptr inbounds %string* %id.arg, i64 0, i32 2
  store i8* %new.1.i, i8** %cast.11.i, align 8
  br label %setLength.exit

if.else.i:                                        ; preds = %strLen.exit
  %add.4.i = add i32 %cast.5.load.i, 1
  %cast.13.i = sext i32 %add.4.i to i64
  %new.2.i = tail call noalias i8* @new(i64 %cast.13.i)
  %cast.18.load1.i = load i32* %cast.5.i, align 4
  %cmp.22.i = icmp slt i32 %cast.18.load1.i, 0
  %cast.107.phi.trans.insert.pre = getelementptr inbounds %string* %id.arg, i64 0, i32 2
  br i1 %cmp.22.i, label %for.end.i, label %for.do.lr.ph.i

for.do.lr.ph.i:                                   ; preds = %if.else.i
  %cast.19.load.i = load i8** %cast.107.phi.trans.insert.pre, align 8
  br label %for.do.i

for.do.i:                                         ; preds = %for.do.i, %for.do.lr.ph.i
  %storemerge3.i = phi i32 [ 0, %for.do.lr.ph.i ], [ %add.5.i, %for.do.i ]
  %1 = sext i32 %storemerge3.i to i64
  %cast.20.i = getelementptr inbounds i8* %cast.19.load.i, i64 %1
  %cast.20.load.i = load i8* %cast.20.i, align 1
  %cast.22.i = getelementptr inbounds i8* %new.2.i, i64 %1
  store i8 %cast.20.load.i, i8* %cast.22.i, align 1
  %add.5.i = add i32 %storemerge3.i, 1
  %cmp.2.i = icmp sgt i32 %add.5.i, %cast.18.load1.i
  br i1 %cmp.2.i, label %for.end.i.loopexit, label %for.do.i

for.end.i.loopexit:                               ; preds = %for.do.i
  br label %for.end.i

for.end.i:                                        ; preds = %for.end.i.loopexit, %if.else.i
  call void @delete(i8* %0)
  %cast.107.load.pre = load i8** %cast.107.phi.trans.insert.pre, align 8
  br label %setLength.exit

setLength.exit:                                   ; preds = %if.i, %for.end.i
  %cast.107.load = phi i8* [ %new.1.i, %if.i ], [ %cast.107.load.pre, %for.end.i ]
  call void @llvm.lifetime.end(i64 8, i8* %0)
  %cast.108.load = load i32* %cast.5.i, align 4
  %2 = sext i32 %cast.108.load to i64
  %cast.109 = getelementptr inbounds i8* %cast.107.load, i64 %2
  store i8 0, i8* %cast.109, align 1
  %cast.4.load.i1 = load i8* %val.arg, align 1
  %cmp.i2 = icmp eq i8 %cast.4.load.i1, 0
  br i1 %cmp.i2, label %strLen.exit4, label %while.i3.preheader

while.i3.preheader:                               ; preds = %setLength.exit
  br label %while.i3

while.i3:                                         ; preds = %while.i3.preheader, %while.i3
  br label %while.i3

strLen.exit4:                                     ; preds = %setLength.exit
  ret void
}

; Function Attrs: inlinehint nounwind readonly
define i1 @binary.eq(%string* %a.arg, %string* %b.arg) #2 {
code:
  %cast.113 = getelementptr inbounds %string* %a.arg, i64 0, i32 1
  %cast.113.load = load i32* %cast.113, align 4
  %cast.114 = getelementptr inbounds %string* %b.arg, i64 0, i32 1
  %cast.114.load = load i32* %cast.114, align 4
  %cmp.3 = icmp eq i32 %cast.113.load, %cast.114.load
  br i1 %cmp.3, label %for.1.preheader, label %if.1

for.1.preheader:                                  ; preds = %code
  %cmp.42 = icmp slt i32 %cast.113.load, 0
  br i1 %cmp.42, label %if.1, label %for.do.1.lr.ph

for.do.1.lr.ph:                                   ; preds = %for.1.preheader
  %cast.116 = getelementptr inbounds %string* %a.arg, i64 0, i32 2
  %cast.116.load = load i8** %cast.116, align 8
  %cast.118 = getelementptr inbounds %string* %b.arg, i64 0, i32 2
  %cast.118.load = load i8** %cast.118, align 8
  br label %for.do.1

if.1.loopexit:                                    ; preds = %for.1, %for.do.1
  %merge.ph = phi i1 [ true, %for.1 ], [ false, %for.do.1 ]
  br label %if.1

if.1:                                             ; preds = %if.1.loopexit, %for.1.preheader, %code
  %merge = phi i1 [ false, %code ], [ true, %for.1.preheader ], [ %merge.ph, %if.1.loopexit ]
  ret i1 %merge

for.1:                                            ; preds = %for.do.1
  %cmp.4 = icmp sgt i32 %add.13, %cast.113.load
  br i1 %cmp.4, label %if.1.loopexit, label %for.do.1

for.do.1:                                         ; preds = %for.do.1.lr.ph, %for.1
  %storemerge3 = phi i32 [ 0, %for.do.1.lr.ph ], [ %add.13, %for.1 ]
  %0 = sext i32 %storemerge3 to i64
  %cast.117 = getelementptr inbounds i8* %cast.116.load, i64 %0
  %cast.117.load = load i8* %cast.117, align 1
  %cast.119 = getelementptr inbounds i8* %cast.118.load, i64 %0
  %cast.119.load = load i8* %cast.119, align 1
  %cmp.5 = icmp eq i8 %cast.117.load, %cast.119.load
  %add.13 = add i32 %storemerge3, 1
  br i1 %cmp.5, label %for.1, label %if.1.loopexit
}

; Function Attrs: inlinehint nounwind readonly
define i1 @binary.eq.1(%string* nocapture readonly %a.arg, i8 %b.arg) #2 {
code:
  %cast.120 = getelementptr inbounds %string* %a.arg, i64 0, i32 1
  %cast.120.load = load i32* %cast.120, align 4
  %cmp.6 = icmp eq i32 %cast.120.load, 1
  ret i1 %cmp.6
}

; Function Attrs: inlinehint nounwind readnone
define i8* @alignForward(i8* %address.arg, i8 %alignment.arg) #4 {
code:
  %cast.123 = ptrtoint i8* %address.arg to i64
  %cast.1241 = zext i8 %alignment.arg to i64
  %sub.2 = add nuw nsw i64 %cast.1241, 1
  %xor = icmp eq i8 %alignment.arg, 1
  %cast.127 = zext i1 %xor to i64
  %and = and i64 %sub.2, %cast.127
  %add.14 = add i64 %and, %cast.123
  %cast.128 = inttoptr i64 %add.14 to i8*
  ret i8* %cast.128
}

; Function Attrs: inlinehint nounwind readnone
define i8* @alignBackward(i8* %address.arg, i8 %alignment.arg) #4 {
code:
  %cast.129 = ptrtoint i8* %address.arg to i64
  %xor.1 = icmp eq i8 %alignment.arg, 1
  %cast.131 = zext i1 %xor.1 to i64
  %and.1 = and i64 %cast.131, %cast.129
  %cast.132 = inttoptr i64 %and.1 to i8*
  ret i8* %cast.132
}

; Function Attrs: inlinehint nounwind readnone
define i8 @alignForwardAdjustment(i8* %address.arg, i8 %alignment.arg) #4 {
code:
  %cast.1331 = zext i8 %alignment.arg to i64
  %cast.134 = ptrtoint i8* %address.arg to i64
  %sub.5 = add nuw nsw i64 %cast.1331, 255
  %and.2 = and i64 %sub.5, %cast.134
  %sub.6 = sub i64 %cast.1331, %and.2
  %cast.137 = trunc i64 %sub.6 to i8
  %cmp.10 = icmp eq i8 %cast.137, %alignment.arg
  %.cast.137 = select i1 %cmp.10, i8 0, i8 %cast.137
  ret i8 %.cast.137
}

; Function Attrs: inlinehint nounwind readnone
define i8 @alignForwardAdjustmentWithHeader(i8* %address.arg, i8 %alignment.arg, i8 %headerSize.arg) #4 {
code:
  %cast.1331.i = zext i8 %alignment.arg to i64
  %cast.134.i = ptrtoint i8* %address.arg to i64
  %sub.5.i = add nuw nsw i64 %cast.1331.i, 255
  %and.2.i = and i64 %sub.5.i, %cast.134.i
  %sub.6.i = sub i64 %cast.1331.i, %and.2.i
  %cast.137.i = trunc i64 %sub.6.i to i8
  %cmp.10.i = icmp eq i8 %cast.137.i, %alignment.arg
  %.cast.137.i = select i1 %cmp.10.i, i8 0, i8 %cast.137.i
  %cmp.11 = icmp ult i8 %.cast.137.i, %headerSize.arg
  br i1 %cmp.11, label %if.4, label %if.end.4

if.4:                                             ; preds = %code
  %sub.7 = sub i8 %headerSize.arg, %.cast.137.i
  %div = srem i8 %sub.7, %alignment.arg
  %cmp.12 = icmp sgt i8 %div, 0
  %add.16 = select i1 %cmp.12, i8 %alignment.arg, i8 0
  %add.15 = sub i8 %headerSize.arg, %div
  %add.16.add.15 = add i8 %add.15, %add.16
  ret i8 %add.16.add.15

if.end.4:                                         ; preds = %code
  ret i8 %.cast.137.i
}

; Function Attrs: inlinehint nounwind readnone
define i8 @alignBackwardAdjustment(i8* %address.arg, i8 %alignment.arg) #4 {
code:
  %cast.139 = ptrtoint i8* %address.arg to i64
  %cast.1401 = zext i8 %alignment.arg to i64
  %sub.8 = add nuw nsw i64 %cast.1401, 255
  %and.3 = and i64 %sub.8, %cast.139
  %cast.142 = trunc i64 %and.3 to i8
  %cmp.13 = icmp eq i8 %cast.142, %alignment.arg
  %.cast.142 = select i1 %cmp.13, i8 0, i8 %cast.142
  ret i8 %.cast.142
}

; Function Attrs: inlinehint nounwind readnone
define i8* @ptrAdd(i8* %p.arg, i64 %x.arg) #4 {
code:
  %cast.143 = ptrtoint i8* %p.arg to i64
  %add.17 = add i64 %cast.143, %x.arg
  %cast.144 = inttoptr i64 %add.17 to i8*
  ret i8* %cast.144
}

; Function Attrs: inlinehint nounwind readnone
define i8* @ptrSub(i8* %p.arg, i64 %x.arg) #4 {
code:
  %cast.145 = ptrtoint i8* %p.arg to i64
  %sub.9 = sub i64 %cast.145, %x.arg
  %cast.146 = inttoptr i64 %sub.9 to i8*
  ret i8* %cast.146
}

; Function Attrs: inlinehint nounwind
define void @createLinearAllocator(%LinearAllocator* %allocator.arg, i64 %size.arg, i8* %start.arg) #1 {
code:
  %0 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 1
  %cast.1.i = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 2
  %size.arg.c = inttoptr i64 %size.arg to i8*
  store i8* %size.arg.c, i8** %cast.1.i, align 8
  %1 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 3
  %2 = bitcast i64* %1 to i8*
  tail call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 16, i32 8, i1 false) #0
  store i8* %start.arg, i8** %0, align 8
  ret void
}

; Function Attrs: inlinehint nounwind readnone
define void @deleteLinearAllocator(%LinearAllocator* nocapture %allocator.arg) #4 {
code:
  ret void
}

; Function Attrs: inlinehint nounwind
define i8* @linearAllocate(%LinearAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg) #1 {
code:
  %cast.149 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 1
  %cast.149.load = load i8** %cast.149, align 8
  %cast.1331.i = zext i8 %alignment.arg to i64
  %cast.134.i = ptrtoint i8* %cast.149.load to i64
  %sub.5.i = add nuw nsw i64 %cast.1331.i, 255
  %and.2.i = and i64 %cast.134.i, %sub.5.i
  %sub.6.i = sub i64 %cast.1331.i, %and.2.i
  %cast.137.i = trunc i64 %sub.6.i to i8
  %cmp.10.i = icmp eq i8 %cast.137.i, %alignment.arg
  %cast.150 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 4
  %cast.150.load = load i64* %cast.150, align 8
  %sext = shl i64 %sub.6.i, 56
  %0 = ashr exact i64 %sext, 56
  %cast.151 = select i1 %cmp.10.i, i64 0, i64 %0
  %add.18 = add i64 %cast.150.load, %size.arg
  %add.19 = add i64 %add.18, %cast.151
  %cast.152 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 3
  %cast.152.load = load i64* %cast.152, align 8
  %cmp.14 = icmp ugt i64 %add.19, %cast.152.load
  br i1 %cmp.14, label %if.7, label %if.end.7

if.7:                                             ; preds = %code
  ret i8* null

if.end.7:                                         ; preds = %code
  %add.20 = add i64 %cast.151, %cast.134.i
  %add.21 = add i64 %add.20, %size.arg
  %cast.156 = inttoptr i64 %add.21 to i8*
  store i8* %cast.156, i8** %cast.149, align 8
  %add.22 = add i64 %cast.151, %size.arg
  %add.23 = add i64 %add.22, %cast.150.load
  %1 = bitcast i64* %cast.150 to <2 x i64>*
  %cast.161 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 5
  %cast.161.load = load i64* %cast.161, align 8
  %add.24 = add i64 %cast.161.load, 1
  %2 = insertelement <2 x i64> undef, i64 %add.23, i32 0
  %3 = insertelement <2 x i64> %2, i64 %add.24, i32 1
  store <2 x i64> %3, <2 x i64>* %1, align 8
  %cast.163 = inttoptr i64 %add.20 to i8*
  ret i8* %cast.163
}

; Function Attrs: inlinehint nounwind
define void @clearLinearAllocator(%LinearAllocator* %allocator.arg) #1 {
code:
  %cast.165 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 4
  %cast.166 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 2
  %0 = bitcast i64* %cast.165 to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 16, i32 8, i1 false)
  %cast.166.load = load i8** %cast.166, align 8
  %cast.167 = getelementptr inbounds %LinearAllocator* %allocator.arg, i64 0, i32 1
  store i8* %cast.166.load, i8** %cast.167, align 8
  ret void
}

; Function Attrs: inlinehint nounwind
define void @createStackAllocator(%StackAllocator* %allocator.arg, i64 %size.arg, i8* %start.arg) #1 {
code:
  %cast.i = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 1
  %0 = bitcast %StackHeader* %cast.i to i8**
  store i8* %start.arg, i8** %0, align 8
  %cast.1.i = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 1, i32 1
  %size.arg.c = inttoptr i64 %size.arg to i8*
  store i8* %size.arg.c, i8** %cast.1.i, align 8
  %cast.2.i = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 1, i32 2
  call void @llvm.memset.p0i8.i64(i8* %cast.2.i, i8 0, i64 16, i32 8, i1 false)
  ret void
}

; Function Attrs: inlinehint nounwind readnone
define void @deleteStackAllocator(%StackAllocator* nocapture %allocator.arg) #4 {
code:
  ret void
}

; Function Attrs: inlinehint nounwind
define i8* @stackAllocate(%StackAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg) #1 {
code:
  %cast.170 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 3
  %cast.170.load = load i8** %cast.170, align 8
  %cast.1331.i.i = zext i8 %alignment.arg to i64
  %cast.134.i.i = ptrtoint i8* %cast.170.load to i64
  %sub.5.i.i = add nuw nsw i64 %cast.1331.i.i, 255
  %and.2.i.i = and i64 %cast.134.i.i, %sub.5.i.i
  %sub.6.i.i = sub i64 %cast.1331.i.i, %and.2.i.i
  %cast.137.i.i = trunc i64 %sub.6.i.i to i8
  %cmp.10.i.i = icmp eq i8 %cast.137.i.i, %alignment.arg
  %.cast.137.i.i = select i1 %cmp.10.i.i, i8 0, i8 %cast.137.i.i
  %cmp.11.i = icmp ult i8 %.cast.137.i.i, 9
  br i1 %cmp.11.i, label %if.4.i, label %alignForwardAdjustmentWithHeader.exit

if.4.i:                                           ; preds = %code
  %sub.7.i = sub i8 9, %.cast.137.i.i
  %div.i = srem i8 %sub.7.i, %alignment.arg
  %cmp.12.i = icmp sgt i8 %div.i, 0
  %add.16.i = select i1 %cmp.12.i, i8 %alignment.arg, i8 0
  %add.15.i = sub i8 9, %div.i
  %add.16.add.15.i = add i8 %add.15.i, %add.16.i
  br label %alignForwardAdjustmentWithHeader.exit

alignForwardAdjustmentWithHeader.exit:            ; preds = %code, %if.4.i
  %call.91 = phi i8 [ %add.16.add.15.i, %if.4.i ], [ %.cast.137.i.i, %code ]
  %cast.171 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 6
  %cast.171.load = load i64* %cast.171, align 8
  %cast.172 = sext i8 %call.91 to i64
  %add.25 = add i64 %cast.172, %size.arg
  %add.26 = add i64 %add.25, %cast.171.load
  %cast.173 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 5
  %cast.173.load = load i64* %cast.173, align 8
  %cmp.15 = icmp ugt i64 %add.26, %cast.173.load
  br i1 %cmp.15, label %if.8, label %if.end.8

if.8:                                             ; preds = %alignForwardAdjustmentWithHeader.exit
  ret i8* null

if.end.8:                                         ; preds = %alignForwardAdjustmentWithHeader.exit
  %add.17.i3 = add i64 %cast.134.i.i, %cast.172
  %cast.144.i4 = inttoptr i64 %add.17.i3 to i8*
  %sub.9.i = add i64 %add.17.i3, -9
  %cast.146.i = inttoptr i64 %sub.9.i to i8*
  %cast.176 = inttoptr i64 %sub.9.i to %StackHeader*
  %0 = getelementptr inbounds i8* %cast.146.i, i64 16
  store i8 %call.91, i8* %0, align 1
  %cast.178 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 2
  %cast.178.load = load i8** %cast.178, align 8
  %cast.179 = getelementptr inbounds %StackHeader* %cast.176, i64 0, i32 1
  store i8* %cast.178.load, i8** %cast.179, align 8
  store i8* %cast.144.i4, i8** %cast.178, align 8
  %add.17.i = add i64 %add.17.i3, %size.arg
  %cast.144.i = inttoptr i64 %add.17.i to i8*
  store i8* %cast.144.i, i8** %cast.170, align 8
  %1 = bitcast i64* %cast.171 to <2 x i64>*
  %2 = load <2 x i64>* %1, align 8
  %3 = insertelement <2 x i64> undef, i64 %add.25, i32 0
  %4 = insertelement <2 x i64> %3, i64 1, i32 1
  %5 = add <2 x i64> %4, %2
  store <2 x i64> %5, <2 x i64>* %1, align 8
  ret i8* %cast.144.i4
}

; Function Attrs: inlinehint nounwind
define void @stackDeallocate(%StackAllocator* %allocator.arg, i8* %p.arg) #1 {
code:
  %cast.145.i = ptrtoint i8* %p.arg to i64
  %sub.9.i = add i64 %cast.145.i, -9
  %cast.187 = inttoptr i64 %sub.9.i to %StackHeader*
  %cast.188 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 6
  %cast.188.load = load i64* %cast.188, align 8
  %cast.189 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 3
  %cast.189.load = load i8** %cast.189, align 8
  %cast.190 = ptrtoint i8* %cast.189.load to i64
  %cast.192 = getelementptr inbounds %StackHeader* %cast.187, i64 0, i32 2
  %cast.192.load = load i8* %cast.192, align 1
  %cast.193 = sext i8 %cast.192.load to i64
  %sub.10.neg = add i64 %cast.188.load, %cast.145.i
  %add.30.neg = sub i64 %sub.10.neg, %cast.190
  %sub.11 = sub i64 %add.30.neg, %cast.193
  %0 = bitcast i64* %cast.188 to <2 x i64>*
  %cast.198 = getelementptr inbounds %StackHeader* %cast.187, i64 0, i32 1
  %cast.199 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 2
  %cast.200 = getelementptr inbounds %StackAllocator* %allocator.arg, i64 0, i32 7
  %cast.200.load = load i64* %cast.200, align 8
  %sub.12 = add i64 %cast.200.load, -1
  %1 = insertelement <2 x i64> undef, i64 %sub.11, i32 0
  %2 = insertelement <2 x i64> %1, i64 %sub.12, i32 1
  store <2 x i64> %2, <2 x i64>* %0, align 8
  %cast.195.load = load i8* %cast.192, align 1
  %cast.196 = sext i8 %cast.195.load to i64
  %sub.9.i2 = sub i64 %cast.145.i, %cast.196
  %cast.146.i3 = inttoptr i64 %sub.9.i2 to i8*
  store i8* %cast.146.i3, i8** %cast.189, align 8
  %cast.198.load = load i8** %cast.198, align 8
  store i8* %cast.198.load, i8** %cast.199, align 8
  ret void
}

; Function Attrs: inlinehint nounwind
define void @createFreeListAllocator(%FreeListAllocator* %allocator.arg, i64 %size.arg, i8* %start.arg) #1 {
code:
  %cast.i = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 1
  %0 = bitcast %FreeListHeader* %cast.i to i8**
  store i8* %start.arg, i8** %0, align 8
  %1 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 1, i32 1
  store i64 %size.arg, i64* %1, align 8
  %cast.2.i = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 1, i32 2
  tail call void @llvm.memset.p0i8.i64(i8* %cast.2.i, i8 0, i64 16, i32 8, i1 false) #0
  store i64 %size.arg, i64* inttoptr (i64 8 to i64*), align 8
  store %FreeBlock* null, %FreeBlock** inttoptr (i64 16 to %FreeBlock**), align 16
  ret void
}

; Function Attrs: inlinehint nounwind
define void @deleteFreeListAllocator(%FreeListAllocator* nocapture %allocator.arg) #1 {
code:
  %cast.207 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 2
  store %FreeBlock* null, %FreeBlock** %cast.207, align 8
  ret void
}

; Function Attrs: inlinehint nounwind readonly
define i8 @getSizeFreeList(%FreeListAllocator* nocapture readonly %allocator.arg, i64 %size.arg, i8 %alignment.arg) #2 {
code:
  %free_block = alloca %FreeBlock*, align 8
  %cast.208 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 2
  %storemerge3 = load %FreeBlock** %cast.208, align 8
  store %FreeBlock* %storemerge3, %FreeBlock** %free_block, align 8
  %cmp.164 = icmp eq %FreeBlock* %storemerge3, null
  br i1 %cmp.164, label %while.end.1, label %while.do.1.lr.ph

while.do.1.lr.ph:                                 ; preds = %code
  %cast.1331.i.i = zext i8 %alignment.arg to i64
  %cast.134.i.i = ptrtoint %FreeBlock** %free_block to i64
  %sub.5.i.i = add nuw nsw i64 %cast.1331.i.i, 255
  %and.2.i.i = and i64 %cast.134.i.i, %sub.5.i.i
  %sub.6.i.i = sub i64 %cast.1331.i.i, %and.2.i.i
  %cast.137.i.i = trunc i64 %sub.6.i.i to i8
  %cmp.10.i.i = icmp eq i8 %cast.137.i.i, %alignment.arg
  %.cast.137.i.i = select i1 %cmp.10.i.i, i8 0, i8 %cast.137.i.i
  %cmp.11.i = icmp ult i8 %.cast.137.i.i, 13
  %sub.7.i = sub i8 13, %.cast.137.i.i
  br i1 %cmp.11.i, label %alignForwardAdjustmentWithHeader.exit.us.preheader, label %alignForwardAdjustmentWithHeader.exit.preheader

alignForwardAdjustmentWithHeader.exit.preheader:  ; preds = %while.do.1.lr.ph
  br label %alignForwardAdjustmentWithHeader.exit

alignForwardAdjustmentWithHeader.exit.us.preheader: ; preds = %while.do.1.lr.ph
  br label %alignForwardAdjustmentWithHeader.exit.us

while.1.us:                                       ; preds = %alignForwardAdjustmentWithHeader.exit.us
  %cast.212.us = getelementptr inbounds %FreeBlock* %free_block.load.1.us, i64 0, i32 2
  %storemerge.us = load %FreeBlock** %cast.212.us, align 8
  store %FreeBlock* %storemerge.us, %FreeBlock** %free_block, align 8
  %cmp.16.us = icmp eq %FreeBlock* %storemerge.us, null
  br i1 %cmp.16.us, label %while.end.1.loopexit, label %alignForwardAdjustmentWithHeader.exit.us

alignForwardAdjustmentWithHeader.exit.us:         ; preds = %alignForwardAdjustmentWithHeader.exit.us.preheader, %while.1.us
  %free_block.load.1.us = phi %FreeBlock* [ %storemerge.us, %while.1.us ], [ %storemerge3, %alignForwardAdjustmentWithHeader.exit.us.preheader ]
  %div.i.us = srem i8 %sub.7.i, %alignment.arg
  %cmp.12.i.us = icmp sgt i8 %div.i.us, 0
  %add.16.i.us = select i1 %cmp.12.i.us, i8 %alignment.arg, i8 0
  %add.15.i.us = sub i8 13, %div.i.us
  %add.16.add.15.i.us = add i8 %add.15.i.us, %add.16.i.us
  %phitmp = sext i8 %add.16.add.15.i.us to i64
  %add.31.us = add i64 %phitmp, %size.arg
  %cast.211.us = getelementptr inbounds %FreeBlock* %free_block.load.1.us, i64 0, i32 1
  %cast.211.load.us = load i64* %cast.211.us, align 8
  %cmp.17.us = icmp ult i64 %cast.211.load.us, %add.31.us
  br i1 %cmp.17.us, label %while.1.us, label %if.end.9.loopexit

while.1:                                          ; preds = %alignForwardAdjustmentWithHeader.exit
  %cast.212 = getelementptr inbounds %FreeBlock* %free_block.load.1, i64 0, i32 2
  %storemerge = load %FreeBlock** %cast.212, align 8
  store %FreeBlock* %storemerge, %FreeBlock** %free_block, align 8
  %cmp.16 = icmp eq %FreeBlock* %storemerge, null
  br i1 %cmp.16, label %while.end.1.loopexit30, label %alignForwardAdjustmentWithHeader.exit

alignForwardAdjustmentWithHeader.exit:            ; preds = %alignForwardAdjustmentWithHeader.exit.preheader, %while.1
  %free_block.load.1 = phi %FreeBlock* [ %storemerge, %while.1 ], [ %storemerge3, %alignForwardAdjustmentWithHeader.exit.preheader ]
  %cast.210 = sext i8 %.cast.137.i.i to i64
  %add.31 = add i64 %cast.210, %size.arg
  %cast.211 = getelementptr inbounds %FreeBlock* %free_block.load.1, i64 0, i32 1
  %cast.211.load = load i64* %cast.211, align 8
  %cmp.17 = icmp ult i64 %cast.211.load, %add.31
  br i1 %cmp.17, label %while.1, label %if.end.9.loopexit29

if.end.9.loopexit:                                ; preds = %alignForwardAdjustmentWithHeader.exit.us
  %cast.211.load.us.lcssa = phi i64 [ %cast.211.load.us, %alignForwardAdjustmentWithHeader.exit.us ]
  %add.31.us.lcssa = phi i64 [ %add.31.us, %alignForwardAdjustmentWithHeader.exit.us ]
  br label %if.end.9

if.end.9.loopexit29:                              ; preds = %alignForwardAdjustmentWithHeader.exit
  %cast.211.load.lcssa35 = phi i64 [ %cast.211.load, %alignForwardAdjustmentWithHeader.exit ]
  %add.31.lcssa33 = phi i64 [ %add.31, %alignForwardAdjustmentWithHeader.exit ]
  br label %if.end.9

if.end.9:                                         ; preds = %if.end.9.loopexit29, %if.end.9.loopexit
  %cast.211.load.lcssa = phi i64 [ %cast.211.load.us.lcssa, %if.end.9.loopexit ], [ %cast.211.load.lcssa35, %if.end.9.loopexit29 ]
  %add.31.lcssa = phi i64 [ %add.31.us.lcssa, %if.end.9.loopexit ], [ %add.31.lcssa33, %if.end.9.loopexit29 ]
  %sub.13 = sub i64 %cast.211.load.lcssa, %add.31.lcssa
  %cmp.18 = icmp ult i64 %sub.13, 14
  %cast.211.load.add.31 = select i1 %cmp.18, i64 %cast.211.load.lcssa, i64 %add.31.lcssa
  %cast.215 = trunc i64 %cast.211.load.add.31 to i8
  ret i8 %cast.215

while.end.1.loopexit:                             ; preds = %while.1.us
  br label %while.end.1

while.end.1.loopexit30:                           ; preds = %while.1
  br label %while.end.1

while.end.1:                                      ; preds = %while.end.1.loopexit30, %while.end.1.loopexit, %code
  ret i8 undef
}

; Function Attrs: inlinehint nounwind
define i8* @freeListAllocate(%FreeListAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg, i32 %pageId.arg) #1 {
code:
  %free_block.1 = alloca %FreeBlock*, align 8
  %cast.216 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 2
  %storemerge4 = load %FreeBlock** %cast.216, align 8
  store %FreeBlock* %storemerge4, %FreeBlock** %free_block.1, align 8
  %cmp.195 = icmp eq %FreeBlock* %storemerge4, null
  br i1 %cmp.195, label %while.end.2, label %while.do.2.lr.ph

while.do.2.lr.ph:                                 ; preds = %code
  %cast.1331.i.i = zext i8 %alignment.arg to i64
  %cast.134.i.i = ptrtoint %FreeBlock** %free_block.1 to i64
  %sub.5.i.i = add nuw nsw i64 %cast.1331.i.i, 255
  %and.2.i.i = and i64 %cast.134.i.i, %sub.5.i.i
  %sub.6.i.i = sub i64 %cast.1331.i.i, %and.2.i.i
  %cast.137.i.i = trunc i64 %sub.6.i.i to i8
  %cmp.10.i.i = icmp eq i8 %cast.137.i.i, %alignment.arg
  %.cast.137.i.i = select i1 %cmp.10.i.i, i8 0, i8 %cast.137.i.i
  %cmp.11.i = icmp ult i8 %.cast.137.i.i, 13
  %sub.7.i = sub i8 13, %.cast.137.i.i
  br i1 %cmp.11.i, label %alignForwardAdjustmentWithHeader.exit.us.preheader, label %alignForwardAdjustmentWithHeader.exit.preheader

alignForwardAdjustmentWithHeader.exit.preheader:  ; preds = %while.do.2.lr.ph
  br label %alignForwardAdjustmentWithHeader.exit

alignForwardAdjustmentWithHeader.exit.us.preheader: ; preds = %while.do.2.lr.ph
  br label %alignForwardAdjustmentWithHeader.exit.us

while.2.us:                                       ; preds = %alignForwardAdjustmentWithHeader.exit.us
  %cast.220.us = getelementptr inbounds %FreeBlock* %free_block.1.load.1.us, i64 0, i32 2
  %storemerge.us = load %FreeBlock** %cast.220.us, align 8
  store %FreeBlock* %storemerge.us, %FreeBlock** %free_block.1, align 8
  %cmp.19.us = icmp eq %FreeBlock* %storemerge.us, null
  br i1 %cmp.19.us, label %while.end.2.loopexit, label %alignForwardAdjustmentWithHeader.exit.us

alignForwardAdjustmentWithHeader.exit.us:         ; preds = %alignForwardAdjustmentWithHeader.exit.us.preheader, %while.2.us
  %free_block.1.load.1.us = phi %FreeBlock* [ %storemerge.us, %while.2.us ], [ %storemerge4, %alignForwardAdjustmentWithHeader.exit.us.preheader ]
  %0 = phi %FreeBlock* [ %free_block.1.load.1.us, %while.2.us ], [ null, %alignForwardAdjustmentWithHeader.exit.us.preheader ]
  %div.i.us = srem i8 %sub.7.i, %alignment.arg
  %cmp.12.i.us = icmp sgt i8 %div.i.us, 0
  %add.16.i.us = select i1 %cmp.12.i.us, i8 %alignment.arg, i8 0
  %add.15.i.us = sub i8 13, %div.i.us
  %add.16.add.15.i.us = add i8 %add.15.i.us, %add.16.i.us
  %phitmp = sext i8 %add.16.add.15.i.us to i64
  %add.32.us = add i64 %phitmp, %size.arg
  %cast.219.us = getelementptr inbounds %FreeBlock* %free_block.1.load.1.us, i64 0, i32 1
  %cast.219.load.us = load i64* %cast.219.us, align 8
  %cmp.20.us = icmp ult i64 %cast.219.load.us, %add.32.us
  br i1 %cmp.20.us, label %while.2.us, label %if.end.11.loopexit

while.2:                                          ; preds = %alignForwardAdjustmentWithHeader.exit
  %cast.220 = getelementptr inbounds %FreeBlock* %free_block.1.load.1, i64 0, i32 2
  %storemerge = load %FreeBlock** %cast.220, align 8
  store %FreeBlock* %storemerge, %FreeBlock** %free_block.1, align 8
  %cmp.19 = icmp eq %FreeBlock* %storemerge, null
  br i1 %cmp.19, label %while.end.2.loopexit57, label %alignForwardAdjustmentWithHeader.exit

alignForwardAdjustmentWithHeader.exit:            ; preds = %alignForwardAdjustmentWithHeader.exit.preheader, %while.2
  %free_block.1.load.1 = phi %FreeBlock* [ %storemerge, %while.2 ], [ %storemerge4, %alignForwardAdjustmentWithHeader.exit.preheader ]
  %1 = phi %FreeBlock* [ %free_block.1.load.1, %while.2 ], [ null, %alignForwardAdjustmentWithHeader.exit.preheader ]
  %cast.218 = sext i8 %.cast.137.i.i to i64
  %add.32 = add i64 %cast.218, %size.arg
  %cast.219 = getelementptr inbounds %FreeBlock* %free_block.1.load.1, i64 0, i32 1
  %cast.219.load = load i64* %cast.219, align 8
  %cmp.20 = icmp ult i64 %cast.219.load, %add.32
  br i1 %cmp.20, label %while.2, label %if.end.11.loopexit56

if.end.11.loopexit:                               ; preds = %alignForwardAdjustmentWithHeader.exit.us
  %cast.219.load.us.lcssa = phi i64 [ %cast.219.load.us, %alignForwardAdjustmentWithHeader.exit.us ]
  %add.32.us.lcssa = phi i64 [ %add.32.us, %alignForwardAdjustmentWithHeader.exit.us ]
  %phitmp.lcssa = phi i64 [ %phitmp, %alignForwardAdjustmentWithHeader.exit.us ]
  %.lcssa = phi %FreeBlock* [ %0, %alignForwardAdjustmentWithHeader.exit.us ]
  %free_block.1.load.1.us.lcssa = phi %FreeBlock* [ %free_block.1.load.1.us, %alignForwardAdjustmentWithHeader.exit.us ]
  br label %if.end.11

if.end.11.loopexit56:                             ; preds = %alignForwardAdjustmentWithHeader.exit
  %cast.219.load.lcssa71 = phi i64 [ %cast.219.load, %alignForwardAdjustmentWithHeader.exit ]
  %add.32.lcssa69 = phi i64 [ %add.32, %alignForwardAdjustmentWithHeader.exit ]
  %cast.218.lcssa67 = phi i64 [ %cast.218, %alignForwardAdjustmentWithHeader.exit ]
  %.lcssa65 = phi %FreeBlock* [ %1, %alignForwardAdjustmentWithHeader.exit ]
  %free_block.1.load.1.lcssa63 = phi %FreeBlock* [ %free_block.1.load.1, %alignForwardAdjustmentWithHeader.exit ]
  br label %if.end.11

if.end.11:                                        ; preds = %if.end.11.loopexit56, %if.end.11.loopexit
  %cast.219.load.lcssa = phi i64 [ %cast.219.load.us.lcssa, %if.end.11.loopexit ], [ %cast.219.load.lcssa71, %if.end.11.loopexit56 ]
  %free_block.1.load.1.lcssa = phi %FreeBlock* [ %free_block.1.load.1.us.lcssa, %if.end.11.loopexit ], [ %free_block.1.load.1.lcssa63, %if.end.11.loopexit56 ]
  %add.32.lcssa = phi i64 [ %add.32.us.lcssa, %if.end.11.loopexit ], [ %add.32.lcssa69, %if.end.11.loopexit56 ]
  %cast.218.lcssa = phi i64 [ %phitmp.lcssa, %if.end.11.loopexit ], [ %cast.218.lcssa67, %if.end.11.loopexit56 ]
  %.lcssa3 = phi %FreeBlock* [ %.lcssa, %if.end.11.loopexit ], [ %.lcssa65, %if.end.11.loopexit56 ]
  %sub.14 = sub i64 %cast.219.load.lcssa, %add.32.lcssa
  %cmp.21 = icmp ult i64 %sub.14, 14
  br i1 %cmp.21, label %if.12, label %if.else.1

if.12:                                            ; preds = %if.end.11
  %cmp.22 = icmp eq %FreeBlock* %.lcssa3, null
  %cast.225 = getelementptr inbounds %FreeBlock* %free_block.1.load.1.lcssa, i64 0, i32 2
  %cast.225.load = load %FreeBlock** %cast.225, align 8
  br i1 %cmp.22, label %if.else.2, label %if.13

if.13:                                            ; preds = %if.12
  %cast.224 = getelementptr inbounds %FreeBlock* %.lcssa3, i64 0, i32 2
  store %FreeBlock* %cast.225.load, %FreeBlock** %cast.224, align 8
  br label %if.end.12

if.else.2:                                        ; preds = %if.12
  store %FreeBlock* %cast.225.load, %FreeBlock** %cast.216, align 8
  br label %if.end.12

if.else.1:                                        ; preds = %if.end.11
  %add.17.i = add i64 %add.32.lcssa, %cast.134.i.i
  %cast.144.i = inttoptr i64 %add.17.i to i8*
  %cast.228 = inttoptr i64 %add.17.i to %FreeBlock*
  %cast.230 = getelementptr inbounds i8* %cast.144.i, i64 8
  %2 = bitcast i8* %cast.230 to i64*
  store i64 %sub.14, i64* %2, align 8
  %free_block.1.load.9 = load %FreeBlock** %free_block.1, align 8
  %cast.231 = getelementptr inbounds %FreeBlock* %free_block.1.load.9, i64 0, i32 2
  %cast.231.load = load %FreeBlock** %cast.231, align 8
  %cast.232 = getelementptr inbounds %FreeBlock* %cast.228, i64 0, i32 2
  store %FreeBlock* %cast.231.load, %FreeBlock** %cast.232, align 8
  %cmp.23 = icmp eq %FreeBlock* %.lcssa3, null
  br i1 %cmp.23, label %if.else.3, label %if.14

if.14:                                            ; preds = %if.else.1
  %cast.233 = getelementptr inbounds %FreeBlock* %.lcssa3, i64 0, i32 2
  store %FreeBlock* %cast.228, %FreeBlock** %cast.233, align 8
  br label %if.end.12

if.else.3:                                        ; preds = %if.else.1
  store %FreeBlock* %cast.228, %FreeBlock** %cast.216, align 8
  br label %if.end.12

if.end.12:                                        ; preds = %if.14, %if.else.3, %if.13, %if.else.2
  %add.322 = phi i64 [ %add.32.lcssa, %if.14 ], [ %add.32.lcssa, %if.else.3 ], [ %cast.219.load.lcssa, %if.13 ], [ %cast.219.load.lcssa, %if.else.2 ]
  %free_block.1.load.10 = load %FreeBlock** %free_block.1, align 8
  %cast.235 = ptrtoint %FreeBlock* %free_block.1.load.10 to i64
  %add.33 = add i64 %cast.235, %cast.218.lcssa
  %cast.237 = inttoptr i64 %add.33 to i8*
  %cast.240 = getelementptr inbounds i8* %cast.237, i64 -5
  %3 = bitcast i8* %cast.240 to i64*
  store i64 %add.322, i64* %3, align 8
  %cast.241 = trunc i64 %add.322 to i8
  %4 = getelementptr inbounds i8* %cast.237, i64 3
  store i8 %cast.241, i8* %4, align 1
  %cast.243 = getelementptr inbounds i8* %cast.237, i64 7
  %5 = bitcast i8* %cast.243 to i32*
  store i32 %pageId.arg, i32* %5, align 4
  %cast.244 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 5
  %6 = bitcast i64* %cast.244 to <2 x i64>*
  %7 = load <2 x i64>* %6, align 8
  %8 = insertelement <2 x i64> undef, i64 %add.322, i32 0
  %9 = insertelement <2 x i64> %8, i64 1, i32 1
  %10 = add <2 x i64> %7, %9
  store <2 x i64> %10, <2 x i64>* %6, align 8
  ret i8* %cast.237

while.end.2.loopexit:                             ; preds = %while.2.us
  br label %while.end.2

while.end.2.loopexit57:                           ; preds = %while.2
  br label %while.end.2

while.end.2:                                      ; preds = %while.end.2.loopexit57, %while.end.2.loopexit, %code
  ret i8* undef
}

; Function Attrs: inlinehint nounwind
define void @freeListDeallocate(%FreeListAllocator* %allocator.arg, i8* %p.arg) #1 {
code:
  %cast.145.i = ptrtoint i8* %p.arg to i64
  %sub.9.i = add i64 %cast.145.i, -13
  %cast.146.i = inttoptr i64 %sub.9.i to i8*
  %cast.249 = inttoptr i64 %sub.9.i to %FreeListHeader*
  %0 = getelementptr inbounds i8* %cast.146.i, i64 16
  %cast.251.load = load i8* %0, align 1
  %cast.252 = sext i8 %cast.251.load to i64
  %sub.17 = sub i64 %cast.145.i, %cast.252
  %cast.253 = getelementptr inbounds %FreeListHeader* %cast.249, i64 0, i32 1
  %cast.253.load = load i64* %cast.253, align 8
  %add.36 = add i64 %sub.17, %cast.253.load
  %cast.254 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 2
  br label %while.3

while.3:                                          ; preds = %while.3, %code
  %1 = phi %FreeBlock* [ %storemerge, %while.3 ], [ null, %code ]
  %storemerge.in = phi %FreeBlock** [ %cast.256, %while.3 ], [ %cast.254, %code ]
  %storemerge = load %FreeBlock** %storemerge.in, align 8
  %cmp.24 = icmp eq %FreeBlock* %storemerge, null
  %cmp.24.not = xor i1 %cmp.24, true
  %cast.255 = ptrtoint %FreeBlock* %storemerge to i64
  %cmp.25 = icmp ult i64 %cast.255, %add.36
  %or.cond = and i1 %cmp.25, %cmp.24.not
  %cast.256 = getelementptr inbounds %FreeBlock* %storemerge, i64 0, i32 2
  br i1 %or.cond, label %while.3, label %while.end.3

while.end.3:                                      ; preds = %while.3
  %cast.256.lcssa = phi %FreeBlock** [ %cast.256, %while.3 ]
  %cmp.24.lcssa = phi i1 [ %cmp.24, %while.3 ]
  %storemerge.lcssa = phi %FreeBlock* [ %storemerge, %while.3 ]
  %.lcssa = phi %FreeBlock* [ %1, %while.3 ]
  %cmp.26 = icmp eq %FreeBlock* %.lcssa, null
  br i1 %cmp.26, label %if.16, label %if.else.4

if.16:                                            ; preds = %while.end.3
  %cast.258 = inttoptr i64 %sub.17 to %FreeBlock*
  %cast.259 = getelementptr inbounds %FreeBlock* %cast.258, i64 0, i32 1
  store i64 %cast.253.load, i64* %cast.259, align 8
  %cast.260.load = load %FreeBlock** %cast.254, align 8
  %cast.261 = getelementptr inbounds %FreeBlock* %cast.258, i64 0, i32 2
  store %FreeBlock* %cast.260.load, %FreeBlock** %cast.261, align 8
  store %FreeBlock* %cast.258, %FreeBlock** %cast.254, align 8
  br label %if.end.16

if.else.4:                                        ; preds = %while.end.3
  %cast.263 = ptrtoint %FreeBlock* %.lcssa to i64
  %cast.264 = getelementptr inbounds %FreeBlock* %.lcssa, i64 0, i32 1
  %cast.264.load = load i64* %cast.264, align 8
  %add.37 = add i64 %cast.264.load, %cast.263
  %cmp.27 = icmp eq i64 %add.37, %sub.17
  br i1 %cmp.27, label %if.17, label %if.end.17

if.17:                                            ; preds = %if.else.4
  %add.38 = add i64 %cast.264.load, %cast.253.load
  store i64 %add.38, i64* %cast.264, align 8
  br label %if.end.17

if.end.17:                                        ; preds = %if.17, %if.else.4
  %cast.268 = inttoptr i64 %sub.17 to %FreeBlock*
  %cast.269 = getelementptr inbounds %FreeBlock* %cast.268, i64 0, i32 1
  store i64 %cast.253.load, i64* %cast.269, align 8
  %cast.270 = getelementptr inbounds %FreeBlock* %.lcssa, i64 0, i32 2
  %cast.270.load = load %FreeBlock** %cast.270, align 8
  %cast.271 = getelementptr inbounds %FreeBlock* %cast.268, i64 0, i32 2
  store %FreeBlock* %cast.270.load, %FreeBlock** %cast.271, align 8
  store %FreeBlock* %cast.268, %FreeBlock** %cast.270, align 8
  br label %if.end.16

if.end.16:                                        ; preds = %if.end.17, %if.16
  %2 = phi %FreeBlock* [ %cast.268, %if.end.17 ], [ %cast.258, %if.16 ]
  br i1 %cmp.24.lcssa, label %if.end.18, label %if.18

if.18:                                            ; preds = %if.end.16
  %cast.273 = getelementptr inbounds %FreeBlock* %2, i64 0, i32 1
  %cast.273.load = load i64* %cast.273, align 8
  %cast.274 = getelementptr inbounds %FreeBlock* %storemerge.lcssa, i64 0, i32 1
  %cast.274.load = load i64* %cast.274, align 8
  %add.39 = add i64 %cast.274.load, %cast.273.load
  store i64 %add.39, i64* %cast.273, align 8
  %cast.276.load = load %FreeBlock** %cast.256.lcssa, align 8
  %cast.277 = getelementptr inbounds %FreeBlock* %2, i64 0, i32 2
  store %FreeBlock* %cast.276.load, %FreeBlock** %cast.277, align 8
  br label %if.end.18

if.end.18:                                        ; preds = %if.end.16, %if.18
  %cast.278 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 6
  %cast.278.load = load i64* %cast.278, align 8
  %sub.18 = add i64 %cast.278.load, -1
  store i64 %sub.18, i64* %cast.278, align 8
  %cast.280 = getelementptr inbounds %FreeListAllocator* %allocator.arg, i64 0, i32 5
  %cast.280.load = load i64* %cast.280, align 8
  %sub.19 = sub i64 %cast.280.load, %cast.253.load
  store i64 %sub.19, i64* %cast.280, align 8
  ret void
}

; Function Attrs: inlinehint nounwind
define void @createPoolAllocator(%PoolAllocator* %allocator.arg, i64 %size.arg, i64 %objectSize.arg, i8 %objectAlignment.arg, i8* %start.arg) #1 {
code:
  %p.4 = alloca i8**, align 8
  %cast.i = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 1
  %start.arg.c = ptrtoint i8* %start.arg to i64
  store i64 %start.arg.c, i64* %cast.i, align 8
  %cast.1.i = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 2
  %0 = bitcast i8* %cast.1.i to i64*
  store i64 %size.arg, i64* %0, align 8
  %cast.2.i = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 3
  %1 = bitcast i8*** %cast.2.i to i8*
  tail call void @llvm.memset.p0i8.i64(i8* %1, i8 0, i64 16, i32 8, i1 false) #0
  %cast.1331.i = zext i8 %objectAlignment.arg to i64
  %sub.5.i = add nuw nsw i64 %cast.1331.i, 255
  %and.2.i = and i64 %sub.5.i, %start.arg.c
  %sub.6.i = sub i64 %cast.1331.i, %and.2.i
  %cast.137.i = trunc i64 %sub.6.i to i8
  %cmp.10.i = icmp eq i8 %cast.137.i, %objectAlignment.arg
  %sext = shl i64 %sub.6.i, 56
  %2 = ashr exact i64 %sext, 56
  %cast.283 = select i1 %cmp.10.i, i64 0, i64 %2
  %add.17.i2 = add i64 %cast.283, %start.arg.c
  %cast.284 = inttoptr i64 %add.17.i2 to i8**
  store i8** %cast.284, i8*** %cast.2.i, align 8
  %sub.20 = sub i64 %size.arg, %cast.283
  %div.1 = sdiv i64 %sub.20, %objectSize.arg
  store i8** %cast.284, i8*** %p.4, align 8
  %sub.21 = add i64 %div.1, -1
  %cmp.294 = icmp slt i64 %sub.21, 0
  br i1 %cmp.294, label %for.end.2, label %for.do.2.lr.ph

for.do.2.lr.ph:                                   ; preds = %code
  %cast.143.i = ptrtoint i8*** %p.4 to i64
  %add.17.i = add i64 %cast.143.i, %objectSize.arg
  %cast.144.i = inttoptr i64 %add.17.i to i8*
  br label %for.do.2

for.do.2:                                         ; preds = %for.do.2.lr.ph, %for.do.2
  %p.4.load = phi i8** [ %cast.284, %for.do.2.lr.ph ], [ %cast.289, %for.do.2 ]
  %storemerge5 = phi i64 [ 0, %for.do.2.lr.ph ], [ %add.40, %for.do.2 ]
  store i8* %cast.144.i, i8** %p.4.load, align 8
  %p.4.load.1 = load i8*** %p.4, align 8
  %p.4.load.1.load = load i8** %p.4.load.1, align 8
  %cast.289 = bitcast i8* %p.4.load.1.load to i8**
  store i8** %cast.289, i8*** %p.4, align 8
  %add.40 = add i64 %storemerge5, 1
  %cmp.29 = icmp sgt i64 %add.40, %sub.21
  br i1 %cmp.29, label %for.end.2.loopexit, label %for.do.2

for.end.2.loopexit:                               ; preds = %for.do.2
  %cast.289.lcssa = phi i8** [ %cast.289, %for.do.2 ]
  br label %for.end.2

for.end.2:                                        ; preds = %for.end.2.loopexit, %code
  %p.4.load.2 = phi i8** [ %cast.284, %code ], [ %cast.289.lcssa, %for.end.2.loopexit ]
  store i8* null, i8** %p.4.load.2, align 8
  ret void
}

; Function Attrs: inlinehint nounwind
define void @deletePoolAllocator(%PoolAllocator* nocapture %allocator.arg) #1 {
code:
  %cast.290 = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 3
  store i8** null, i8*** %cast.290, align 8
  ret void
}

; Function Attrs: inlinehint nounwind
define i8* @poolAllocate(%PoolAllocator* %allocator.arg, i64 %size.arg, i8 %alignment.arg) #1 {
code:
  %cast.291 = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 3
  %cast.291.load = load i8*** %cast.291, align 8
  %cmp.30 = icmp eq i8** %cast.291.load, null
  br i1 %cmp.30, label %if.19, label %if.end.19

if.19:                                            ; preds = %code
  ret i8* null

if.end.19:                                        ; preds = %code
  %cast.293 = bitcast i8*** %cast.291 to i8*
  %cast.294.load.load = load i8** %cast.291.load, align 8
  %cast.295 = bitcast i8* %cast.294.load.load to i8**
  store i8** %cast.295, i8*** %cast.291, align 8
  %cast.297 = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 6
  %0 = bitcast i64* %cast.297 to <2 x i64>*
  %1 = load <2 x i64>* %0, align 8
  %2 = insertelement <2 x i64> undef, i64 %size.arg, i32 0
  %3 = insertelement <2 x i64> %2, i64 1, i32 1
  %4 = add <2 x i64> %1, %3
  store <2 x i64> %4, <2 x i64>* %0, align 8
  ret i8* %cast.293
}

; Function Attrs: inlinehint nounwind
define void @poolDeallocate(%PoolAllocator* %allocator.arg, i8* %p.arg) #1 {
code:
  %cast.301 = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 3
  %cast.302 = bitcast i8*** %cast.301 to i8*
  %cast.303 = bitcast i8* %p.arg to i8**
  store i8* %cast.302, i8** %cast.303, align 8
  store i8** %cast.303, i8*** %cast.301, align 8
  %cast.306 = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 6
  %cast.306.load = load i64* %cast.306, align 8
  %cast.307 = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 1
  %cast.307.load = load i64* %cast.307, align 8
  %sub.22 = sub i64 %cast.306.load, %cast.307.load
  store i64 %sub.22, i64* %cast.306, align 8
  %cast.309 = getelementptr inbounds %PoolAllocator* %allocator.arg, i64 0, i32 7
  %cast.309.load = load i64* %cast.309, align 8
  %sub.23 = add i64 %cast.309.load, -1
  store i64 %sub.23, i64* %cast.309, align 8
  ret void
}

; Function Attrs: nounwind readnone
define void @main() #5 {
code:
  ret void
}

attributes #0 = { nounwind }
attributes #1 = { inlinehint nounwind }
attributes #2 = { inlinehint nounwind readonly }
attributes #3 = { inlinehint }
attributes #4 = { inlinehint nounwind readnone }
attributes #5 = { nounwind readnone }
