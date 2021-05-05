; ModuleID = 'sample.ll'
source_filename = "sample.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.List = type { i64, %struct.NodeData*, %struct.NodeData* }
%struct.NodeData = type { i8*, %struct.NodeData*, %struct.NodeData* }

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"hi there!!\00", align 1
@.str.2 = private unnamed_addr constant [13 x i8] c"element: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [19 x i8] c"array element: %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.List, align 8
  %3 = alloca %struct.NodeData*, align 8
  %4 = alloca i8**, align 8
  %5 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void (%struct.List*, ...) @List_new(%struct.List* sret %2)
  call void @List_push_back(%struct.List* %2, i8* inttoptr (i64 8 to i8*))
  call void @List_push_back(%struct.List* %2, i8* inttoptr (i64 10 to i8*))
  call void @List_push_back(%struct.List* %2, i8* inttoptr (i64 9 to i8*))
  %6 = call i64 @List_size(%struct.List* byval(%struct.List) align 8 %2)
  %7 = getelementptr [4 x i8], [4 x i8]* @.str, i64 0, i64 0
  %8 = call i32 (i8*, ...) @printf(i8* %7, i64 %6)
  %9 = call %struct.NodeData* @List_front(%struct.List* byval(%struct.List) align 8 %2)
  store %struct.NodeData* %9, %struct.NodeData** %3, align 8
  %ArrAy = call i8** @List_to_array(%struct.List* %2)
  %siZE = call i64 @List_size(%struct.List* %2)
  %iiIi = alloca i64
  store i64 0, i64* %iiIi
  br label %10

10:                                               ; preds = %13, %0
  %11 = load %struct.NodeData*, %struct.NodeData** %3, align 8
  %12 = icmp ne %struct.NodeData* %11, null
  %giraffe = load i64, i64* %iiIi
  %cmpResult = icmp slt i64 %giraffe, %siZE
  %element = getelementptr inbounds i8*, i8** %ArrAy, i64 %giraffe
  %curr = load i8*, i8** %element
  br i1 %cmpResult, label %13, label %22

13:                                               ; preds = %10
  %14 = getelementptr [11 x i8], [11 x i8]* @.str.1, i64 0, i64 0
  %15 = call i32 (i8*, ...) @printf(i8* %14)
  %16 = load %struct.NodeData*, %struct.NodeData** %3, align 8
  %17 = call i8* @Node_get(%struct.NodeData* byval(%struct.NodeData) align 8 %16)
  %18 = getelementptr [13 x i8], [13 x i8]* @.str.2, i64 0, i64 0
  %19 = call i32 (i8*, ...) @printf(i8* %18, i8* %curr)
  %20 = load %struct.NodeData*, %struct.NodeData** %3, align 8
  %21 = call %struct.NodeData* @Node_next(%struct.NodeData* byval(%struct.NodeData) align 8 %20)
  store %struct.NodeData* %21, %struct.NodeData** %3, align 8
  %loadI = load i64, i64* %iiIi
  %incI = add i64 %loadI, 1
  store i64 %incI, i64* %iiIi
  br label %10

22:                                               ; preds = %10
  %23 = call i8** @List_to_array(%struct.List* %2)
  store i8** %23, i8*** %4, align 8
  store i32 0, i32* %5, align 4
  br label %24

24:                                               ; preds = %37, %22
  %25 = load i32, i32* %5, align 4
  %26 = sext i32 %25 to i64
  %27 = call i64 @List_size(%struct.List* byval(%struct.List) align 8 %2)
  %28 = icmp slt i64 %26, %27
  br i1 %28, label %29, label %40

29:                                               ; preds = %24
  %30 = load i8**, i8*** %4, align 8
  %31 = load i32, i32* %5, align 4
  %32 = sext i32 %31 to i64
  %33 = getelementptr inbounds i8*, i8** %30, i64 %32
  %34 = load i8*, i8** %33, align 8
  %35 = getelementptr [19 x i8], [19 x i8]* @.str.3, i64 0, i64 0
  %36 = call i32 (i8*, ...) @printf(i8* %35, i8* %34)
  br label %37

37:                                               ; preds = %29
  %38 = load i32, i32* %5, align 4
  %39 = add nsw i32 %38, 1
  store i32 %39, i32* %5, align 4
  br label %24

40:                                               ; preds = %24
  %41 = load i8**, i8*** %4, align 8
  %42 = bitcast i8** %41 to i8*
  call void @free(i8* %42) #3
  ret i32 0
}

declare dso_local void @List_new(%struct.List* sret, ...) #1

declare dso_local void @List_push_back(%struct.List*, i8*) #1

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @List_size(%struct.List* byval(%struct.List) align 8) #1

declare dso_local %struct.NodeData* @List_front(%struct.List* byval(%struct.List) align 8) #1

declare dso_local i8* @Node_get(%struct.NodeData* byval(%struct.NodeData) align 8) #1

declare dso_local %struct.NodeData* @Node_next(%struct.NodeData* byval(%struct.NodeData) align 8) #1

declare dso_local i8** @List_to_array(%struct.List*) #1

; Function Attrs: nounwind
declare dso_local void @free(i8*) #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 9.0.0 (git@github.com:scampanoni/LLVM_installer.git 713d2f6594d9a0b77e7f9a120aaa7c917715a640)"}
