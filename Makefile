.PHONY: default
default: rebuild compile_commands.json

.PHONY: rebuild 
rebuild: reconfigure compile_commands.json
	cmake --build Debug

.PHONY: reconfigure
reconfigure:
	cmake -S. -B Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

.PHONY: clean
clean:
	cmake --build Debug --target clean

.PHONY: run
run: rebuild
	Debug/window

compile_commands.json: Debug/compile_commands.json
	cp Debug/compile_commands.json compile_commands.json
