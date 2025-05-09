FROM jgmr2/calpine-mongoxxx4.1.0-crow:latest AS builder
WORKDIR /app
COPY ./src /app
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
RUN make

FROM jgmr2/calpine:latest
COPY --from=builder /app/app /app/app
COPY --from=builder /opt/mongocxx /usr/local
WORKDIR /app
EXPOSE 80
CMD ["./app"]