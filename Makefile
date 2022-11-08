CL_PATH = src/pn2012/
SRV_PATH = src/server2012/
SRC_PATH = src/
DOC_PATH = ./doc/
NAME = xknapo02

all:
	qmake -o $(CL_PATH)Makefile $(CL_PATH)pn2012.pro
	make -C $(CL_PATH)
	qmake -o $(SRV_PATH)Makefile $(SRV_PATH)server2012.pro
	make -C $(SRV_PATH)

run:
	cd $(CL_PATH); ./pn2012

runserver:
	cd $(SRV_PATH); ./server2012

pack:
	tar cvzf ../$(NAME).tar.gz *


doxygen:
	doxygen $(SRC_PATH)Doxyfile

clean:
	make -C $(SRV_PATH) clean
	make -C $(CL_PATH) clean
	rm -rf $(DOC_PATH)html

