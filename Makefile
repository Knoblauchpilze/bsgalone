
# Keep in sync with the CI workflow
LIBPQXX_VERSION=7.10.1
CPP_VERSION=20
DOCKER_IMAGE_TAG=${CPP_VERSION}-${LIBPQXX_VERSION}

NB_PROCS=8

debug:
	mkdir -p cmake-build/Debug && \
	cd cmake-build/Debug && \
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_C_COMPILER_LAUNCHER=ccache \
		-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
		-S ../.. \
		-B . \
	&& \
	make -j ${NB_PROCS}

debugWithTests:
	mkdir -p cmake-build/Debug && \
	cd cmake-build/Debug && \
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DENABLE_TESTS=ON \
		-DCMAKE_C_COMPILER_LAUNCHER=ccache \
		-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
		-S ../.. \
		-B . \
	&& \
	make -j ${NB_PROCS}

release:
	mkdir -p cmake-build/Release && \
	cd cmake-build/Release && \
	cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_C_COMPILER_LAUNCHER=ccache \
		-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
		-S ../.. \
		-B . \
	&& \
	make -j ${NB_PROCS}

clean:
	rm -rf cmake-build

cleanSandbox:
	rm -rf sandbox

copyData:
	mkdir -p sandbox/
	rsync -avH assets sandbox/
	rsync -avH scripts/ sandbox/

copyDebug: debug copyData
	rsync -avH cmake-build/Debug/lib cmake-build/Debug/bin sandbox/

copyRelease: release copyData
	rsync -avH cmake-build/Release/lib cmake-build/Release/bin sandbox/

runclient: copyRelease
	cd sandbox && ./run.sh bsgalone_client 2323

drunclient: copyDebug
	cd sandbox && ./debug.sh bsgalone_client 2323

runserver: copyRelease
	cd sandbox && ./run.sh bsgalone_server 2323

drunserver: copyDebug
	cd sandbox && ./debug.sh bsgalone_server 2323

PHONY: .tests
tests: debugWithTests copyDebug

# https://stackoverflow.com/questions/2826029/passing-additional-variables-from-command-line-to-make
# Use like this:
# make rununittests test_filters="Unit_Bsgo_Serialization_EntityAddedMessage*"
rununittests: tests
	cd sandbox && ./tests.sh unitTests $(test_filters)

runintegrationtests: tests
	cd sandbox && ./tests.sh integrationTests $(test_filters)

ci-cpp-build-image:
	docker build \
		--tag totocorpsoftwareinc/ci-cpp-build-image:${DOCKER_IMAGE_TAG} \
		-f build/ci-cpp-build-image/Dockerfile \
		build/ci-cpp-build-image