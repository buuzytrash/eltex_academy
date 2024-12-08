obj-m += test1.o

all:
	make -C /lib/modules/6.8.0-49-generic/build M=$(PWD) modules

clean:
	make -C /lib/modules/6.8.0-49-generic/build M=$(PWD) clean
