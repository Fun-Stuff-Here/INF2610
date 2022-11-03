all:
	gcc -O0 -std=gnu11 -Werror -Wall -Wno-unused-result -Wno-unused-variable -o GuerreDesLettres  GuerreDesLettres.c -lpthread

handin:
	rm -rf handin
	mkdir handin
	cp GuerreDesLettres.c handin
	tar -czvf handin.tar.gz handin
	rm -rf handin

clean:
	rm -f GuerreDesLettres
	rm -f handin.tar.gz
