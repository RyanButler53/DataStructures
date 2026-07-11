FROM gcc:16.1

COPY . /home/DataStructures
WORKDIR /home/DataStructures
RUN apt-get update
RUN apt-get install cmake -y
RUN apt-get install python3.13-dev -y
# RUN apt-get install python3.13-venv -y