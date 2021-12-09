@p = global i32 0
@.str = private unnamed_addr constant [18 x i8] c"isto ser um teste\00"
@string = global i8* null
@.str.1 = private unnamed_addr constant [8 x i8] c"p = %d\0A\00"
@.str.2 = private unnamed_addr constant [8 x i8] c"ola\0Aola\00"
@.str.3 = private unnamed_addr constant [3 x i8] c"3\0A\00"
@.str.4 = private unnamed_addr constant [3 x i8] c"%s\00"

; Function Attrs: noinline nounwind optnone uwtable
define void @teste() #0 {
  store i32 3, i32* @p
  store i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str, i64 0, i64 0), i8** @string
  %1 = load i32, i32* @p
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.1, i64 0, i64 0), i32 %1)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.2, i64 0, i64 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.2, i64 0, i64 0))
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.2, i64 0, i64 0))
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.2, i64 0, i64 0))
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.2, i64 0, i64 0))
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i64 0, i64 0))
  %9 = load i8*, i8** @string
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.4, i64 0, i64 0), i8* %9)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %1 = alloca i32
  store i32 0, i32* %1
  ret i32 0
}

attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }


