FROM ubuntu:latest

MAINTAINER cpcdoy

RUN apt-get update && apt-get install -y \
 build-essential \
 cmake \
 git \
 libgtk2.0-dev \
 pkg-config \
 libavcodec-dev \
 libavformat-dev \
 libswscale-dev \
 libopencv-dev
RUN git clone https://github.com/cpcdoy/OpenCV-tests.git
WORKDIR OpenCV-tests/test_1
RUN cmake .
RUN make
RUN ./test ../assets/imageTextN.png
RUN ls
#RUN cp output.jpg /artifacts/output.jpg
EXPOSE 4242
