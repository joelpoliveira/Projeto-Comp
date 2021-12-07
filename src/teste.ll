@x = global i32 0

define i32 @teste(i32 %a, i32 %c) {
	%abc = alloca i32
	store i32 %c, i32* %abc
	ret i32 0
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

