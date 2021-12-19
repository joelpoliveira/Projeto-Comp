@argument = global i32 0
@.str = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.1 = private unnamed_addr constant [6 x i8] c"%.08f\00"
@.str.2 = private unnamed_addr constant [6 x i8] c"true\0A\00"
@.str.3 = private unnamed_addr constant [7 x i8] c"false\0A\00"
@.str.4 = private unnamed_addr constant [2 x i8] c"\0A\00"

define i32 @main(i32 %argc, i8** %argv) {
	%1 = getelementptr i8*, i8** %argv, i32 1
	%2 = load i8*, i8** %1
	%3 = call i32 (i8*, ...) bitcast (i32 (...)* @atoi to i32 (i8*, ...)*)(i8* %2)
	store i32 %3, i32* @argument
	call void @print_arg()
	ret i32 0
}

declare i32 @atoi(...)

define void @print_arg() {
	%1 = load i32, i32* @argument
	%2 = call i32(i8*, ...) @printf (i8* getelementptr inbounds([3 x i8], [3 x i8]*  @.str, i32 0, i32 0), i32 %1)
	%3 = call i32(i8*, ...) @printf (i8* getelementptr inbounds([2 x i8], [2 x i8]*  @.str.4, i32 0, i32 0))
	ret void
}

declare i32 @printf(i8*, ...)

