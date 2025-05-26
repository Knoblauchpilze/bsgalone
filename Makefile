
GIT_COMMIT_HASH=$(shell git rev-parse --short HEAD)

debug:
	mkdir -p cmake-build/Debug && \
	cd cmake-build/Debug && \
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		../.. \
	&& \
	make -j 8

debugWithTests:
	mkdir -p cmake-build/Debug && \
	cd cmake-build/Debug && \
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DENABLE_TESTS=ON \
		../.. \
	&& \
	make -j 8

release:
	mkdir -p cmake-build/Release && \
	cd cmake-build/Release && \
	cmake \
		-DCMAKE_BUILD_TYPE=Release \
		../.. \
	&& \
	make -j 8

clean:
	rm -rf cmake-build

cleanSandbox:
	rm -rf sandbox

copyData:
	mkdir -p sandbox/
	rsync -avH data sandbox/
	rsync -avH scripts/ sandbox/

copyDebug: debug copyData
	rsync -avH cmake-build/Debug/lib cmake-build/Debug/bin sandbox/

copyRelease: release copyData
	rsync -avH cmake-build/Release/lib cmake-build/Release/bin sandbox/

runclient: copyRelease
	cd sandbox && ./run.sh bsgalone_client

drunclient: copyDebug
	cd sandbox && ./debug.sh bsgalone_client

runserver: copyRelease
	cd sandbox && ./run.sh bsgalone_server 2323

drunserver: copyDebug
	cd sandbox && ./debug.sh bsgalone_server 2323

PHONY: .tests
tests: debugWithTests copyDebug

runtests: tests
	cd sandbox && ./tests.sh
