FROM ubuntu:16.04
RUN mkdir /code
WORKDIR /code
RUN apt-get update && apt-get -y install libssl-dev libxerces-c-dev libpqxx-dev gcc g++ make valgrind postgresql-9.5
ADD . /code/

