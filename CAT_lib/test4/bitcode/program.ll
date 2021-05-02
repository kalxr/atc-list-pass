; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define dso_local i32 @CAT_execution1() local_unnamed_addr #0 {
  %1 = tail call i8* @CAT_new(i64 5) #3
  %2 = tail call i8* @CAT_new(i64 8) #3
  %3 = tail call i8* @CAT_new(i64 0) #3
  tail call void @CAT_add(i8* %3, i8* %1, i8* %2) #3
  tail call void @CAT_sub(i8* %3, i8* %3, i8* %1) #3
  tail call void @CAT_set(i8* %1, i64 3) #3
  %4 = tail call i64 @CAT_get(i8* %3) #3
  %5 = trunc i64 %4 to i32
  ret i32 %5
}

declare dso_local i8* @CAT_new(i64) local_unnamed_addr #1

declare dso_local void @CAT_add(i8*, i8*, i8*) local_unnamed_addr #1

declare dso_local void @CAT_sub(i8*, i8*, i8*) local_unnamed_addr #1

declare dso_local void @CAT_set(i8*, i64) local_unnamed_addr #1

declare dso_local i64 @CAT_get(i8*) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local i32 @CAT_execution2() local_unnamed_addr #0 {
  %1 = tail call i8* @CAT_new(i64 5) #3
  %2 = tail call i64 @CAT_get(i8* %1) #3
  %3 = trunc i64 %2 to i32
  ret i32 %3
}

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %3 = tail call i32 @CAT_execution1()
  %4 = tail call i32 @CAT_execution2()
  %5 = tail call i64 @CAT_invocations() #3
  %6 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str, i64 0, i64 0), i64 %5)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

declare dso_local i64 @CAT_invocations() local_unnamed_addr #1

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 9.0.0 (git@github.com:scampanoni/LLVM_installer.git 713d2f6594d9a0b77e7f9a120aaa7c917715a640)"}
