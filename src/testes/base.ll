@x = global i32 0
@.str = private unnamed_addr constant [5 x i8] c"123\0A\00"
@.str.1 = private unnamed_addr constant [9 x i8] c"ola\0A123\0A\00"

define i32 @teste(i32 %a, i32 %c, i1 %d) {
	%abc = alloca i32
	%1 = icmp eq i1 1,  1
	%2 = icmp eq i1 2,  2
	%3 = and i1 %1,  %2
	br i1 %3, label %then1, label %else1
then1:
	%4 = icmp eq i1 3,  3
	%5 = and i1 %4,  %d
	br i1 %5, label %then2, label %ifcont2
then2:
	%6 = add i32 %a, %c
	%7 = add i32 %6,  1
	store i32 %7, i32* %abc
	br label %ifcont2
ifcont2:
	store i32 1, i32* %abc
	br label %ifcont1
else1:
	store i32 3, i32* %abc
	br label %ifcont1
ifcont1:
	%8 = icmp eq i1 1,  1
	br i1 %8, label %then4, label %ifcont4
then4:
	br label %ifcont4
ifcont4:
	%9 = load i32, i32* %abc
	ret i32 %9

}

define void @teste_void() {
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([5 x i8], [5 x i8]*  @.str, i32 0, i32 0))
	call i32(i8*, ...) @printf (i8* getelementptr inbounds([9 x i8], [9 x i8]*  @.str.1, i32 0, i32 0))
	
	ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
	call void @teste_void()

	ret i32 0
}

