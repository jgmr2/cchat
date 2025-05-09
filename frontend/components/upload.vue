<template>
    <div>
      <h1>Upload Page</h1>
      <p>This is the upload page of the CChat application.</p>
      <form @submit.prevent="uploadFile">
        <input type="file" @change="handleFileChange" />
        <button type="submit">Upload</button>
      </form>
      <p v-if="message">{{ message }}</p>
    </div>
  </template>
  
  <script>
  export default {
    data() {
      return {
        selectedFile: null, // Archivo seleccionado
        message: "", // Mensaje de éxito o error
      };
    },
    methods: {
      handleFileChange(event) {
        // Manejar el archivo seleccionado
        this.selectedFile = event.target.files[0];
      },
      async uploadFile() {
        if (!this.selectedFile) {
          this.message = "Please select a file to upload.";
          return;
        }
  
        try {
          // Crear un objeto FormData para enviar el archivo
          const formData = new FormData();
          formData.append("file", this.selectedFile);
  
          // Realizar la solicitud POST a la API
          const response = await fetch("http://localhost:81/storage/upload", {
            method: "POST",
            headers: {
              "X-Filename": this.selectedFile.name, // Enviar el nombre del archivo en el encabezado
            },
            body: this.selectedFile, // Enviar el archivo directamente
          });
  
          if (response.ok) {
            const result = await response.text();
            this.message = `File uploaded successfully: ${result}`;
          } else {
            const error = await response.text();
            this.message = `Error uploading file: ${error}`;
          }
        } catch (error) {
          this.message = `Error uploading file: ${error.message}`;
        }
      },
    },
  };
  </script>
  
  <style scoped>
  h1 {
    color: #333;
  }
  form {
    margin-top: 20px;
  }
  button {
    margin-top: 10px;
    padding: 10px 20px;
    background-color: #007bff;
    color: white;
    border: none;
    border-radius: 5px;
    cursor: pointer;
  }
  button:hover {
    background-color: #0056b3;
  }
  p {
    margin-top: 10px;
    color: #555;
  }
  </style>