<script>
    import { onMount } from 'svelte';
    export let onClose; // Función para cerrar el panel, recibida desde el componente padre

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

     function handleKeyDown(event) {
        if (event.key === 'Escape') {
            onClose(); // Llamar a la función para cerrar el panel
        }
    }

    // Agregar y eliminar el evento de teclado
    onMount(() => {
        document.addEventListener('keydown', handleKeyDown);
        return () => {
            document.removeEventListener('keydown', handleKeyDown);
        };
    });
</script>

<div class="file-upload-panel">
    <button class="close-button" on:click={onClose}>X</button>
    <div class="file-upload">
        <h2>Upload a File</h2>
        <input type="file" on:change={handleFileChange} />
        <button on:click={handleFileUpload}>Upload</button>

        {#if uploadMessage}
            <p class="upload-message">{uploadMessage}</p>
        {/if}
    </div>
</div>

<style>
    .file-upload-panel {
        position: fixed;
        top: 0;
        right: 0;
        width: 100%; /* Ocupa el 50% del ancho disponible */
        height: 100%;
        background-color: white;
        box-shadow: -2px 0 5px rgba(0, 0, 0, 0.1);
        z-index: 1000;
        display: flex;
        flex-direction: column;
        padding: 1rem;
    }

    .close-button {
        align-self: flex-end;
        background: none;
        border: none;
        font-size: 18px;
        font-weight: bold;
        cursor: pointer;
        color: #333;
    }

    .close-button:hover {
        color: #ff4d4d;
    }

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