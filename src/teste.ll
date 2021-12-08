@p = global i32 0

; Function Attrs: noinline nounwind optnone uwtable
define i32 @factorial(i32 %0) #0 {
  %2 = alloca i32
  %3 = alloca i32
  store i32 %0, i32* %2
  %4 = load i32, i32* %2
  %5 = add i32 %4, 3
  %6 = add i32 %5, 1
  store i32 %6, i32* %3
  %7 = load i32, i32* %3
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %1 = alloca i32
  store i32 0, i32* %1
  %2 = call i32 @factorial(i32 2)
  ret i32 0
}



