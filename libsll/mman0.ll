; ModuleID = 'mman.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-f80:128:128-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S32"
target triple = "i686-pc-mingw32"

%struct._SECURITY_ATTRIBUTES = type { i32, i8*, i32 }

; Function Attrs: nounwind
define i8* @mmap(i8* %addr, i32 %len, i32 %prot, i32 %flags, i32 %fildes, i32 %off) #0 {
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
  %call2 = call i32* @_errno() #2
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
  %call6 = call i32* @_errno() #2
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
  %call9 = call i32 @_get_osfhandle(i32 %7) #2
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
  %call14 = call i32* @_errno() #2
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
  %call21 = call i32* @_errno() #2
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
  %call29 = call i32* @_errno() #2
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
define internal i32 @__map_mmap_prot_page(i32 %prot) #0 {
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
define internal i32 @__map_mmap_prot_file(i32 %prot) #0 {
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
declare i32* @_errno() #0

; Function Attrs: nounwind
declare i32 @_get_osfhandle(i32) #0

declare x86_stdcallcc i8* @"\01_CreateFileMappingA@24"(i8*, %struct._SECURITY_ATTRIBUTES*, i32, i32, i32, i8*) #1

; Function Attrs: nounwind
define internal i32 @__map_mman_error(i32 %err, i32 %deferr) #0 {
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

declare x86_stdcallcc i32 @"\01_GetLastError@0"() #1

declare x86_stdcallcc i8* @"\01_MapViewOfFile@20"(i8*, i32, i32, i32, i32) #1

declare x86_stdcallcc i32 @"\01_CloseHandle@4"(i8*) #1

; Function Attrs: nounwind
define i32 @munmap(i8* %addr, i32 %len) #0 {
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
  %call3 = call i32* @_errno() #2
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %1 = load i32* %retval
  ret i32 %1
}

declare x86_stdcallcc i32 @"\01_UnmapViewOfFile@4"(i8*) #1

; Function Attrs: nounwind
define i32 @mprotect(i8* %addr, i32 %len, i32 %prot) #0 {
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
  %call4 = call i32* @_errno() #2
  store i32 %call3, i32* %call4, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %4 = load i32* %retval
  ret i32 %4
}

declare x86_stdcallcc i32 @"\01_VirtualProtect@16"(i8*, i32, i32, i32*) #1

; Function Attrs: nounwind
define i32 @msync(i8* %addr, i32 %len, i32 %flags) #0 {
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
  %call3 = call i32* @_errno() #2
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load i32* %retval
  ret i32 %2
}

declare x86_stdcallcc i32 @"\01_FlushViewOfFile@8"(i8*, i32) #1

; Function Attrs: nounwind
define i32 @mlock(i8* %addr, i32 %len) #0 {
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
  %call3 = call i32* @_errno() #2
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load i32* %retval
  ret i32 %2
}

declare x86_stdcallcc i32 @"\01_VirtualLock@8"(i8*, i32) #1

; Function Attrs: nounwind
define i32 @munlock(i8* %addr, i32 %len) #0 {
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
  %call3 = call i32* @_errno() #2
  store i32 %call2, i32* %call3, align 4
  store i32 -1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %2 = load i32* %retval
  ret i32 %2
}

declare x86_stdcallcc i32 @"\01_VirtualUnlock@8"(i8*, i32) #1

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (198054)"}
