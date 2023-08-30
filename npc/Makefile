TOPNAME = top
NXDC_FILES = constr/npc_constr.nxdc
INC_PATH ?= 

# verilator
VERILATOR = verilator

# Configure the verilator flags
VERILATOR_CFLAGS += -MMD --build -cc  \
				-O3 --x-assign fast --x-initial fast --noassert

# Warn abount lint issues; may not want this on less solid designs
#VERILATOR_FLAGS += -Wall
# Make waveforms
#VERILATOR_FLAGS += --trace
# Set the build flag
#VERILATOR_FLAGS += --build


# 这里是一些和Makefile相关的变量
BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
default: $(BIN)

$(shell mkdir -p $(BUILD_DIR))
# constraint file，这里自动借助python脚本，生成相应的约束文件
SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bind.cpp)
$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# project source，这里就是统计一下资源
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")
CSRCS += $(SRC_AUTO_BIND)


# rules for NVBoard
include $(NVBOARD_HOME)/scripts/nvboard.mk

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""
LDFLAGS += -lSDL2 -lSDL2_image


# 这里是用verilator来编译.v电路文件，并生成仿真后的程序。
$(BIN): $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

sim: $(BIN)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "----------VERILATOR RUNNING----------"
	@echo "VERILATING AND BUILDING STEP:"
	@$^
	@echo "----------GENERATING LOGS------------"
	@rm -rf logs
	@mkdir -p logs
	@echo "----------DONE--------------------"
	@echo "To see waveforms, open vlt_dump.vcd in a waveform viewer"
include ../Makefile
