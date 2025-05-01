FROM ubuntu:22.04

# Install dependencies for both test suites
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    g++ \
    cmake \
    git \
    wget \
    unzip \
    python3 \
    python3-pip \
    libbz2-dev \
    libdivsufsort-dev \
    libjsoncpp-dev \
    libssl-dev \
    libmpfr-dev \
    libgmp-dev

# Symlink jsoncpp headers where expected
RUN ln -s /usr/include/jsoncpp/json /usr/include/json

# Set working directory
WORKDIR /workspace

# ===== Clone and build SP800-90B =====
RUN git clone https://github.com/usnistgov/SP800-90B_EntropyAssessment.git
WORKDIR /workspace/SP800-90B_EntropyAssessment/cpp
RUN make CXX=g++ CXXFLAGS="-I/usr/include/jsoncpp -fopenmp"

# ===== Download and build SP800-22 STS =====
WORKDIR /workspace
RUN wget https://csrc.nist.gov/CSRC/media/Projects/Random-Bit-Generation/documents/sts-2_1_2.zip

# Unzip first, then change directory and build
RUN unzip -o /workspace/sts-2_1_2.zip -d /workspace/ && \
    cd /workspace/sts-2.1.2/sts-2.1.2/ && \
    make

# Go back to a general-purpose working dir
WORKDIR /workspace

# Entry point
CMD ["/bin/bash"]