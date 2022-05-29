FROM base:latest

RUN mkdir /game-engine
WORKDIR /game-engine
COPY . /game-engine

RUN g++ -O3 -Wall -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) DeepAxie.cpp -o DeepAxieLinux.so
RUN pip install numpy
RUN pip install pandas
RUN pip install tensorflow
RUN python3 DQN.py