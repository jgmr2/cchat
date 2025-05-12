<script>
    import { onMount } from 'svelte';
    export let onClose;

    let file = null;
    let description = '';
    let tags = [];
    let tagInput = '';
    let uploadMessage = '';

    async function handleFileUpload() {
        if (!file) {
            uploadMessage = 'Please select a file to upload.';
            return;
        }

        if (!description.trim()) {
            uploadMessage = 'Please provide a description for the file.';
            return;
        }

        try {
            const token = localStorage.getItem('sessionToken');

            if (!token) {
                uploadMessage = 'User is not authenticated.';
                return;
            }

            const headers = {
                'Authorization': `Bearer ${token}`,
                'X-Filename': file.name,
                'X-Description': description,
                'X-Tags': JSON.stringify(tags),
            };

            const response = await fetch('http://localhost:81/storage/upload', {
                method: 'POST',
                headers: headers,
                body: file // Enviar solo el archivo como cuerpo de la solicitud
            });

            if (!response.ok) {
                const errorText = await response.text();
                uploadMessage = `Error: ${errorText}`;
                return;
            }

            const successText = await response.text();
            uploadMessage = successText;
        } catch (error) {
            console.error('Error uploading file:', error);
            uploadMessage = 'An error occurred while uploading the file.';
        }
    }

    function handleFileChange(event) {
        file = event.target.files[0];
        uploadMessage = '';
    }

    function handleTagInput(event) {
        if (event.key === 'Enter' || event.key === ' ') {
            const newTag = tagInput.trim();
            if (newTag && !tags.includes(newTag)) {
                tags = [...tags, newTag];
            }
            tagInput = '';
        }
    }

    function removeTag(tagToRemove) {
        tags = tags.filter(tag => tag !== tagToRemove);
    }

    function handleKeyDown(event) {
        if (event.key === 'Escape') {
            onClose();
        }
    }

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
        <textarea
            placeholder="Enter a description for the file..."
            bind:value={description}
        ></textarea>
        <div class="tags-input">
            <input
                type="text"
                placeholder="Add tags (press Enter or Space)..."
                bind:value={tagInput}
                on:keydown={handleTagInput}
            />
            <div class="tags">
                {#each tags as tag}
                    <span class="tag">
                        #{tag}
                        <span class="remove-tag" on:click={() => removeTag(tag)}>×</span>
                    </span>
                {/each}
            </div>
        </div>
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
        width: 100%;
        height: 100%;
        background-color: white;
        box-shadow: -2px 0 5px rgba(0, 0, 0, 0.1);
        z-index: 1000;
        display: flex;
        flex-direction: column;
        padding: 1rem;
        transition: all 0.1s ease-in-out;
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

    textarea {
        width: 100%;
        height: 80px;
        padding: 10px;
        border: 1px solid #ccc;
        border-radius: 4px;
        resize: none;
    }

    .tags-input {
        display: flex;
        flex-direction: column;
        gap: 5px;
    }

    .tags {
        display: flex;
        flex-wrap: wrap;
        gap: 5px;
    }

    .tag {
        background-color: #f0f0f000;
        color: #610464;
        padding: 5px 10px;
        border-radius: 20px;
        font-size: 14px;
        display: flex;
        align-items: center;
        gap: 5px;
    }

    .remove-tag {
        font-size: 14px;
        color: #999;
        cursor: pointer;
    }

    .remove-tag:hover {
        color: #ff4d4d;
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