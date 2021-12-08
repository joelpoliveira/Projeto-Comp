@x = global i32 0

define i32 @teste(i32 %a, i32 %c) {
	%abc = alloca i32 ;var abc int
    %1 = add i32 %a, %c ; %1 = a + c 
    %2 = add i32 %1, 1 ; %2 = %1 + 1
	store i32 %2, i32* %abc ; abc = %2
    %3 = load i32, i32* %abc ;%3 = abc
	ret i32 %3 ; 
}

define i32 @teste_float() {
	ret i32 1
}

define void @teste_void() {
	ret void
}

define i32 @main() {
	ret i32 0
}
