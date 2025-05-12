<script>
    export let searchResults = []; // Recibir los resultados como propiedad
</script>

<div class="gallery">
    {#if searchResults.length > 0}
        <ul class="gallery-list">
            {#each searchResults as result}
                <li class="gallery-item">
                    <img src={result.url} alt={result.name} class="gallery-image" />
                    <p class="gallery-name">{result.name}</p>
                </li>
            {/each}
        </ul>
    {:else}
        <p class="no-results">No hay resultados para mostrar.</p>
    {/if}
    
    
</div>

<style>
    .gallery {
        position: fixed; /* Fijo en la pantalla */
        top: 100px; /* Margen superior para evitar que sea tapado por la barra de búsqueda */
        left: 50%; /* Centrar horizontalmente */
        transform: translateX(-50%); /* Ajustar el centrado */
        width: 100%; 
        max-width: 1500px; /* Limitar el ancho máximo */
        height: calc(95vh - 120px); /* Altura dinámica para ocupar el espacio restante */
        overflow-y: auto; /* Habilitar scroll si el contenido excede la altura */
        background-color: #f9f9f9;
        border-radius: 5px;
        box-shadow: 0 4px 10px rgba(0, 0, 0, 0.678);
        padding: 20px; /* Espaciado interno */
        z-index: 1;
    }

    .gallery-list {
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
        gap: 15px;
        list-style: none;
        padding: 0;
        margin: 0;
        width: 100%;
    }

    .gallery-item {
        display: flex;
        flex-direction: column;
        align-items: center;
        text-align: center;
        background-color: #ffffff;
        border-radius: 8px;
        padding: 10px;
        box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
        transition: transform 0.2s ease, box-shadow 0.2s ease;
    }

    .gallery-item:hover {
        transform: translateY(-5px);
        box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
    }

    .gallery-image {
        width: 100%;
        height: auto;
        border-radius: 8px;
        margin-bottom: 10px;
        object-fit: cover;
        max-height: 150px;
    }

    .gallery-name {
        font-size: 1rem;
        font-weight: 500;
        color: #333;
        margin: 0;
    }

    .no-results {
        font-size: 1.2rem;
        color: #666;
        margin-top: 20px;
    }
</style>