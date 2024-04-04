exec_file := simple.cc
build_dir := build
project_name := SimpleEditor
export MY_BUILD_DIR = $(build_dir)
export MY_EXEC_FILE = $(exec_file)
export MY_PROJECT_NAME := $(project_name)
export MY_EXECUTABLES := Editor.cc utility.cc


default:
ifeq ($(DEBUG), true)
	@cd $(build_dir) && rm -rf debug.txt && make
	@echo -----------------------------
	@cd $(build_dir) && lldb $(basename $(project_name))
else
	@cd $(build_dir) && rm -rf debug.txt && make
	@echo -----------------------------
	@cd $(build_dir) && ./$(basename $(project_name))
endif

create:
	@mkdir -p $(build_dir)
ifeq ($(DEBUG), true)
	@echo "DEBUG is ON"
	@cd $(build_dir) && cmake .. -DCMAKE_BUILD_TYPE=Debug
else
	@cd $(build_dir) && cmake ..
endif

clean:
	rm -rf $(build_dir)

exp:
ifeq ($(DEBUG), true)
	@echo true
else
	@echo false
endif
