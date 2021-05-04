; ModuleID = 'sample.ll'
source_filename = "sample.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.List = type { i64, %struct.NodeData*, %struct.NodeData* }
%struct.NodeData = type { i8*, %struct.NodeData*, %struct.NodeData* }

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [13 x i8] c"element: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [19 x i8] c"array element: %d\0A\00", align 1

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
  %ArrAy = call i8* @List_to_array(%struct.List* %2)
  %siZE = call i64 @List_size(%struct.List* %2)
  %iiIi = alloca i64
  br label %10

10:                                               ; preds = %13, %0
  %11 = load %struct.NodeData*, %struct.NodeData** %3, align 8
  %12 = icmp ne %struct.NodeData* %11, null
  %giraffe = load i64, i64* %iiIi
  %cmpResult = icmp ult i64 %giraffe, %siZE
  br i1 %cmpResult, label %13, label %20

13:                                               ; preds = %10
  %14 = load %struct.NodeData*, %struct.NodeData** %3, align 8
  %15 = call i8* @Node_get(%struct.NodeData* byval(%struct.NodeData) align 8 %14)
  %16 = getelementptr [13 x i8], [13 x i8]* @.str.1, i64 0, i64 0
  %17 = call i32 (i8*, ...) @printf(i8* %16, i8* %15)
  %18 = load %struct.NodeData*, %struct.NodeData** %3, align 8
  %19 = call %struct.NodeData* @Node_next(%struct.NodeData* byval(%struct.NodeData) align 8 %18)
  store %struct.NodeData* %19, %struct.NodeData** %3, align 8
  %loadI = load i64, i64* %iiIi
  %incI = add i64 %loadI, 1
  store i64 %incI, i64* %iiIi
  br label %10

20:                                               ; preds = %10
  %21 = call i8* @List_to_array(%struct.List* %2)
  %22 = bitcast i8* %21 to i8**
  store i8** %22, i8*** %4, align 8
  store i32 0, i32* %5, align 4
  br label %23

23:                                               ; preds = %36, %20
  %24 = load i32, i32* %5, align 4
  %25 = sext i32 %24 to i64
  %26 = call i64 @List_size(%struct.List* byval(%struct.List) align 8 %2)
  %27 = icmp slt i64 %25, %26
  br i1 %27, label %28, label %39

28:                                               ; preds = %23
  %29 = load i8**, i8*** %4, align 8
  %30 = load i32, i32* %5, align 4
  %31 = sext i32 %30 to i64
  %32 = getelementptr inbounds i8*, i8** %29, i64 %31
  %33 = load i8*, i8** %32, align 8
  %34 = getelementptr [19 x i8], [19 x i8]* @.str.2, i64 0, i64 0
  %35 = call i32 (i8*, ...) @printf(i8* %34, i8* %33)
  br label %36

36:                                               ; preds = %28
  %37 = load i32, i32* %5, align 4
  %38 = add nsw i32 %37, 1
  store i32 %38, i32* %5, align 4
  br label %23

39:                                               ; preds = %23
  %40 = load i8**, i8*** %4, align 8
  %41 = bitcast i8** %40 to i8*
  call void @free(i8* %41) #3
  ret i32 0
}

declare dso_local void @List_new(%struct.List* sret, ...) #1

declare dso_local void @List_push_back(%struct.List*, i8*) #1

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @List_size(%struct.List* byval(%struct.List) align 8) #1

declare dso_local %struct.NodeData* @List_front(%struct.List* byval(%struct.List) align 8) #1

declare dso_local i8* @Node_get(%struct.NodeData* byval(%struct.NodeData) align 8) #1

declare dso_local %struct.NodeData* @Node_next(%struct.NodeData* byval(%struct.NodeData) align 8) #1

declare dso_local i8* @List_to_array(%struct.List*) #1

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
