<script>
    let file = null; // Archivo seleccionado por el usuario
    let uploadMessage = ''; // Mensaje de éxito o error

    async function handleFileUpload() {
        if (!file) {
            uploadMessage = 'Please select a file to upload.';
            return;
        }

        try {
            // Crear un objeto FormData para enviar el archivo
            const formData = new FormData();
            formData.append('file', file);

            // Realizar la solicitud POST al endpoint
            const response = await fetch('http://localhost:81/storage/upload', {
                method: 'POST',
                headers: {
                    'X-Filename': file.name // Enviar el nombre del archivo en el encabezado
                },
                body: file // Enviar el archivo directamente en el cuerpo
            });

            if (!response.ok) {
                const errorText = await response.text();
                uploadMessage = `Error: ${errorText}`;
                return;
            }

            // Obtener la respuesta del servidor
            const successText = await response.text();
            uploadMessage = successText; // Mostrar el mensaje de éxito
        } catch (error) {
            console.error('Error uploading file:', error);
            uploadMessage = 'An error occurred while uploading the file.';
        }
    }

    function handleFileChange(event) {
        file = event.target.files[0]; // Obtener el archivo seleccionado
        uploadMessage = ''; // Limpiar el mensaje anterior
    }
</script>

<div class="file-upload">
    <h2>Upload a File</h2>
    <input type="file" on:change={handleFileChange} />
    <button on:click={handleFileUpload}>Upload</button>

    {#if uploadMessage}
        <p class="upload-message">{uploadMessage}</p>
    {/if}
</div>

<style>
    .file-upload {
        display: flex;
        flex-direction: column;
        gap: 10px;
        max-width: 400px;
        margin: 0 auto;
    }

    input[type="file"] {
        padding: 5px;
    }

    button {
        padding: 10px 20px;
        background-color: #007BFF;
        color: white;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        font-size: 16px;
    }

    button:hover {
        background-color: #0056b3;
    }

    .upload-message {
        margin-top: 10px;
        font-size: 14px;
        color: #333;
    }

    .upload-message.error {
        color: #ff4d4d;
    }
</style>