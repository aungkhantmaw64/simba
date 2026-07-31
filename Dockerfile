FROM espressif/idf:v5.5.1

RUN apt-get update && apt-get install -y \
  ruby-full gdb gcovr

RUN gem install ceedling:1.1.1 --no-document

ENV APP_PATH=/app

WORKDIR /app
