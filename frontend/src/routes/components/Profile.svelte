<script>
    import { createEventDispatcher } from 'svelte';

    let username = '';
    const dispatch = createEventDispatcher();

    // Verificar si estamos en el cliente
    if (typeof window !== 'undefined') {
        const sessionToken = localStorage.getItem('sessionToken');

        // Decodificar el token para obtener el correo del usuario
        if (sessionToken) {
            try {
                const payload = JSON.parse(atob(sessionToken.split('.')[1])); // Decodificar el payload del JWT
                username = payload.username || 'Unknown User'; // Extraer el username del payload
            } catch (err) {
                console.error('Error decoding token:', err);
            }
        }
    }

    function handleClick() {
        dispatch('toggleSidebar'); // Emitir el evento al componente padre
    }
</script>

<button class="login-button" on:click={handleClick}>{username}</button>

<style>
    .login-button {
        position: fixed;
        top: 10px;
        right: 10px;
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