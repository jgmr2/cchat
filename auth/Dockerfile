FROM jgmr2/calpine-mongoxxx:latest AS builder

WORKDIR /app

# Clonar headers necesarios
# RUN wget -O /app/httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
# RUN wget -O /app/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp

COPY ./src /app
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

RUN make

# Stage 2: Runtime
FROM jgmr2/calpine:latest

COPY --from=builder /app/app /app/app
COPY --from=builder /opt/mongocxx /usr/local

WORKDIR /app
EXPOSE 80
CMD ["./app"]
