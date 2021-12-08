@p = global i32 0

; Function Attrs: noinline nounwind optnone uwtable
define i32 @factorial(i32 %0, i32 %1) #0 {
  %3 = alloca i32
  %4 = alloca i32
  %5 = alloca i32
  store i32 %0, i32* %3
  store i32 %1, i32* %4
  %6 = load i32, i32* %3
  %7 = load i32, i32* %4
  %8 = add nsw i32 %6, %7
  %9 = add nsw i32 %8, 3
  store i32 %9, i32* %5
  %10 = load i32, i32* %5
  ret i32 %10
}

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %1 = alloca i32
  %2 = alloca i8
  %3 = alloca i8
  %4 = alloca i8
  %5 = alloca i8
  store i32 0, i32* %1
  store i8 0, i8* %2
  store i8 1, i8* %3
  %6 = load i8, i8* %2
  %7 = trunc i8 %6 to i1
  br i1 %7, label %8, label %11

8:                                                ; preds = %0
  %9 = load i8, i8* %3
  %10 = trunc i8 %9 to i1
  br label %11

11:                                               ; preds = %8, %0
  %12 = phi i1 [ false, %0 ], [ %10, %8 ]
  %13 = zext i1 %12 to i8
  store i8 %13, i8* %4
  %14 = load i8, i8* %2
  %15 = trunc i8 %14 to i1
  br i1 %15, label %19, label %16

16:                                               ; preds = %11
  %17 = load i8, i8* %3
  %18 = trunc i8 %17 to i1
  br label %19

19:                                               ; preds = %16, %11
  %20 = phi i1 [ true, %11 ], [ %18, %16 ]
  %21 = zext i1 %20 to i8
  store i8 %21, i8* %5
  %22 = call i32 @factorial(i32 5, i32 6)
  ret i32 0
}



