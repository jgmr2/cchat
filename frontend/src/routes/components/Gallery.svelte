<script>
    export let searchResults = []; // Recevoir les résultats comme propriété
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
        <p class="no-results">Aucun résultat à afficher.</p>
    {/if}
</div>

<style>
    .gallery {
        position: fixed; /* Fixé à l'écran */
        top: 100px; /* Marge supérieure */
        left: 50%; /* Centrer horizontalement */
        transform: translateX(-50%); /* Ajuster le centrage */
        width: 100%; 
        max-width: 1500px; /* Limiter la largeur maximale */
        height: calc(95vh - 120px); /* Hauteur dynamique */
        overflow-y: auto; /* Activer le défilement si le contenu dépasse la hauteur */
        
        padding: 20px; /* Espacement interne */
        z-index: 1;
    }

    .gallery-list {
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); /* Design responsive */
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

    /* Media queries pour écrans moyens */
    @media (max-width: 1480px) {
        .gallery {
            top: 100px; /* Réduire la marge supérieure */
            height: calc(95vh - 110px); /* Ajuster la hauteur */
            padding: 18px; /* Réduire le padding */
            width: 95%; /* Réduire la largeur */
            max-width: 1200px; /* Limiter la largeur maximale */
        }

        .gallery-list {
            grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); /* Ajuster la taille minimale des colonnes */
        }
    }

    /* Media queries pour petits écrans */
    @media (max-width: 768px) {
        .gallery {
            top: 100px; /* Réduire la marge supérieure */
            height: calc(95vh - 100px); /* Ajuster la hauteur */
            padding: 15px; /* Réduire le padding */
            width: 90%; /* Réduire la largeur */
            max-width: 600px; /* Limiter la largeur maximale */
        }

        .gallery-list {
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr)); /* Réduire la taille minimale des colonnes */
            gap: 10px; /* Réduire l'espace entre les éléments */
        }

        .gallery-item {
            padding: 8px; /* Réduire le padding */
        }

        .gallery-image {
            max-height: 120px; /* Réduire la hauteur maximale des images */
        }

        .gallery-name {
            font-size: 0.9rem; /* Réduire la taille du texte */
        }
    }

    /* Media queries pour très petits écrans */
    @media (max-width: 480px) {
        .gallery {
            top: 100px; /* Réduire la marge supérieure */
            height: calc(95vh - 100px); /* Ajuster la hauteur */
            padding: 10px; /* Réduire le padding */
            width: 95%; /* Réduire la largeur */
            max-width: 400px; /* Limiter la largeur maximale */
        }

        .gallery-list {
            grid-template-columns: repeat(auto-fit, minmax(120px, 1fr)); /* Réduire encore plus la taille minimale des colonnes */
        }

        .gallery-item {
            padding: 5px; /* Réduire le padding */
        }

        .gallery-image {
            max-height: 100px; /* Réduire la hauteur maximale des images */
        }

        .gallery-name {
            font-size: 0.8rem; /* Réduire encore plus la taille du texte */
        }
    }

    /* Media queries pour écrans très petits */
    @media (max-width: 360px) {
        .gallery {
            top: 100px; /* Réduire la marge supérieure */
            height: calc(95vh - 100px); /* Ajuster la hauteur */
            padding: 8px; /* Réduire le padding */
            width: 100%; /* Utiliser toute la largeur */
            max-width: 320px; /* Limiter la largeur maximale */
        }

        .gallery-list {
            grid-template-columns: repeat(auto-fit, minmax(100px, 1fr)); /* Réduire la taille minimale des colonnes */
        }

        .gallery-item {
            padding: 4px; /* Réduire le padding */
        }

        .gallery-image {
            max-height: 80px; /* Réduire la hauteur maximale des images */
        }

        .gallery-name {
            font-size: 0.7rem; /* Réduire la taille du texte */
        }
    }
</style>