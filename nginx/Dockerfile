# Etapa 1: Construcción del frontend
FROM node:18 AS builder

WORKDIR /app

COPY frontend/package*.json ./
COPY frontend/ ./

RUN npm install
RUN npm run build

# Etapa 2: Configuración de Nginx
FROM nginx:alpine

# Copiar el build generado al directorio predeterminado de Nginx
COPY --from=builder /app/build /usr/share/nginx/html

# Copiar el archivo de configuración de Nginx
COPY nginx.conf /etc/nginx/nginx.conf

# Exponer el puerto 80
EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]