<script>
    import LoginForm from './LoginForm.svelte';
    import RegisterForm from './RegisterForm.svelte';
    import { goto } from '$app/navigation';

    let isLogin = true; // Estado inicial: mostrar el formulario de login

    // Verificar si existe un token de sesión
    if (localStorage.getItem('sessionToken')) {
        goto('/'); // Redirigir a la página principal
    }

    function toggleForm() {
        isLogin = !isLogin; // Alternar entre login y registro
    }
</script>

<div class="login-container">
    <h1>{isLogin ? 'Login' : 'Register'}</h1>
    {#if isLogin}
        <LoginForm />
    {:else}
        <RegisterForm />
    {/if}
    <button class="toggle-button" on:click={toggleForm}>
        {isLogin ? 'Switch to Register' : 'Switch to Login'}
    </button>
</div>

<style>
    .login-container {
        max-width: 400px;
        margin: 50px auto;
        padding: 20px;
        border: 1px solid #ddd;
        border-radius: 8px;
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        text-align: center;
    }

    .toggle-button {
        margin-top: 20px;
        padding: 10px 20px;
        background-color: #007BFF;
        color: white;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        font-size: 14px;
        transition: background-color 0.3s;
    }

    .toggle-button:hover {
        background-color: #0056b3;
    }
</style>