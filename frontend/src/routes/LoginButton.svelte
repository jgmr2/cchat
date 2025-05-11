<button class="login-button" on:click={goToLogin}>Login</button>

<script>
    import { goto } from '$app/navigation';

    function goToLogin() {
        goto('/login'); // Cambia '/login' por la ruta de tu página de login
    }
</script>

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