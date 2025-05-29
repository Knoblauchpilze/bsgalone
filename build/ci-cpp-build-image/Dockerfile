
FROM ubuntu:24.04 AS dev-tools
# https://stackoverflow.com/questions/34773745/debconf-unable-to-initialize-frontend-dialog
ENV DEBIAN_FRONTEND="noninteractive"
# https://docs.docker.com/build/building/best-practices/#apt-get
# https://docs.datadoghq.com/security/code_security/static_analysis/static_analysis_rules/docker-best-practices/apt-pin-version/
# https://stackoverflow.com/questions/37970990/ssl-certificate-verification-fails-inside-docker-container-on-specific-server
RUN apt-get update && apt-get install -y --no-install-recommends \
  ca-certificates=20240203 \
  build-essential=12.10ubuntu1 \
  wget=1.21.4-1ubuntu4.1 \
  cmake=3.28.3-1build7 \
  pkg-config=1.8.1-2build1 \
  libpq-dev=16.9-0ubuntu0.24.04.1 \
  libx11-dev=2:1.8.7-1build1 \
  libgl-dev=1.7.0-1build1 \
  libasio-dev=1:1.28.1-0.2 \
  libeigen3-dev=3.4.0-4build0.1 \
  libpng-dev=1.6.43-5build1 \
  libgtest-dev=1.14.0-1 \
  rsync=3.2.7-1ubuntu1.2 \
  && rm -rf /var/lib/apt/lists/*

ARG CPP_VERSION="20"
ARG LIBPQXX_VERSION="7.10.1"
ENV CPP_VERSION=$CPP_VERSION
ENV LIBPQXX_VERSION=$LIBPQXX_VERSION
FROM dev-tools AS ci-build-base
WORKDIR /tmp
RUN wget https://github.com/jtv/libpqxx/archive/refs/tags/${LIBPQXX_VERSION}.tar.gz -O libpqxx.tar.gz
RUN tar -xzf libpqxx.tar.gz
WORKDIR libpqxx-${LIBPQXX_VERSION}
RUN cmake \
      -DCMAKE_CXX_STANDARD=${CPP_VERSION} \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=1 \
      .
RUN cmake --build . -j 8
RUN cmake --install .

FROM dev-tools AS ci-cpp-build-image
ARG GIT_COMMIT_HASH="undefined"
ENV GIT_COMMIT_HASH=$GIT_COMMIT_HASH
WORKDIR /
COPY --from=ci-build-base /usr/local/lib /usr/local/lib/
COPY --from=ci-build-base /usr/local/include/pqxx /usr/local/include/pqxx