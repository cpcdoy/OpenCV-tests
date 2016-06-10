FROM ubuntu:latest
RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
RUN apt-get install -y libopencv-dev
RUN git clone https://github.com/cpcdoy/OpenCV-tests.git
RUN cd OpenCV-tests/test_1/
RUN cmake .
RUN make
RUN ./test ../assets/imageTextN.png
