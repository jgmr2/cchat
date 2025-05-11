FROM jgmr2/calpine-mongoxxx4.1.0-crow:latest AS builder
WORKDIR /app
COPY ./src /app
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
RUN make

FROM jgmr2/calpine:latest
RUN apk add --no-cache \
    bash \
    fuse \
    s3fs-fuse \
    curl \
    ca-certificates
    
RUN mkdir -p /mnt/cdb
COPY passwd-s3fs /etc/passwd-s3fs
RUN chmod 600 /etc/passwd-s3fs
COPY --from=builder /app/app /app/app
COPY --from=builder /opt/mongocxx /usr/local
WORKDIR /app
EXPOSE 80
CMD sh -c "until curl -s http://minio:9000/minio/health/ready; do echo 'Esperando a MinIO...'; sleep 2; done && \
    s3fs cdb /mnt/cdb \
    -o url=http://minio:9000 \
    -o use_path_request_style \
    -o passwd_file=/etc/passwd-s3fs \
    -o allow_other && \
    ./app"
