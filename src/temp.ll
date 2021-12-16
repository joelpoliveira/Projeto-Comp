@.str = private unnamed_addr constant [28 x i8] c"Print123%%d%%f%%.08fTesting%%s\0A\00"
@.str.1 = private unnamed_addr constant [4 x i8] c"%%d\0A\00"
@.str.2 = private unnamed_addr constant [4 x i8] c"%%f\0A\00"
@.str.3 = private unnamed_addr constant [6 x i8] c"true\0A\00"
@.str.4 = private unnamed_addr constant [7 x i8] c"false\0A\00"

define i32 @teste(i32 %a, i32 %b) {
	ret i32 0
}

define void @main(i32 %argc, i8** %argv) {
	%1 = call i32(i8*, ...) @printf (i8* getelementptr inbounds([28 x i8], [28 x i8]*  @.str, i32 0, i32 0))
	ret void
}

declare i32 @printf(i8*, ...)

