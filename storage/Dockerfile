# Etapa 1: Compilación
FROM builder_image AS builder

WORKDIR /app/src
COPY src/ .

RUN make

# Etapa 2: Imagen final
FROM alpine:latest

WORKDIR /app
COPY --from=builder /app/src/endpoint .

EXPOSE 8080
CMD ["./endpoint"]
