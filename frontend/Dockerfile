FROM node:20-alpine

# Crear y usar la carpeta de trabajo
WORKDIR /home/node/app

# Copiamos package.json y lock
COPY package*.json ./

# Instalamos dependencias
RUN npm install

# Copiamos todo (esto luego se sobreescribe por el volumen)
COPY . .

# Variables necesarias
ENV NUXT_HOST=0.0.0.0
ENV NUXT_PORT=3000

# Exponemos el puerto para Nuxt
EXPOSE 3000

# Ejecutamos el servidor en modo desarrollo
CMD ["npm", "run", "dev"]


