.class public sample
.super java/lang/Object

; inta,b,c

.field private static a I
.field private static b I
.field private static c I

; decd,e

.field private static d F
.field private static e F

; boolf,g

.field private static f Z
.field private static g Z

; intans

.field private static ans I

; intnum1

.field private static num1 I

; intnum2

.field private static num2 I

.field private static _runTimer LRunTimer;
.field private static _standardIn LPascalTextIn;
.method public <init>()V

	aload_0
	invokenonvirtual java/lang/Object/<init>()V
	return

.limit locals 1
.limit stack 1
.end method

.method public static main([Ljava/lang/String;)V

	new RunTimer
	dup
	invokenonvirtual RunTimer/<init>()V
	putstatic	sample/_runTimer LRunTimer;
	new PascalTextIn
	dup
	invokenonvirtual PascalTextIn/<init>()V
	putstatic	sample/_standardIn LPascalTextIn;

goto End_sum
L_sum: 
	astore_1

; ans=0

	ldc	0
	putstatic	sample/ans I

; for(2x){ans=num1+num2;}

	.var 0 is i I from Label_0 to Label_1
	bipush	2
	istore_0
Label_0:
	iload_0
	bipush 0
	if_icmple Label_1

; ans=num1+num2

	getstatic	sample/num1 I
	getstatic	sample/num2 I
	iadd
	putstatic	sample/ans I
	iinc 0 -1
	goto Label_0

Label_1:
	getstatic	sample/ans I

	ret 1
End_sum:


; print("\nStart Sample.\n\n")

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\nStart Sample.\n\n"
	ldc	0
	anewarray	java/lang/Object
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

; a=1

	ldc	1
	putstatic	sample/a I

; d=2.3

	ldc	2.3
	putstatic	sample/d F

; e=2.0

	ldc	2.0
	putstatic	sample/e F

; e=d*e

	getstatic	sample/d F
	getstatic	sample/e F
	fmul
	putstatic	sample/e F

; f=false

	ldc	"false"
	invokestatic	java/lang/Boolean.parseBoolean(Ljava/lang/String;)Z
	putstatic	sample/f Z

; g=true

	ldc	"true"
	invokestatic	java/lang/Boolean.parseBoolean(Ljava/lang/String;)Z
	putstatic	sample/g Z

; a=1+2

	ldc	1
	ldc	2
	iadd
	putstatic	sample/a I

; b=a+1

	getstatic	sample/a I
	ldc	1
	iadd
	putstatic	sample/b I

; c=b+a

	getstatic	sample/b I
	getstatic	sample/a I
	iadd
	putstatic	sample/c I

; ifa==b,{print("a == b\n");}else{print("a =/= b\n");b=1+b*3;}

	getstatic	sample/a I
	getstatic	sample/b I
	if_icmpeq Label_2
	goto Label_3

Label_2:

; print("a == b\n")

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"a == b\n"
	ldc	0
	anewarray	java/lang/Object
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
	goto Label_4

Label_3:

; print("a =/= b\n")

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"a =/= b\n"
	ldc	0
	anewarray	java/lang/Object
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

; b=1+b*3

	ldc	1
	getstatic	sample/b I
	ldc	3
	imul
	iadd
	putstatic	sample/b I
Label_4:

; print("\nhello")

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\nhello"
	ldc	0
	anewarray	java/lang/Object
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

; print("\na = %d , b = %d, c = %d\n",a,b,c)

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\na = %d , b = %d, c = %d\n"
	ldc	3
	anewarray	java/lang/Object
	dup
	ldc	0
	getstatic	sample/a I
	invokestatic	java/lang/Integer.valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	ldc	1
	getstatic	sample/b I
	invokestatic	java/lang/Integer.valueOf(I)Ljava/lang/Integer;
	aastore
	dup
	ldc	2
	getstatic	sample/c I
	invokestatic	java/lang/Integer.valueOf(I)Ljava/lang/Integer;
	aastore
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

; print("d = %f, e = %f\n",d,e)

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"d = %f, e = %f\n"
	ldc	2
	anewarray	java/lang/Object
	dup
	ldc	0
	getstatic	sample/d F
	invokestatic	java/lang/Float.valueOf(F)Ljava/lang/Float;
	aastore
	dup
	ldc	1
	getstatic	sample/e F
	invokestatic	java/lang/Float.valueOf(F)Ljava/lang/Float;
	aastore
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

; print("f = %b, g = %b\n",f,g)

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"f = %b, g = %b\n"
	ldc	2
	anewarray	java/lang/Object
	dup
	ldc	0
	getstatic	sample/f Z
	invokestatic	java/lang/Boolean.valueOf(Z)Ljava/lang/Boolean;
	aastore
	dup
	ldc	1
	getstatic	sample/g Z
	invokestatic	java/lang/Boolean.valueOf(Z)Ljava/lang/Boolean;
	aastore
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

; if"milk tea"is not"coffee",{print("\nmilk tea is not coffee\n\n");}

	ldc	10
	ldc	8
	if_icmpne Label_5
	goto Label_6

Label_5:

; print("\nmilk tea is not coffee\n\n")

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\nmilk tea is not coffee\n\n"
	ldc	0
	anewarray	java/lang/Object
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
Label_6:

; for(3x){print("peanut butter oreos\n");}

	.var 0 is i I from Label_7 to Label_8
	bipush	3
	istore_0
Label_7:
	iload_0
	bipush 0
	if_icmple Label_8

; print("peanut butter oreos\n")

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"peanut butter oreos\n"
	ldc	0
	anewarray	java/lang/Object
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop
	iinc 0 -1
	goto Label_7

Label_8:

; print("\nsum(a,b) = %d\n",sum(3,4))

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\nsum(a,b) = %d\n"
	ldc	1
	anewarray	java/lang/Object
	dup
	ldc	0
	ldc	3
	putstatic	sample/num1 I
	ldc	4
	putstatic	sample/num2 I
	jsr L_sum
	invokestatic	java/lang/Integer.valueOf(I)Ljava/lang/Integer;
	aastore
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

; print("\nEnd Sample.")

	getstatic	java/lang/System/out Ljava/io/PrintStream;
	ldc	"\nEnd Sample."
	ldc	0
	anewarray	java/lang/Object
	invokevirtual java/io/PrintStream.printf(Ljava/lang/String;[Ljava/lang/Object;)Ljava/io/PrintStream;
	pop

	getstatic     sample/_runTimer LRunTimer;
	invokevirtual RunTimer.printElapsedTime()V

	return

.limit locals 16
.limit stack 16
.end method
