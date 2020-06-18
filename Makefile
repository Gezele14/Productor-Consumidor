all: libs code

code:
		(cd src && make)

libs:
		(cd lib && make)