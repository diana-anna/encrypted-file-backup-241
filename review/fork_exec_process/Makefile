EXES = zombie orphan exec_leak sigchld_demo
all: $(EXES)

zombie: zombies_and_orphans.c
	clang $^ -DCHILD_DIES -o $@

orphan: zombies_and_orphans.c
	clang $^ -o $@

%: %.c
	clang $^ -o $@

clean:
	rm -rf $(EXES)
