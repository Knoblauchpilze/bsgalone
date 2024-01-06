debug:
	mkdir -p build/Debug && cd build/Debug && cmake -DCMAKE_BUILD_TYPE=Debug ../.. && make -j 8

release:
	mkdir -p build/Release && cd build/Release && cmake -DCMAKE_BUILD_TYPE=Release ../.. && make -j 8

clean:
	rm -rf build

cleanSandbox:
	rm -rf sandbox

copyRelease:
	rsync -avH build/Release/lib build/Release/bin sandbox/

copyDebug:
	rsync -avH build/Debug/lib build/Debug/bin sandbox/

copy:
	mkdir -p sandbox/
	rsync -avH data sandbox/
	mv sandbox/data/*.sh sandbox/

sandbox: release copy copyRelease

sandboxDebug: debug copy copyDebug

# Run client
run: sandbox
	cd sandbox && ./run.sh bsgalone_client

# Debug client
drun: sandboxDebug
	cd sandbox && ./debug.sh bsgalone_client

# Run server
runserver: sandbox
	cd sandbox && ./run.sh bsgalone_server

# Debug server
drunserver: sandboxDebug

v: sandboxDebug
	cd sandbox && ./valgrind.sh bsgalone_server

profile: sandboxDebug
	cd sandbox && ./profile.sh local

tests: sandboxDebug
	cd sandbox && ./tests.sh local
# See here: https://stackoverflow.com/questions/3931741/why-does-make-think-the-target-is-up-to-date
PHONY: .tests

dtests: sandboxDebug
	cd sandbox && ./debug-tests.sh local
