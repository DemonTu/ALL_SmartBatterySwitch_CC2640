# Find all subdirectories
SUBDIRS = $(wildcard */)

# All the directories; therefore we always need to rerun this target
.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

all clean : $(SUBDIRS)

default: all

# Call sub-make recursively to allow parallel build
$(SUBDIRS)::
	@ $(MAKE) -C $@ $(MAKECMDGOALS)
