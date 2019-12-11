SHELL := /bin/bash

ROOT := "$(shell pwd)"
BUILD := $(ROOT)/build
SRC := $(ROOT)/src
DATA := $(ROOT)/data
SCRIPTS := $(ROOT)/scripts
TMP := $(ROOT)/tmp
BUILDER := $(SRC)/parallel_builder
PLOTS := $(ROOT)/plots

MODULES := intel CMake Python/3.6.1

BUILD_TYPE := Release

PARAMS :=
INPUT := bun_zipper_res4.pts
OUTPUT :=

PACK := xharmi00


.PHONY: build
build:
	ml $(MODULES) && \
		mkdir -p $(BUILD) && \
		cd $(BUILD) && \
		cmake $(SRC) -DCMAKE_BUILD_TYPE='$(BUILD_TYPE)' && \
		make -j


.PHONY: run
run:
	ml $(MODULES) && \
		$(BUILD)/PMC $(DATA)/$(INPUT) $(OUTPUT) $(PARAMS)


.PHONY: generate-plots
generate-plots:
	ml $(MODULES) && \
		cd $(BUILD) && \
		$(SCRIPTS)/generate_data_and_plots.sh


.PHONY: qsub
qsub:
	qsub -A DD-19-32 -q qexp -l select=1:ncpus=16,walltime=1:00:00 -I


.PHONY: pack
pack: $(PACK).zip

$(PACK).zip:
	mkdir -p $(TMP) && \
		cd $(TMP) && \
		cp -R $(BUILDER) $(ROOT)/PMC-xharmi00.txt $(PLOTS)/* . && \
		zip -r $@ ./* && \
		cp $@ $(ROOT) && \
		rm -rf $(TMP)


.PHONY: clean
clean:
	rm -rf $(BUILD) $(TMP) $(PACK).zip
