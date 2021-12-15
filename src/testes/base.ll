@x = global i32 0
@.str = private unnamed_addr constant [5 x i8] c"123\0A\00"
@.str.1 = private unnamed_addr constant [7 x i8] c"abc = \00"
@.str.2 = private unnamed_addr constant [3 x i8] c"%d\00"
@.str.3 = private unnamed_addr constant [2 x i8] c"\0A\00"

define void @teste_void() {
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))

	ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
	%abc = alloca i32
	%c = alloca i32
	store i32 3, i32* %c
	%1 = icmp eq i1 1,  1
	%2 = icmp eq i1 2,  2
	%3 = and i1 %1,  %2
	br i1 %3, label %then1, label %else1
then1:
	%4 = icmp eq i1 3,  3
	br i1 %4, label %then2, label %ifcont2
then2:
	%5 = load i32, i32* %c
	%6 = add i32 %5,  1
	store i32 %6, i32* %abc
	br label %ifcont2
ifcont2:
	store i32 1, i32* %abc
	br label %ifcont1
else1:
	store i32 3, i32* %abc
	br label %ifcont1
ifcont1:
	%7 = icmp eq i1 1,  1
	br i1 %7, label %then4, label %ifcont4
then4:
	br label %ifcont4
ifcont4:
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([7 x i8], [7 x i8]*  @.str.1, i32 0, i32 0))
	%9 = load i32, i32* %abc
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([3 x i8], [3 x i8]*  @.str.2, i32 0, i32 0), i32 %9)
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([2 x i8], [2 x i8]*  @.str.3, i32 0, i32 0))

	ret i32 0
}
