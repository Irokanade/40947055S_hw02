all: bmp_alpha.o bmpDistortion.o footballQuery.o lyrics.o
	gcc lyrics.o hw0301.c -o hw0301
	gcc footballQuery.o hw0302.c -o hw0302
	gcc bmpDistortion.o hw0303.c -o hw0303
	gcc bmp_alpha.o hw0304.c -o hw0304
	gcc hw0306.c -o hw0306
	
bmp_alpha.o:
	gcc -c bmp_alpha.c

bmpDistortion.o:
	gcc -c bmpDistortion.c -lm

footballQuery.o:
	gcc -c footballQuery.c

lyrics.o:
	gcc -c lyrics.c

clean:
	rm *.o
