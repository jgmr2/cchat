<script>
    import { createEventDispatcher } from 'svelte';

    let username = '';
    const dispatch = createEventDispatcher();

    // Vérifier si nous sommes côté client
    if (typeof window !== 'undefined') {
        const sessionToken = localStorage.getItem('sessionToken');

        // Décoder le token pour obtenir le nom d'utilisateur
        if (sessionToken) {
            try {
                const payload = JSON.parse(atob(sessionToken.split('.')[1])); // Décoder le payload du JWT
                username = payload.username || 'Utilisateur inconnu'; // Extraire le nom d'utilisateur du payload
            } catch (err) {
                console.error('Erreur lors du décodage du token :', err);
            }
        }
    }

    function handleClick() {
        dispatch('toggleSidebar'); // Émettre l'événement au composant parent
    }
</script>

<button class="login-button" on:click={handleClick}>{username}</button>

<style>
    .login-button {
        position: fixed;
        top: 30px;
        right: 20px;
        padding: 8px 16px;
        background-color: transparent;
        color: #007BFF;
        border: 1px solid #007BFF;
        border-radius: 4px;
        cursor: pointer;
        font-size: 14px;
        transition: background-color 0.3s, color 0.3s;
        z-index: 1000; 
    }

    .login-button:hover {
        background-color: #007BFF;
        color: white;
    }
</style>