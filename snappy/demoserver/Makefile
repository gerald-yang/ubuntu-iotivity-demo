# -*- Mode: Makefile; indent-tabs-mode:t; tab-width: 4 -*-

all:

install:
	mkdir -p $(DESTDIR)/bin
	cp -a start-demo.sh $(DESTDIR)/bin/start-demo
	cp -a demoserver $(DESTDIR)/bin
	cp -ra lib $(DESTDIR)/
	chmod a+x $(DESTDIR)/bin/start-demo
	chmod a+x $(DESTDIR)/bin/demoserver
