SOURCES:=$(wildcard *.cc)
# HEADERS:=$(wildcard *.h)
TARGETLIST:=$(patsubst %.cc,%,$(SOURCES))
all:${TARGETLIST}
.PHONY:clean
clean:
	$(foreach TARGET,${TARGETLIST},rm -f $(TARGET);)
