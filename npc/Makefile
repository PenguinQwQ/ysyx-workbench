all:
	@echo "Write this Makefile by your self."


# Get the proper verilator location:

ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
VERILATOR_COVERAGE = verilator_coverage
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
VERILATOR_COVERAGE = $(VERILATOR_ROOT)/bin/verilator_coverage
endif

# Configure the verilator flags
VERILATOR_FLAGS += -cc --exe
VERILATOR_FLAGS += -x-assign fast
# Warn abount lint issues; may not want this on less solid designs
VERILATOR_FLAGS += -Wall
# Make waveforms
VERILATOR_FLAGS += --trace
# Set the build flag
VERILATOR_FLAGS += --build

# Input files for Verilator
VERILATOR_INPUT = ./vsrc/*.v ./csrc/*.cpp



sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "----------VERILATOR RUNNING----------"
	@echo "VERILATING AND BUILD STEP:"
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILATOR_INPUT)	
	@echo "----------GENERATING LOGS------------"
	@rm -rf logs
	@mkdir -p logs
	@echo "----------DONE--------------------"
	@echo "To see waveforms, open vlt_dump.vcd in a waveform viewer"
include ../Makefile
