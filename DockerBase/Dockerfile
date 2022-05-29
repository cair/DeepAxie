FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update; apt-get install -y curl python3-dev python3-pip \
    wget build-essential zlib1g-dev libxml2-dev libxslt-dev
RUN pip install pybind11


