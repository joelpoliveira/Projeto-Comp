@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@.str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@.str.2 = private unnamed_addr constant [6 x i8] c"true\0A\00"
@.str.3 = private unnamed_addr constant [7 x i8] c"false\0A\00"
@.str.4 = private unnamed_addr constant [3 x i8] c"10\00"

define i32 @factorial(i32 %n) {
	%1 = add i32 %n, 0
	%2 = icmp eq i32 %1,  0
	br i1 %2, label %then1, label %ifcont1
then1:
	ret i32 1
ifcont1:
	%3 = add i32 %n, 0
	%4 = add i32 %n, 0
	%5 = sub i32 %4,  1
	%6 = call i32 @factorial(i32 %5)
	%7 = mul i32 %3,  %6
	ret i32 %7
}

define void @main() {
	%argument = alloca i32
	%1 = load i8*, i8** @.str.4
	%2 = call i32 (i32) @atoi([3 x i8*] @.str.4)
	%3 = load i32, i32* %argument
	%4 = call i32 @factorial(i32 %3)
	%5 = call i32(i8*, ...) @printf (i8* getelementptr inbounds([4 x i8], [4 x i8]*  @.str, i32 0, i32 0), i32 %4)
	ret void
}

declare i32 @printf(i8*, ...)

declare i32 @atoi(i8*)

