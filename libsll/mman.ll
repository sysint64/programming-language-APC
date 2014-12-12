; ModuleID = 'mman.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-f80:128:128-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S32"
target triple = "i686-pc-mingw32"

%struct._SECURITY_ATTRIBUTES = type { i32, i8*, i32 }

; Function Attrs: nounwind
define i8* @mmap(i8* nocapture readnone %addr, i32 %len, i32 %prot, i32 %flags, i32 %fildes, i32 %off) #0 {
entry:
  %cmp.i = icmp eq i32 %prot, 0
  br i1 %cmp.i, label %__map_mmap_prot_file.exit, label %if.end.i

if.end.i:                                         ; preds = %entry
  %and.i = and i32 %prot, 4
  %cmp1.i = icmp eq i32 %and.i, 0
  %and3.i = and i32 %prot, 2
  br i1 %cmp1.i, label %__map_mmap_prot_page.exit.thread47, label %__map_mmap_prot_page.exit

__map_mmap_prot_page.exit.thread47:               ; preds = %if.end.i
  %0 = add i32 %and3.i, 2
  br label %if.end.i44

__map_mmap_prot_page.exit:                        ; preds = %if.end.i
  %1 = shl nuw nsw i32 %and3.i, 4
  %2 = add i32 %1, 32
  br label %if.end.i44

if.end.i44:                                       ; preds = %__map_mmap_prot_page.exit, %__map_mmap_prot_page.exit.thread47
  %retval.0.i48 = phi i32 [ %0, %__map_mmap_prot_page.exit.thread47 ], [ %2, %__map_mmap_prot_page.exit ]
  %and.i43 = shl i32 %prot, 2
  %3 = and i32 %and.i43, 4
  %4 = or i32 %3, %and3.i
  %and9.i = shl i32 %prot, 3
  %5 = and i32 %and9.i, 32
  %6 = or i32 %4, %5
  br label %__map_mmap_prot_file.exit

__map_mmap_prot_file.exit:                        ; preds = %entry, %if.end.i44
  %retval.0.i46 = phi i32 [ %retval.0.i48, %if.end.i44 ], [ 0, %entry ]
  %retval.0.i45 = phi i32 [ %6, %if.end.i44 ], [ 0, %entry ]
  %call2 = tail call i32* @_errno() #2
  store i32 0, i32* %call2, align 4, !tbaa !1
  %cmp = icmp eq i32 %len, 0
  br i1 %cmp, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %__map_mmap_prot_file.exit
  %and = and i32 %flags, 16
  %cmp3 = icmp ne i32 %and, 0
  %cmp5 = icmp eq i32 %prot, 4
  %or.cond = or i1 %cmp3, %cmp5
  br i1 %or.cond, label %if.then, label %if.end

if.then:                                          ; preds = %lor.lhs.false, %__map_mmap_prot_file.exit
  %call6 = tail call i32* @_errno() #2
  store i32 22, i32* %call6, align 4, !tbaa !1
  br label %return

if.end:                                           ; preds = %lor.lhs.false
  %and7 = and i32 %flags, 32
  %cmp8 = icmp eq i32 %and7, 0
  br i1 %cmp8, label %cond.end, label %if.end15

cond.end:                                         ; preds = %if.end
  %call9 = tail call i32 @_get_osfhandle(i32 %fildes) #2
  %7 = inttoptr i32 %call9 to i8*
  %cmp12 = icmp eq i8* %7, inttoptr (i32 -1 to i8*)
  br i1 %cmp12, label %if.then13, label %if.end15

if.then13:                                        ; preds = %cond.end
  %call14 = tail call i32* @_errno() #2
  store i32 9, i32* %call14, align 4, !tbaa !1
  br label %return

if.end15:                                         ; preds = %if.end, %cond.end
  %cond51 = phi i8* [ %7, %cond.end ], [ inttoptr (i32 -1 to i8*), %if.end ]
  %call16 = tail call x86_stdcallcc i8* @"\01_CreateFileMappingA@24"(i8* %cond51, %struct._SECURITY_ATTRIBUTES* null, i32 %retval.0.i46, i32 0, i32 %len, i8* null) #2
  %cmp17 = icmp eq i8* %call16, null
  br i1 %cmp17, label %if.then18, label %if.end22

if.then18:                                        ; preds = %if.end15
  %call19 = tail call x86_stdcallcc i32 @"\01_GetLastError@0"() #2
  %call21 = tail call i32* @_errno() #2
  store i32 %call19, i32* %call21, align 4, !tbaa !1
  br label %return

if.end22:                                         ; preds = %if.end15
  %call23 = tail call x86_stdcallcc i8* @"\01_MapViewOfFile@20"(i8* %call16, i32 %retval.0.i45, i32 0, i32 %off, i32 %len) #2
  %call24 = tail call x86_stdcallcc i32 @"\01_CloseHandle@4"(i8* %call16) #2
  %cmp25 = icmp eq i8* %call23, null
  br i1 %cmp25, label %if.then26, label %return

if.then26:                                        ; preds = %if.end22
  %call27 = tail call x86_stdcallcc i32 @"\01_GetLastError@0"() #2
  %call29 = tail call i32* @_errno() #2
  store i32 %call27, i32* %call29, align 4, !tbaa !1
  br label %return

return:                                           ; preds = %if.end22, %if.then26, %if.then18, %if.then13, %if.then
  %retval.0 = phi i8* [ inttoptr (i32 -1 to i8*), %if.then ], [ inttoptr (i32 -1 to i8*), %if.then13 ], [ inttoptr (i32 -1 to i8*), %if.then18 ], [ inttoptr (i32 -1 to i8*), %if.then26 ], [ %call23, %if.end22 ]
  ret i8* %retval.0
}

; Function Attrs: nounwind
declare i32* @_errno() #0

; Function Attrs: nounwind
declare i32 @_get_osfhandle(i32) #0

declare x86_stdcallcc i8* @"\01_CreateFileMappingA@24"(i8*, %struct._SECURITY_ATTRIBUTES*, i32, i32, i32, i8*) #1

declare x86_stdcallcc i32 @"\01_GetLastError@0"() #1

declare x86_stdcallcc i8* @"\01_MapViewOfFile@20"(i8*, i32, i32, i32, i32) #1

declare x86_stdcallcc i32 @"\01_CloseHandle@4"(i8*) #1

; Function Attrs: nounwind
define i32 @munmap(i8* %addr, i32 %len) #0 {
entry:
  %call = tail call x86_stdcallcc i32 @"\01_UnmapViewOfFile@4"(i8* %addr) #2
  %tobool = icmp eq i32 %call, 0
  br i1 %tobool, label %if.end, label %return

if.end:                                           ; preds = %entry
  %call1 = tail call x86_stdcallcc i32 @"\01_GetLastError@0"() #2
  %call3 = tail call i32* @_errno() #2
  store i32 %call1, i32* %call3, align 4, !tbaa !1
  br label %return

return:                                           ; preds = %entry, %if.end
  %retval.0 = phi i32 [ -1, %if.end ], [ 0, %entry ]
  ret i32 %retval.0
}

declare x86_stdcallcc i32 @"\01_UnmapViewOfFile@4"(i8*) #1

; Function Attrs: nounwind
define i32 @mprotect(i8* %addr, i32 %len, i32 %prot) #0 {
entry:
  %oldProtect = alloca i32, align 4
  %cmp.i = icmp eq i32 %prot, 0
  br i1 %cmp.i, label %__map_mmap_prot_page.exit, label %if.end.i

if.end.i:                                         ; preds = %entry
  %and.i = and i32 %prot, 4
  %cmp1.i = icmp eq i32 %and.i, 0
  %and3.i = and i32 %prot, 2
  br i1 %cmp1.i, label %if.else.i, label %if.then2.i

if.then2.i:                                       ; preds = %if.end.i
  %0 = shl nuw nsw i32 %and3.i, 4
  %1 = add i32 %0, 32
  br label %__map_mmap_prot_page.exit

if.else.i:                                        ; preds = %if.end.i
  %2 = add i32 %and3.i, 2
  br label %__map_mmap_prot_page.exit

__map_mmap_prot_page.exit:                        ; preds = %entry, %if.then2.i, %if.else.i
  %retval.0.i = phi i32 [ 0, %entry ], [ %1, %if.then2.i ], [ %2, %if.else.i ]
  store i32 0, i32* %oldProtect, align 4, !tbaa !5
  %call1 = call x86_stdcallcc i32 @"\01_VirtualProtect@16"(i8* %addr, i32 %len, i32 %retval.0.i, i32* %oldProtect) #2
  %tobool = icmp eq i32 %call1, 0
  br i1 %tobool, label %if.end, label %return

if.end:                                           ; preds = %__map_mmap_prot_page.exit
  %call2 = call x86_stdcallcc i32 @"\01_GetLastError@0"() #2
  %call4 = call i32* @_errno() #2
  store i32 %call2, i32* %call4, align 4, !tbaa !1
  br label %return

return:                                           ; preds = %__map_mmap_prot_page.exit, %if.end
  %retval.0 = phi i32 [ -1, %if.end ], [ 0, %__map_mmap_prot_page.exit ]
  ret i32 %retval.0
}

declare x86_stdcallcc i32 @"\01_VirtualProtect@16"(i8*, i32, i32, i32*) #1

; Function Attrs: nounwind
define i32 @msync(i8* %addr, i32 %len, i32 %flags) #0 {
entry:
  %call = tail call x86_stdcallcc i32 @"\01_FlushViewOfFile@8"(i8* %addr, i32 %len) #2
  %tobool = icmp eq i32 %call, 0
  br i1 %tobool, label %if.end, label %return

if.end:                                           ; preds = %entry
  %call1 = tail call x86_stdcallcc i32 @"\01_GetLastError@0"() #2
  %call3 = tail call i32* @_errno() #2
  store i32 %call1, i32* %call3, align 4, !tbaa !1
  br label %return

return:                                           ; preds = %entry, %if.end
  %retval.0 = phi i32 [ -1, %if.end ], [ 0, %entry ]
  ret i32 %retval.0
}

declare x86_stdcallcc i32 @"\01_FlushViewOfFile@8"(i8*, i32) #1

; Function Attrs: nounwind
define i32 @mlock(i8* %addr, i32 %len) #0 {
entry:
  %call = tail call x86_stdcallcc i32 @"\01_VirtualLock@8"(i8* %addr, i32 %len) #2
  %tobool = icmp eq i32 %call, 0
  br i1 %tobool, label %if.end, label %return

if.end:                                           ; preds = %entry
  %call1 = tail call x86_stdcallcc i32 @"\01_GetLastError@0"() #2
  %call3 = tail call i32* @_errno() #2
  store i32 %call1, i32* %call3, align 4, !tbaa !1
  br label %return

return:                                           ; preds = %entry, %if.end
  %retval.0 = phi i32 [ -1, %if.end ], [ 0, %entry ]
  ret i32 %retval.0
}

declare x86_stdcallcc i32 @"\01_VirtualLock@8"(i8*, i32) #1

; Function Attrs: nounwind
define i32 @munlock(i8* %addr, i32 %len) #0 {
entry:
  %call = tail call x86_stdcallcc i32 @"\01_VirtualUnlock@8"(i8* %addr, i32 %len) #2
  %tobool = icmp eq i32 %call, 0
  br i1 %tobool, label %if.end, label %return

if.end:                                           ; preds = %entry
  %call1 = tail call x86_stdcallcc i32 @"\01_GetLastError@0"() #2
  %call3 = tail call i32* @_errno() #2
  store i32 %call1, i32* %call3, align 4, !tbaa !1
  br label %return

return:                                           ; preds = %entry, %if.end
  %retval.0 = phi i32 [ -1, %if.end ], [ 0, %entry ]
  ret i32 %retval.0
}

declare x86_stdcallcc i32 @"\01_VirtualUnlock@8"(i8*, i32) #1

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (198054)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
!5 = metadata !{metadata !6, metadata !6, i64 0}
!6 = metadata !{metadata !"long", metadata !3, i64 0}
