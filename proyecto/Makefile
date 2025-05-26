# Makefile principal en project-root

.PHONY: all os p1 p2 clean

all: os p1 p2
	@echo "Build completo de todo el sistema"

os:
	$(MAKE) -C os

p1:
	$(MAKE) -C p1

p2:
	$(MAKE) -C p2

clean:
	$(MAKE) -C os clean
	$(MAKE) -C p1 clean
	$(MAKE) -C p2 clean
	@echo " Limpieza completa"
